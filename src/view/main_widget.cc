#include "main_widget.h"

#include <QApplication>
#include <QComboBox>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QFileInfo>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QRadioButton>
#include <QResizeEvent>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

#include "../controller/facade.h"
#include "builder/template_axis_control_builder.h"
#include "builder/transform_control_builder.h"
#include "builder/visual_settings_builder.h"
#include "cyclic_double_spin_box.h"
#include "opengl_widget.h"

namespace s21 {
MainWidget::MainWidget(QWidget* parent) : QWidget(parent), full_file_name_("") {
  MainWidget::SetupUI();
  ViewParams params;
  if (control_.LoadLastState(params)) {
    edge_r_color_->setValue(params.edge_color.x);
    edge_g_color_->setValue(params.edge_color.y);
    edge_b_color_->setValue(params.edge_color.z);
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

    gl_widget_->SetProjectionType(
        static_cast<OpenGLWidget::ProjectionType>(params.projection_type));
    parallel_btn_->setChecked(params.projection_type ==
                              static_cast<int>(OpenGLWidget::kParallel));
    central_btn_->setChecked(params.projection_type ==
                             static_cast<int>(OpenGLWidget::kCentral));

    // если базовое имя файла поменялось, значит файл с моделью существовал и
    // был корректным
    if (params.file_name != ".obj" && control_.GetVertices().size()) {
      full_file_name_ = QString::fromStdString(params.file_name);
      vertex_count_label_->setText(
          QString::number(control_.GetVertices().size()));
      edge_count_label_->setText(QString::number(control_.GetEdges().size()));
      Params affine = control_.GetCurrentSettings();
      move_x_->setValue(affine.shift.x);
      move_y_->setValue(affine.shift.y);
      move_z_->setValue(affine.shift.z);
      rotate_z_->setValue(affine.rotation.z);
      rotate_y_->setValue(affine.rotation.y);
      rotate_x_->setValue(affine.rotation.x);
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
  ViewParams params;
  params.projection_type = static_cast<int>(gl_widget_->GetProjectionType());
  params.edge_color.x = edge_r_color_->value();
  params.edge_color.y = edge_g_color_->value();
  params.edge_color.z = edge_b_color_->value();
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
    FileError error = control_.ParseFile(file_name.toStdString());
    if (error == FileError::kOk) {
      full_file_name_ = file_info.fileName();

      MainWidget::UpdateFileNameLabel();
      std::vector<Vertex> vertices = control_.GetVertices();
      std::unordered_set<std::pair<unsigned, unsigned>, PairHash> edges =
          control_.GetEdges();
      vertex_count_label_->setText(QString::number(vertices.size()));
      edge_count_label_->setText(QString::number(edges.size()));
      gl_widget_->SetModelData(vertices, edges);
      MainWidget::ResetTransform();
    } else if (error == FileError::kNotExist) {
      QMessageBox::warning(this, "Ошибка открытия",
                           "Файл не существует!\n"
                           "Проверьте путь: " +
                               file_name);
    } else if (error == FileError::kInvalidFile) {
      QMessageBox::warning(this, "Ошибка чтения",
                           "Некорректный файл!\n"
                           "В файле должны быть координаты вершин!",
                           QMessageBox::Ok);
    }
  }
}

// protected
void MainWidget::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);
  // При изменении размера пересчитываем образанное имя
  MainWidget::UpdateFileNameLabel();
}

// private
void MainWidget::SetupUI() {
  CreateMainLayout();
  CreateSidebar();
  CreateConnections();
}

double MainWidget::GetStepValue(TransformType type) const {
  switch (type) {
    case TransformType::kMove:
      return 0.25;
    case TransformType::kRotate:
      return 1.0;
    case TransformType::kScale:
      return 0.05;
    default:
      return 0;
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
  if (type == TransformType::kRotate) {
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

void MainWidget::UpdateEdgeSettings() {
  OpenGLWidget::EdgeType type =
      static_cast<OpenGLWidget::EdgeType>(edge_type_combo_->currentIndex());
  QVector3D color(edge_r_color_->value() / 255.0f,
                  edge_g_color_->value() / 255.0f,
                  edge_b_color_->value() / 255.0f);
  gl_widget_->SetEdgeSettings(type, color, edge_thickness_->value());

  // Обновление превью цвета
  edge_color_preview_->setStyleSheet(
      QString("background-color: rgb(%1,%2,%3); border: 1px solid gray;")
          .arg(edge_r_color_->value())
          .arg(edge_g_color_->value())
          .arg(edge_b_color_->value()));
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

  control_.ResetSettings();

  OnTransformChanged();
}

void MainWidget::ResetDisplay() {
  central_btn_->setChecked(true);
  // Установка отображения ребер: цвет RGB; толщина; тип отображения
  edge_r_color_->setValue(204);
  edge_g_color_->setValue(204);
  edge_b_color_->setValue(255);
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
  qDebug() << "Here";
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

  gl_widget_->SetTransformations(matrix);
}

// tmp разобрать потом и кровью

QGroupBox* MainWidget::CreateLoadGroup() {
  QGroupBox* group = new QGroupBox("Загрузка модели");
  QVBoxLayout* layout = new QVBoxLayout(group);

  file_name_label_ = new QLabel("Файл не выбран");
  file_name_label_->setFrameStyle(static_cast<int>(QFrame::Panel) |
                                  static_cast<int>(QFrame::Sunken));
  file_name_label_->setStyleSheet(
      "padding: 3px; background-color: #F0F0F0; color: #0d0c0c;");
  file_name_label_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);

  QPushButton* load_btn = new QPushButton("Загрузить OBJ файл");
  connect(load_btn, &QPushButton::clicked, this, &MainWidget::LoadModel);

  layout->addWidget(file_name_label_);
  layout->addWidget(load_btn);

  return group;
}

QGroupBox* MainWidget::CreateInfoGroup() {
  QGroupBox* group = new QGroupBox("Информация о модели");
  QFormLayout* layout = new QFormLayout(group);

  vertex_count_label_ = new QLabel("0");
  edge_count_label_ = new QLabel("0");

  layout->addRow("Вершин:", vertex_count_label_);
  layout->addRow("Ребер:", edge_count_label_);

  return group;
}

QGroupBox* MainWidget::CreateRecordGroup() {
  QGroupBox* group = new QGroupBox("Запись");
  QHBoxLayout* layout = new QHBoxLayout(group);
  QPushButton* gif_btn = new QPushButton("GIF");
  QPushButton* save_bmp_btn = new QPushButton("BMP");
  QPushButton* save_jpeg_btn = new QPushButton("JPEG");
  connect(gif_btn, &QPushButton::clicked, this, &MainWidget::RecordGif);
  connect(&timer, &QTimer::timeout, this, &MainWidget::CaptureFrame);
  connect(save_bmp_btn, &QPushButton::clicked, this, &MainWidget::SaveBMP);
  connect(save_jpeg_btn, &QPushButton::clicked, this, &MainWidget::SaveJPEG);
  layout->addWidget(gif_btn);
  layout->addWidget(save_bmp_btn);
  layout->addWidget(save_jpeg_btn);
  return group;
}

QGroupBox* MainWidget::CreateProjectionGroup() {
  QGroupBox* group = new QGroupBox("Проекция");
  QVBoxLayout* layout = new QVBoxLayout(group);
  parallel_btn_ = new QRadioButton("Параллельная");
  central_btn_ = new QRadioButton("Центральная");
  central_btn_->setChecked(true);
  connect(parallel_btn_, &QRadioButton::toggled, [this](bool checked) {
    if (checked) gl_widget_->SetProjectionType(OpenGLWidget::kParallel);
  });

  connect(central_btn_, &QRadioButton::toggled, [this](bool checked) {
    if (checked) gl_widget_->SetProjectionType(OpenGLWidget::kCentral);
  });
  layout->addWidget(parallel_btn_);
  layout->addWidget(central_btn_);
  return group;
}

QGroupBox* MainWidget::CreateResetGroup() {
  QGroupBox* group = new QGroupBox("Сброс");
  QHBoxLayout* layout = new QHBoxLayout;
  QPushButton* reset_model_btn = new QPushButton("Сброс\nпреобразования");
  QPushButton* reset_view_btn = new QPushButton("Сброс\nотображения");
  QFontMetrics font_metrics(reset_model_btn->font());
  int min_height = font_metrics.lineSpacing() * 2;

  auto SetupButton = [min_height](QPushButton* button) {
    button->setMinimumHeight(min_height);
    button->setStyleSheet("text-align: center;");
    button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  };

  SetupButton(reset_model_btn);
  SetupButton(reset_view_btn);

  connect(reset_model_btn, &QPushButton::clicked, this,
          &MainWidget::ResetTransform);
  connect(reset_view_btn, &QPushButton::clicked, this,
          &MainWidget::ResetDisplay);

  layout->addWidget(reset_model_btn);
  layout->addWidget(reset_view_btn);

  group->setLayout(layout);

  return group;
}

void MainWidget::OnWheelScrolled(int delta) {
  // Определяем направление прокрутки (1 - вверх, -1 - вниз)
  int direction = (delta > 0) ? 1 : -1;

  // Рассчитываем новый масштаб
  double step = GetStepValue(TransformType::kScale);
  double new_scale =
      scale_->value() + direction * step * 0.5;  // Медленное изменение

  // Устанавливаем границы масштабирования
  if (new_scale < 0.01) new_scale = 0.01;
  if (new_scale > 100.0) new_scale = 100.0;

  scale_->setValue(new_scale);
}

void MainWidget::HandleRotationDelta(float dx, float dy) {
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

void MainWidget::HandleTranslationDelta(float dx, float dy, float dz) {
  // Обновляем значения в спинбоксах
  move_x_->setValue(move_x_->value() + dx);
  move_y_->setValue(move_y_->value() + dy);

  // Если есть смещение по Z
  if (fabs(dz) > std::numeric_limits<float>::epsilon()) {
    move_z_->setValue(move_z_->value() + dz);
  }
}

// RECORDING
void MainWidget::RecordGif() {
  if (is_recording_) return;
  qDebug("Начало записи GIF");
  is_recording_ = true;
  gif_frames_.clear();
  frame_counter_ = 0;
  timer.start(100);  // 10 fps
}

void MainWidget::StopRecording() {
  timer.stop();
  SaveGif();
  is_recording_ = false;
}

void MainWidget::CaptureFrame() {
  if (frame_counter_ >= 50) {  // 5 sec * 10 fps
    StopRecording();
    return;
  }

  QImage frame = gl_widget_->grabFramebuffer();
  gif_frames_.append(
      frame.scaled(640, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation));

  frame_counter_++;
}

void MainWidget::SaveGif() {
  QString file_name = QFileDialog::getSaveFileName(
      this, "Сохранить как GIF", QCoreApplication::applicationDirPath(),
      "GIF Images (*.gif)");

  QDir temp_dir = QCoreApplication::applicationDirPath() + "/frames";
  if (!temp_dir.exists()) {
    temp_dir.mkpath(".");
  }
  for (int i = 0; i < 50; i++) {
    QString framePath = temp_dir.filePath(QString("frame_%1.png").arg(i));
    gif_frames_[i].save(framePath);
  }

  QProcess ffmpeg;
  ffmpeg.start("ffmpeg", {"-y", "-framerate", "10", "-i",
                          temp_dir.filePath("frame_%d.png"), "-vf",
                          "scale=640:480", file_name});

  if (ffmpeg.waitForFinished()) {
    qDebug("Конец записи GIF");
  }
  temp_dir.removeRecursively();
}

void MainWidget::SaveBMP() {
  QString file_name = QFileDialog::getSaveFileName(
      this, "Сохранить как BMP", QCoreApplication::applicationDirPath(),
      "BMP Images (*.bmp)");
  gl_widget_->grabFramebuffer().save(file_name);
}

void MainWidget::SaveJPEG() {
  QString file_name = QFileDialog::getSaveFileName(
      this, "Сохранить как JPEG", QCoreApplication::applicationDirPath(),
      "JPEG Images (*.jpeg)");
  gl_widget_->grabFramebuffer().save(file_name);
}

QScrollArea* MainWidget::CreateScrollArea() {
  QScrollArea* scroll_area = new QScrollArea;
  scroll_area->setWidgetResizable(true);
  scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  QWidget* scroll_content = new QWidget;
  QVBoxLayout* scroll_layout = new QVBoxLayout(scroll_content);
  scroll_layout->setAlignment(Qt::AlignTop);

  scroll_layout->addWidget(CreateMoveGroup());
  scroll_layout->addWidget(CreateRotateGroup());
  scroll_layout->addWidget(CreateScaleGroup());
  scroll_layout->addWidget(CreateProjectionGroup());
  scroll_layout->addWidget(CreateEdgeSettingsGroup());
  scroll_layout->addWidget(CreateVertexSettingsGroup());
  scroll_layout->addWidget(CreateBackgroundSettingsGroup());
  scroll_area->setWidget(scroll_content);
  scroll_content->setMinimumWidth(340);

  return scroll_area;
}

void MainWidget::CreateMainLayout() {
  QHBoxLayout* main_layout = new QHBoxLayout(this);
  main_layout->setContentsMargins(10, 10, 10, 10);
  main_layout->setSpacing(15);

  gl_widget_ = new OpenGLWidget;
  gl_widget_->setMinimumSize(640, 480);
  main_layout->addWidget(gl_widget_, 7);  // 70% ширины
}

void MainWidget::CreateSidebar() {
  // Боковая панель управления
  QWidget* sidebar = new QWidget;
  QVBoxLayout* sidebar_layout = new QVBoxLayout(sidebar);
  sidebar_layout->setAlignment(Qt::AlignTop);
  sidebar_layout->setContentsMargins(5, 5, 5, 5);
  sidebar_layout->setSpacing(10);

  sidebar_layout->addWidget(CreateLoadGroup());
  sidebar_layout->addWidget(CreateInfoGroup());
  sidebar_layout->addWidget(CreateRecordGroup());

  QScrollArea* scroll_area = CreateScrollArea();
  sidebar_layout->addWidget(scroll_area);
  sidebar_layout->addWidget(CreateResetGroup());
  sidebar->setMinimumWidth(360);
  qobject_cast<QHBoxLayout*>(layout())->addWidget(sidebar, 3);
}

QGroupBox* MainWidget::CreateMoveGroup() {
  return TransformBuilder<QDoubleSpinBox>(TransformType::kMove)
      .AddAxis(
          Axis::kX, GetStepValue(TransformType::kMove), move_x_,
          [this]() { ChangeValue(move_x_, TransformType::kMove, false); },
          [this]() {
            move_x_->setValue(
                DoStep(move_x_->value(), true, TransformType::kMove));
          })
      .AddAxis(
          Axis::kY, GetStepValue(TransformType::kMove), move_y_,
          [this]() {
            move_y_->setValue(
                DoStep(move_y_->value(), false, TransformType::kMove));
          },
          [this]() {
            move_y_->setValue(
                DoStep(move_y_->value(), true, TransformType::kMove));
          })
      .AddAxis(
          Axis::kZ, GetStepValue(TransformType::kMove), move_z_,
          [this]() {
            move_z_->setValue(
                DoStep(move_z_->value(), false, TransformType::kMove));
          },
          [this]() {
            move_z_->setValue(
                DoStep(move_z_->value(), true, TransformType::kMove));
          })
      .Build();
}
QGroupBox* MainWidget::CreateRotateGroup() {
  return TransformBuilder<CyclicDoubleSpinBox>(TransformType::kRotate)
      .AddAxis(
          Axis::kX, GetStepValue(TransformType::kRotate), rotate_x_,
          [this]() {
            rotate_x_->setValue(
                DoStep(rotate_x_->value(), false, TransformType::kRotate));
          },
          [this]() {
            rotate_x_->setValue(
                DoStep(rotate_x_->value(), true, TransformType::kRotate));
          })
      .AddAxis(
          Axis::kY, GetStepValue(TransformType::kRotate), rotate_y_,
          [this]() {
            rotate_y_->setValue(
                DoStep(rotate_y_->value(), false, TransformType::kRotate));
          },
          [this]() {
            rotate_y_->setValue(
                DoStep(rotate_y_->value(), true, TransformType::kRotate));
          })
      .AddAxis(
          Axis::kZ, GetStepValue(TransformType::kRotate), rotate_z_,
          [this]() {
            rotate_z_->setValue(
                DoStep(rotate_z_->value(), false, TransformType::kRotate));
          },
          [this]() {
            rotate_z_->setValue(
                DoStep(rotate_z_->value(), true, TransformType::kRotate));
          })
      .Build();
}
QGroupBox* MainWidget::CreateScaleGroup() {
  return TransformBuilder<QDoubleSpinBox>(TransformType::kScale)
      .AddAxis(
          Axis::kNone, GetStepValue(TransformType::kScale), scale_,
          [this]() {
            scale_->setValue(
                DoStep(scale_->value(), false, TransformType::kScale));
          },
          [this]() {
            scale_->setValue(
                DoStep(scale_->value(), true, TransformType::kScale));
          })
      .Build();
}
QGroupBox* MainWidget::CreateEdgeSettingsGroup() {
  return VisualSettingsBuilder()
      .AddComboBox("Тип линии:", edge_type_combo_, {"Сплошная", "Пунктирная"})
      .AddColorWidget("Цвет:", edge_r_color_, edge_g_color_, edge_b_color_,
                      edge_color_preview_, Qt::white)
      .AddDoubleSpinBox("Толщина:", edge_thickness_, 0.1, 10.0, 0.1, 1)
      .Build("Настройки ребер");
}
QGroupBox* MainWidget::CreateVertexSettingsGroup() {
  return VisualSettingsBuilder()
      .AddComboBox("Отображение", vertex_display_combo_,
                   {"Отсутствует", "Круг", "Квадрат"})
      .AddColorWidget("Цвет:", vertex_r_color_, vertex_g_color_,
                      vertex_b_color_, vertex_color_preview_, Qt::red)
      .AddDoubleSpinBox("Размер:", vertex_size_, 0.1, 25.0, 0.1, 1)
      .Build("Настройки вершин");
}
QGroupBox* MainWidget::CreateBackgroundSettingsGroup() {
  return VisualSettingsBuilder()
      .AddColorWidget("Цвет фона:", background_color_r_, background_color_g_,
                      background_color_b_, background_color_preview_, Qt::black)
      .Build("Настройки фона");
}

void MainWidget::CreateConnections() {
  auto ConnectTransformSignal = [this](auto widget) {
    connect(widget, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &MainWidget::OnTransformChanged);
  };
  // Подключение виджетов трансформации
  ConnectTransformSignal(move_x_);
  ConnectTransformSignal(move_y_);
  ConnectTransformSignal(move_z_);
  ConnectTransformSignal(rotate_x_);
  ConnectTransformSignal(rotate_y_);
  ConnectTransformSignal(rotate_z_);
  ConnectTransformSignal(scale_);
  // Общая функция для подключения цветовых компонентов
  auto ConnectColorGroup = [](const std::vector<QSpinBox*>& color_boxes,
                              auto update_func) {
    for (auto* box : color_boxes) {
      connect(box, QOverload<int>::of(&QSpinBox::valueChanged), update_func);
    }
  };
  // Настройки ребер
  auto UpdateEdge = [this] { UpdateEdgeSettings(); };
  connect(edge_type_combo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
          UpdateEdge);
  connect(edge_thickness_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          UpdateEdge);
  ConnectColorGroup({edge_r_color_, edge_g_color_, edge_b_color_}, UpdateEdge);
  // Настройки вершин
  auto UpdateVertex = [this] { UpdateVertexSettings(); };
  connect(vertex_display_combo_,
          QOverload<int>::of(&QComboBox::currentIndexChanged), UpdateVertex);
  connect(vertex_size_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          UpdateVertex);
  ConnectColorGroup({vertex_r_color_, vertex_g_color_, vertex_b_color_},
                    UpdateVertex);
  // Настройки фона
  auto UpdateBackground = [this] { MainWidget::UpdateBackground(); };
  ConnectColorGroup(
      {background_color_r_, background_color_g_, background_color_b_},
      UpdateBackground);
  // Управление мышью
  connect(gl_widget_, &OpenGLWidget::WheelScrolled, this,
          &MainWidget::OnWheelScrolled);
  connect(gl_widget_, &OpenGLWidget::RotationDeltaChanged, this,
          &MainWidget::HandleRotationDelta);
  connect(gl_widget_, &OpenGLWidget::TranslationDeltaChanged, this,
          &MainWidget::HandleTranslationDelta);
}
}  // namespace s21