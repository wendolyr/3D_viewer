#include "MainWidget.h"

#include <QApplication>
#include <QComboBox>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QFileInfo>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QResizeEvent>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

#include "../controller/facade.h"
#include "Builder/TemplateAxisControlBuilder.h"
#include "Builder/TransformControlBuilder.h"
#include "CyclicDoubleSpinBox.h"
#include "OpenGLWidget.h"

// public
MainWidget::MainWidget(QWidget* parent) : QWidget(parent), full_file_name_("") {
  MainWidget::SetupUI();
  s21::ViewParams params;
  if (control_.LoadLastState(params)) {
    edge_r_color->setValue(params.edge_color.x);
    edge_g_color->setValue(params.edge_color.y);
    edge_b_color->setValue(params.edge_color.z);
    edge_thickness_->setValue(params.edge_thickness);
    edge_type_combo_->setCurrentIndex(params.edge_type);
    UpdateEdgeSettings();

    vertex_r_color_->setValue(params.vertex_color.x);
    vertex_g_color_->setValue(params.vertex_color.y);
    vertex_b_color_->setValue(params.vertex_color.z);
    vertex_size_->setValue(params.vertex_size);
    vertex_display_combo_->setCurrentIndex(params.vertex_display);
    UpdateVertexSettings();

    background_color_r_->setValue(params.background_color.x);
    background_color_g_->setValue(params.background_color.y);
    background_color_b_->setValue(params.background_color.z);
    UpdateBackground();

    /**
     * на экране кнопка всегда центральная
     */
    gl_widget_->SetProjectionType(
        static_cast<OpenGLWidget::ProjectionType>(params.projection_type));

    // если базовое имя файла поменялось, значит файл с моделью существовал и
    // был корректным
    if (params.file_name != ".obj" && control_.GetVertices().size()) {
      full_file_name_ = QString::fromStdString(params.file_name);
      s21::Params affine = control_.GetCurrentSettings();
      move_x_->setValue(affine.shift.x);
      move_y_->setValue(affine.shift.y);
      move_z_->setValue(affine.shift.z);
      rotate_x_->setValue(affine.rotation.x);
      rotate_y_->setValue(affine.rotation.y);
      rotate_z_->setValue(affine.rotation.z);
      scale_->setValue(affine.scale);
    }
  } else {
    MainWidget::ResetDisplay();
  }
}

void MainWidget::showEvent(QShowEvent* event) {
  QWidget::showEvent(event);
  if (control_.GetVertices().size() > 0) {
    gl_widget_->SetModelData(control_.GetVertices(), control_.GetEdges());
  }
}

MainWidget::~MainWidget() {
  s21::ViewParams params;
  params.projection_type = static_cast<int>(gl_widget_->GetProjectionType());
  params.edge_color.x = edge_r_color->value();
  params.edge_color.y = edge_g_color->value();
  params.edge_color.z = edge_b_color->value();
  params.edge_thickness = edge_thickness_->value();
  params.edge_type = edge_type_combo_->currentIndex();

  params.vertex_color.x = vertex_r_color_->value();
  params.vertex_color.y = vertex_g_color_->value();
  params.vertex_color.z = vertex_b_color_->value();
  params.vertex_size = vertex_size_->value();
  params.vertex_display = vertex_display_combo_->currentIndex();
  params.background_color.x = background_color_r_->value();
  params.background_color.y = background_color_g_->value();
  params.background_color.z = background_color_b_->value();
  params.file_name = full_file_name_.toStdString();

  control_.SaveModel(params);
}

// public slots
void MainWidget::LoadModel() {
  // Открываем диалог выбора файла
  QString file_name = QFileDialog::getOpenFileName(
      this, tr("Выберите файл модели"), QDir::homePath(),
      tr("OBJ Files (*.obj);;All Files (*)"));

  if (!file_name.isEmpty()) {
    // Извлекаем только имя файла для отображения
    QFileInfo file_info(file_name);
    full_file_name_ = file_info.fileName();

    // if (!MainWidget::LoadModelData(file_name)) {
    //   MainWidget::UpdateFileNameLabel();
    //   MainWidget::ResetTransform();
    // }
    MainWidget::ResetTransform();
    control_.ParseFile(file_name.toStdString());
    MainWidget::UpdateFileNameLabel();

    gl_widget_->SetModelData(control_.GetVertices(), control_.GetEdges());
  }
}

// protected
void MainWidget::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);
  // При изменении размера пересчитываем образанное имя
  MainWidget::UpdateFileNameLabel();
}

// private
// Следует разделить на блоки: ---, ---, сборка боковой панели, подключение, ---
void MainWidget::SetupUI() {
  // Главная горизонтальная разметка
  QHBoxLayout* main_layout = new QHBoxLayout(this);
  main_layout->setContentsMargins(10, 10, 10, 10);
  main_layout->setSpacing(15);

  gl_widget_ = new OpenGLWidget;
  gl_widget_->setMinimumSize(640, 480);
  main_layout->addWidget(gl_widget_, 7);  // 70% ширины

  // Боковая панель управления
  QWidget* sidebar = new QWidget;
  QVBoxLayout* sidebar_layout = new QVBoxLayout(sidebar);
  sidebar_layout->setAlignment(Qt::AlignTop);
  sidebar_layout->setContentsMargins(5, 5, 5, 5);
  sidebar_layout->setSpacing(10);

  // Группа загрузки модели
  QPushButton* load_btn;
  QGroupBox* load_group = CreateLoadGroup(load_btn, file_name_label_);
  // Группа информации о модели
  QGroupBox* info_group =
      CreateInfoGroup(vertex_count_label_, edge_count_label_);
  // Группа записи
  QPushButton *gif_btn, *screen_btn;
  QGroupBox* record_group = CreateRecordGroup(gif_btn, screen_btn);

  // Создание области скрола
  QScrollArea* scroll_area = new QScrollArea;
  scroll_area->setWidgetResizable(true);
  scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  QWidget* scroll_content = new QWidget;
  QVBoxLayout* scroll_layout = new QVBoxLayout(scroll_content);
  scroll_layout->setAlignment(Qt::AlignTop);

  // Группа перемещения
  QGroupBox* move_group =
      TransformBuilder<QDoubleSpinBox>(TransformType::Move)
          .AddAxis(
              Axis::X, GetStepValue(TransformType::Move), move_x_,
              [this]() { ChangeValue(move_x_, TransformType::Move, false); },
              [this]() {
                move_x_->setValue(
                    DoStep(move_x_->value(), true, TransformType::Move));
              })
          .AddAxis(
              Axis::Y, GetStepValue(TransformType::Move), move_y_,
              [this]() {
                move_y_->setValue(
                    DoStep(move_y_->value(), false, TransformType::Move));
              },
              [this]() {
                move_y_->setValue(
                    DoStep(move_y_->value(), true, TransformType::Move));
              })
          .AddAxis(
              Axis::Z, GetStepValue(TransformType::Move), move_z_,
              [this]() {
                move_z_->setValue(
                    DoStep(move_z_->value(), false, TransformType::Move));
              },
              [this]() {
                move_z_->setValue(
                    DoStep(move_z_->value(), true, TransformType::Move));
              })
          .Build();
  // Группа поворота
  QGroupBox* rotate_group =
      TransformBuilder<CyclicDoubleSpinBox>(TransformType::Rotate)
          .AddAxis(
              Axis::X, GetStepValue(TransformType::Rotate), rotate_x_,
              [this]() {
                rotate_x_->setValue(
                    DoStep(rotate_x_->value(), false, TransformType::Rotate));
              },
              [this]() {
                rotate_x_->setValue(
                    DoStep(rotate_x_->value(), true, TransformType::Rotate));
              })
          .AddAxis(
              Axis::Y, GetStepValue(TransformType::Rotate), rotate_y_,
              [this]() {
                rotate_y_->setValue(
                    DoStep(rotate_y_->value(), false, TransformType::Rotate));
              },
              [this]() {
                rotate_y_->setValue(
                    DoStep(rotate_y_->value(), true, TransformType::Rotate));
              })
          .AddAxis(
              Axis::Z, GetStepValue(TransformType::Rotate), rotate_z_,
              [this]() {
                rotate_z_->setValue(
                    DoStep(rotate_z_->value(), false, TransformType::Rotate));
              },
              [this]() {
                rotate_z_->setValue(
                    DoStep(rotate_z_->value(), true, TransformType::Rotate));
              })
          .Build();
  // Группа масштабирования
  QGroupBox* scale_group =
      TransformBuilder<QDoubleSpinBox>(TransformType::Scale)
          .AddAxis(
              Axis::None, GetStepValue(TransformType::Scale), scale_,
              [this]() {
                scale_->setValue(
                    DoStep(scale_->value(), false, TransformType::Scale));
              },
              [this]() {
                scale_->setValue(
                    DoStep(scale_->value(), true, TransformType::Scale));
              })
          .Build();
  // Группа: Настройки проекции
  QRadioButton *parallel_btn, *central_btn;
  QGroupBox* projection_group =
      CreateProjectionGroup(parallel_btn, central_btn);

  // Группа: Настройки рёбер
  QGroupBox* edge_settings_group = new QGroupBox("Настройки рёбер");
  QVBoxLayout* edge_layout = new QVBoxLayout(edge_settings_group);

  // Тип линии
  QWidget* edge_type_widget = new QWidget;
  QHBoxLayout* edge_type_layout = new QHBoxLayout(edge_type_widget);
  edge_type_layout->setContentsMargins(0, 0, 0, 0);
  edge_type_layout->addWidget(new QLabel("Тип линии:"));
  edge_type_combo_ = new QComboBox;
  edge_type_combo_->addItem("Сплошная");
  edge_type_combo_->addItem("Пунктирная");
  edge_type_layout->addWidget(edge_type_combo_);
  edge_layout->addWidget(edge_type_widget);

  // Цвет ребра
  QWidget* edge_color_widget = CreateColorWidget(
      edge_r_color, edge_g_color, edge_b_color, edge_color_preview_, "Цвет:");
  edge_layout->addWidget(edge_color_widget);

  // Толщина ребра
  QWidget* edge_thickness_widget = new QWidget;
  QHBoxLayout* edge_thickness_layout = new QHBoxLayout(edge_thickness_widget);
  edge_thickness_layout->setContentsMargins(0, 0, 0, 0);
  edge_thickness_layout->addWidget(new QLabel("Толщина:"));
  edge_thickness_ = new QDoubleSpinBox;
  edge_thickness_->setRange(0.1, 10.0);
  edge_thickness_->setSingleStep(0.1);
  edge_thickness_layout->addWidget(edge_thickness_);
  edge_layout->addWidget(edge_thickness_widget);

  // Группа: Настройки вершин
  QGroupBox* vertex_settings_group = new QGroupBox("Настройки вершин");
  QVBoxLayout* vertex_layout = new QVBoxLayout(vertex_settings_group);

  // Способ отображения
  QWidget* vertex_display_widget = new QWidget;
  QHBoxLayout* vertex_display_layout = new QHBoxLayout(vertex_display_widget);
  vertex_display_layout->setContentsMargins(0, 0, 0, 0);
  vertex_display_layout->addWidget(new QLabel("Отображение:"));
  vertex_display_combo_ = new QComboBox;
  vertex_display_combo_->addItem("Отсутствует");
  vertex_display_combo_->addItem("Круг");
  vertex_display_combo_->addItem("Квадрат");
  vertex_display_layout->addWidget(vertex_display_combo_);
  vertex_layout->addWidget(vertex_display_widget);

  // Цвет вершин
  QWidget* vertex_color_widget =
      CreateColorWidget(vertex_r_color_, vertex_g_color_, vertex_b_color_,
                        vertex_color_preview_, "Цвет:");
  vertex_layout->addWidget(vertex_color_widget);

  // Размер вершин
  QWidget* vertex_size_widget = new QWidget;
  QHBoxLayout* vertex_size_layout = new QHBoxLayout(vertex_size_widget);
  vertex_size_layout->setContentsMargins(0, 0, 0, 0);
  vertex_size_layout->addWidget(new QLabel("Размер:"));
  //   QDoubleSpinBox* vertex_size = new QDoubleSpinBox;
  vertex_size_ = new QDoubleSpinBox;
  vertex_size_->setDecimals(1);
  vertex_size_->setRange(0.1, 25.0);
  vertex_size_->setSingleStep(0.1);
  vertex_size_layout->addWidget(vertex_size_);
  vertex_layout->addWidget(vertex_size_widget);

  // Группа: Настройки фона
  QGroupBox* bg_settings_group = new QGroupBox("Настройки фона");
  QVBoxLayout* bg_layout = new QVBoxLayout(bg_settings_group);

  // Цвет фона
  QWidget* bg_color_widget = CreateColorWidget(
      background_color_r_, background_color_g_, background_color_b_,
      background_color_preview_, "Цвет фона:");
  bg_layout->addWidget(bg_color_widget);

  scroll_layout->addWidget(move_group);
  scroll_layout->addWidget(rotate_group);
  scroll_layout->addWidget(scale_group);
  scroll_layout->addWidget(projection_group);
  scroll_layout->addWidget(edge_settings_group);
  scroll_layout->addWidget(vertex_settings_group);
  scroll_layout->addWidget(bg_settings_group);

  scroll_area->setWidget(scroll_content);

  // Группа сброса преобразований
  QGroupBox* reset_group = new QGroupBox("Сброс");
  QHBoxLayout* reset_layout = new QHBoxLayout;
  QPushButton* reset_model_btn = new QPushButton("Сброс\nпреобразования");
  QPushButton* reset_view_btn = new QPushButton("Сброс\nотображения");

  QFontMetrics font_metrics(reset_model_btn->font());
  int min_height = font_metrics.lineSpacing() * 2;
  reset_model_btn->setMinimumHeight(min_height);
  reset_view_btn->setMinimumHeight(min_height);
  reset_model_btn->setStyleSheet("text-align: center;");
  reset_view_btn->setStyleSheet("text-align: center;");
  reset_model_btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  reset_view_btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  reset_layout->addWidget(reset_model_btn);
  reset_layout->addWidget(reset_view_btn);

  reset_group->setLayout(reset_layout);

  // Сбор боковой панели
  sidebar->setMinimumWidth(360);
  scroll_content->setMinimumWidth(340);

  sidebar_layout->addWidget(load_group);
  sidebar_layout->addWidget(info_group);
  sidebar_layout->addWidget(record_group);
  sidebar_layout->addWidget(scroll_area);
  sidebar_layout->addWidget(reset_group);
  main_layout->addWidget(sidebar, 3);

  // Подключение сигналов и слотов
  connect(load_btn, &QPushButton::clicked, this,
          &MainWidget::LoadModel);  // Загрузка модели
  // TODO Подключение к кнопке гиф и скриншота
  // Перемещение модели
  connect(move_x_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWidget::OnTransformChanged);
  connect(move_y_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWidget::OnTransformChanged);
  connect(move_z_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWidget::OnTransformChanged);
  // Поворот модели
  connect(rotate_x_, QOverload<double>::of(&CyclicDoubleSpinBox::valueChanged),
          this, &MainWidget::OnTransformChanged);
  connect(rotate_y_, QOverload<double>::of(&CyclicDoubleSpinBox::valueChanged),
          this, &MainWidget::OnTransformChanged);
  connect(rotate_z_, QOverload<double>::of(&CyclicDoubleSpinBox::valueChanged),
          this, &MainWidget::OnTransformChanged);
  // Масштаб модели
  connect(scale_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWidget::OnTransformChanged);
  // Проекция
  connect(parallel_btn, &QRadioButton::toggled, [this](bool checked) {
    if (checked) gl_widget_->SetProjectionType(OpenGLWidget::Parallel);
  });
  connect(central_btn, &QRadioButton::toggled, [this](bool checked) {
    if (checked) gl_widget_->SetProjectionType(OpenGLWidget::Central);
  });

  // Настройки ребер
  // Отображение ребер
  connect(edge_type_combo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this,
          &MainWidget::UpdateEdgeSettings);  // Тип отображения
  connect(edge_thickness_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          this,
          &MainWidget::UpdateEdgeSettings);  // Толщина ребра
  // Настройка цвета ребер с помощью RGB
  connect(edge_r_color, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWidget::UpdateEdgeSettings);
  connect(edge_g_color, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWidget::UpdateEdgeSettings);
  connect(edge_b_color, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWidget::UpdateEdgeSettings);

  // Настройки вершин
  // Отображение вершин
  connect(vertex_display_combo_,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &MainWidget::UpdateVertexSettings);  // Тип отображения
  connect(vertex_size_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          this,
          &MainWidget::UpdateVertexSettings);  // Размер вершин
  // Настройка цвета вершин с помощью RGB
  connect(vertex_r_color_, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWidget::UpdateVertexSettings);
  connect(vertex_g_color_, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWidget::UpdateVertexSettings);
  connect(vertex_b_color_, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWidget::UpdateVertexSettings);

  // Настройки фона
  // Настройка цвета вершин с помощью RGB
  connect(background_color_r_, QOverload<int>::of(&QSpinBox::valueChanged),
          this, &MainWidget::UpdateBackground);
  connect(background_color_g_, QOverload<int>::of(&QSpinBox::valueChanged),
          this, &MainWidget::UpdateBackground);
  connect(background_color_b_, QOverload<int>::of(&QSpinBox::valueChanged),
          this, &MainWidget::UpdateBackground);

  // Cброс модели
  connect(reset_model_btn, &QPushButton::clicked, this,
          &MainWidget::ResetTransform);
  // Сброс вида
  connect(reset_view_btn, &QPushButton::clicked, this,
          &MainWidget::ResetDisplay);

  connect(gl_widget_, &OpenGLWidget::wheelScrolled, this,
          &MainWidget::OnWheelScrolled);

  connect(gl_widget_, &OpenGLWidget::rotationDeltaChanged, this,
          &MainWidget::handleRotationDelta);
}

double MainWidget::GetStepValue(TransformType type) const {
  switch (type) {
    case TransformType::Move:
      return 0.25;
    case TransformType::Rotate:
      return 1.0;
    case TransformType::Scale:
      return 0.05;
  }
}

double MainWidget::DoStep(double value, bool sum, TransformType type) {
  // Изменяем значение
  if (sum) {
    value += MainWidget::GetStepValue(type);
  } else {
    value -= MainWidget::GetStepValue(type);
  }
  // Для поворотов цикличная арифметика
  if (type == TransformType::Rotate) {
    value = fmod(value, 360.0);
    if (value < 0.0) value += 360.0;
  } else {
    const double factor = std::pow(10.0, 2);
    value = std::round(value * factor) / factor;
  }
  return value;
}

void MainWidget::UpdateFileNameLabel() {
  // Обработка событий для актуализации размеров
  QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

  // Вычисление максимальной ширины для текста
  QFontMetrics metrics(file_name_label_->font());
  int max_width = file_name_label_->width() - 10;  // Учет отступов

  // Сокращение текста с многоточием при необходимости
  QString short_name =
      metrics.elidedText(full_file_name_, Qt::ElideRight, max_width);

  file_name_label_->setText(short_name);
  file_name_label_->setToolTip(full_file_name_);  // Полное имя в подсказке

  // Обработка случая отсутствия файла
  if (full_file_name_.isEmpty()) {
    file_name_label_->setText("Файл не выбран");
    file_name_label_->setToolTip("");
  }
}

QWidget* MainWidget::CreateColorWidget(QSpinBox*& r, QSpinBox*& g, QSpinBox*& b,
                                       QLabel*& preview,
                                       const QString& labelText) {
  QWidget* container = new QWidget;
  QHBoxLayout* layout = new QHBoxLayout(container);
  layout->setContentsMargins(0, 0, 0, 0);

  // Метка названия цвета
  QLabel* color_label = new QLabel(labelText);
  color_label->setMinimumWidth(65);  // Фиксированная ширина для выравнивания

  // Компоненты RGB
  r = new QSpinBox;
  r->setRange(0, 255);
  r->setValue(0);
  r->setMaximumWidth(50);

  g = new QSpinBox;
  g->setRange(0, 255);
  g->setValue(0);
  g->setMaximumWidth(50);

  b = new QSpinBox;
  b->setRange(0, 255);
  b->setValue(0);
  b->setMaximumWidth(50);

  // Превью цвета
  preview = new QLabel;
  preview->setFixedSize(20, 20);
  preview->setStyleSheet("border: 1px solid gray;");

  layout->addWidget(color_label);
  layout->addWidget(new QLabel("R:"));
  layout->addWidget(r);
  layout->addWidget(new QLabel("G:"));
  layout->addWidget(g);
  layout->addWidget(new QLabel("B:"));
  layout->addWidget(b);
  layout->addWidget(preview);

  return container;
}

void MainWidget::UpdateEdgeSettings() {
  OpenGLWidget::EdgeType type =
      static_cast<OpenGLWidget::EdgeType>(edge_type_combo_->currentIndex());
  QVector3D color(edge_r_color->value() / 255.0f,
                  edge_g_color->value() / 255.0f,
                  edge_b_color->value() / 255.0f);
  gl_widget_->SetEdgeSettings(type, color, edge_thickness_->value());

  // Обновление превью цвета
  edge_color_preview_->setStyleSheet(
      QString("background-color: rgb(%1,%2,%3); border: 1px solid gray;")
          .arg(edge_r_color->value())
          .arg(edge_g_color->value())
          .arg(edge_b_color->value()));
}

void MainWidget::UpdateVertexSettings() {
  OpenGLWidget::VertexDisplay display =
      static_cast<OpenGLWidget::VertexDisplay>(
          vertex_display_combo_->currentIndex());
  QVector3D color(vertex_r_color_->value() / 255.0f,
                  vertex_g_color_->value() / 255.0f,
                  vertex_b_color_->value() / 255.0f);
  gl_widget_->SetVertexSettings(display, color, vertex_size_->value());

  // Обновление превью цвета
  vertex_color_preview_->setStyleSheet(
      QString("background-color: rgb(%1,%2,%3); border: 1px solid gray;")
          .arg(vertex_r_color_->value())
          .arg(vertex_g_color_->value())
          .arg(vertex_b_color_->value()));
}

void MainWidget::UpdateBackground() {
  QVector3D color(background_color_r_->value() / 255.0f,
                  background_color_g_->value() / 255.0f,
                  background_color_b_->value() / 255.0f);
  gl_widget_->SetBackgroundColor(color);

  // Обновление превью цвета
  background_color_preview_->setStyleSheet(
      QString("background-color: rgb(%1,%2,%3); border: 1px solid gray;")
          .arg(background_color_r_->value())
          .arg(background_color_g_->value())
          .arg(background_color_b_->value()));
}

// private slots
void MainWidget::ResetTransform() {
  move_x_->setValue(0.0);
  move_y_->setValue(0.0);
  move_z_->setValue(0.0);

  rotate_x_->setValue(0.0);
  rotate_y_->setValue(0.0);
  rotate_z_->setValue(0.0);

  scale_->setValue(1.0);

  OnTransformChanged();
}

void MainWidget::ResetDisplay() {
  // Установка отображения ребер: цвет RGB; толщина; тип отображения
  edge_r_color->setValue(204);
  edge_g_color->setValue(204);
  edge_b_color->setValue(255);
  edge_thickness_->setValue(1.0f);
  edge_type_combo_->setCurrentIndex(0);
  UpdateEdgeSettings();
  // Установка отображения вершин: цвет RGB; размер; тип отображения
  vertex_r_color_->setValue(255);
  vertex_g_color_->setValue(0);
  vertex_b_color_->setValue(0);
  vertex_size_->setValue(5.0f);
  vertex_display_combo_->setCurrentIndex(2);
  UpdateVertexSettings();
  // Установка цвета заднего фона RGB;
  background_color_r_->setValue(0);
  background_color_g_->setValue(0);
  background_color_b_->setValue(0);
  UpdateBackground();
}

void MainWidget::OnTransformChanged() {
  std::vector<std::vector<float>> matrix(4, std::vector<float>(4, 0.0f));
  for (int i = 0; i < 4; ++i) {
    matrix[i][i] = 1;
  }
  control_.ScaleFigure(matrix, static_cast<float>(scale_->value()));
  control_.RotateFigure(matrix, {static_cast<float>(rotate_x_->value()),
                                 static_cast<float>(rotate_y_->value()),
                                 static_cast<float>(rotate_z_->value())});
  control_.MoveFigure(matrix, {static_cast<float>(move_x_->value()),
                               static_cast<float>(move_y_->value()),
                               static_cast<float>(move_z_->value())});

  gl_widget_->NewSetTransformations(matrix);
}

// tmp разобрать потом и кровью

QGroupBox* MainWidget::CreateLoadGroup(QPushButton*& load_btn,
                                       QLabel*& file_name_label) {
  QGroupBox* group = new QGroupBox("Загрузка модели");
  QVBoxLayout* layout = new QVBoxLayout(group);

  file_name_label = new QLabel("Файл не выбран");
  file_name_label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  file_name_label->setStyleSheet(
      "padding: 3px; background-color: #F0F0F0; color: #0d0c0c;");
  file_name_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);

  load_btn = new QPushButton("Загрузить OBJ файл");

  layout->addWidget(file_name_label);
  layout->addWidget(load_btn);

  return group;
}

QGroupBox* MainWidget::CreateInfoGroup(QLabel*& vertex_count,
                                       QLabel*& edge_count) {
  QGroupBox* group = new QGroupBox("Информация о модели");
  QFormLayout* layout = new QFormLayout(group);

  vertex_count = new QLabel("0");
  edge_count = new QLabel("0");

  layout->addRow("Вершин:", vertex_count);
  layout->addRow("Ребер:", edge_count);

  return group;
}

QGroupBox* MainWidget::CreateRecordGroup(QPushButton*& gif_btn,
                                         QPushButton*& screen_btn) {
  QGroupBox* group = new QGroupBox("Запись");
  QHBoxLayout* layout = new QHBoxLayout(group);
  gif_btn = new QPushButton("GIF");
  screen_btn = new QPushButton("Изображение");
  layout->addWidget(gif_btn);
  layout->addWidget(screen_btn);
  return group;
}

QGroupBox* MainWidget::CreateProjectionGroup(QRadioButton*& parallel_btn,
                                             QRadioButton*& central_btn) {
  QGroupBox* group = new QGroupBox("Проекция");
  QVBoxLayout* layout = new QVBoxLayout(group);
  parallel_btn = new QRadioButton("Параллельная");
  central_btn = new QRadioButton("Центральная");
  central_btn->setChecked(true);
  layout->addWidget(parallel_btn);
  layout->addWidget(central_btn);
  return group;
}

void MainWidget::OnWheelScrolled(int delta) {
  // Определяем направление прокрутки (1 - вверх, -1 - вниз)
  int direction = (delta > 0) ? 1 : -1;

  // Рассчитываем новый масштаб
  double step = GetStepValue(TransformType::Scale);
  double newScale =
      scale_->value() + direction * step * 0.5;  // Медленное изменение

  // Устанавливаем границы масштабирования
  if (newScale < 0.01) newScale = 0.01;
  if (newScale > 100.0) newScale = 100.0;

  scale_->setValue(newScale);
}

void MainWidget::handleRotationDelta(float dx, float dy) {
  // Обновляем углы вращения в spinbox'ах
  float new_x = rotate_x_->value() + dx;
  float new_y = rotate_y_->value() + dy;

  // Нормализуем углы в диапазон [0, 360)
  new_x = fmod(new_x, 360.0f);
  if (new_x < 0) new_x += 360.0f;

  new_y = fmod(new_y, 360.0f);
  if (new_y < 0) new_y += 360.0f;

  rotate_x_->setValue(new_x);
  rotate_y_->setValue(new_y);
}