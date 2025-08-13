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

#include "Builder/TemplateAxisControlBuilder.h"
#include "Builder/TransformControlBuilder.h"
#include "CyclicDoubleSpinBox.h"
#include "OpenGLWidget.h"
// public
MainWidget::MainWidget(QWidget* parent) : QWidget(parent), full_file_name_("") {
  MainWidget::SetupUI();
  // TODO Добавить загрузку настроек иначе загрузка базового профиля
  //   if (есть настройки) {
  //     LoadModel();
  //     SetTransformSettings()  // типо метод для загрузки параметров модели,
  //     пока
  //                             // не существует
  //   } else {
  MainWidget::ResetDisplay();
  //   }
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

    if (!MainWidget::LoadModelData(file_name)) {
      MainWidget::UpdateFileNameLabel();
      MainWidget::ResetTransform();
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
}

// QWidget* MainWidget::CreateAxisWidgetsMoveAndScale(QDoubleSpinBox*& spin_box,
//                                                    TransformType type,
//                                                    Axis axis) {
//   QWidget* container = new QWidget;
//   QHBoxLayout* h_layout = new QHBoxLayout(container);
//   h_layout->setContentsMargins(0, 0, 0, 0);  // Убираем отступы

//   // Кнопка уменьшения значения
//   QPushButton* btn_minus = new QPushButton("-");
//   // Кнопка увеличения значения
//   QPushButton* btn_plus = new QPushButton("+");
//   // Фиксируем ширину кнопок
//   btn_minus->setFixedWidth(25);
//   btn_plus->setFixedWidth(25);
//   spin_box = new QDoubleSpinBox;
//   spin_box->setDecimals(2);

//   // Настройка диапозонов в зависимости от типа преобразования
//   if (type == TransformType::Move) {
//     spin_box->setRange(-1000000.0, 1000000.0);
//     spin_box->setValue(0.0);
//   } else {
//     spin_box->setRange(0.01, 1000.0);
//     spin_box->setValue(1.0);
//   }
//   spin_box->setSingleStep(MainWidget::GetStepValue(type));
//   spin_box->setButtonSymbols(QAbstractSpinBox::NoButtons);  // Скрываем
//   кнопки

//   // Компоновка элементов
//   h_layout->addWidget(btn_minus);
//   h_layout->addWidget(spin_box);
//   h_layout->addWidget(btn_plus);

//   // Обработчик кнопок +/-
//   connect(btn_minus, &QPushButton::clicked, [this, spin_box, type]() {
//     spin_box->setValue(DoStep(spin_box->value(), false, type));
//   });
//   connect(btn_plus, &QPushButton::clicked, [this, spin_box, type]() {
//     spin_box->setValue(DoStep(spin_box->value(), true, type));
//   });

//   return container;
// }

// QWidget* MainWidget::CreateAxisWidgetsRotate(CyclicDoubleSpinBox*& spin_box,
//                                              Axis axis) {
//   QWidget* container = new QWidget;
//   QHBoxLayout* h_layout = new QHBoxLayout(container);
//   h_layout->setContentsMargins(0, 0, 0, 0);  // Убираем отступы

//   // Кнопка уменьшения значения
//   QPushButton* btn_minus = new QPushButton("-");
//   // Кнопка увеличения значения
//   QPushButton* btn_plus = new QPushButton("+");
//   // Фиксируем ширину кнопок
//   btn_minus->setFixedWidth(25);
//   btn_plus->setFixedWidth(25);
//   spin_box = new CyclicDoubleSpinBox;
//   spin_box->setDecimals(0);

//   // Настройка диапозонов в зависимости от типа преобразования
//   spin_box->setRange(0.0, 359.0);  // Диапазон [0, 359]
//   spin_box->setValue(0.0);
//   spin_box->setDecimals(0);
//   spin_box->setSingleStep(MainWidget::GetStepValue(TransformType::Rotate));
//   spin_box->setButtonSymbols(QAbstractSpinBox::NoButtons);  // Скрываем
//   кнопки

//   // Компоновка элементов
//   h_layout->addWidget(btn_minus);
//   h_layout->addWidget(spin_box);
//   h_layout->addWidget(btn_plus);

//   // Обработчик кнопок +/-
//   connect(btn_minus, &QPushButton::clicked, [this, spin_box]() {
//     spin_box->setValue(DoStep(spin_box->value(), false,
//     TransformType::Rotate));
//   });
//   connect(btn_plus, &QPushButton::clicked, [this, spin_box]() {
//     spin_box->setValue(DoStep(spin_box->value(), true,
//     TransformType::Rotate));
//   });

//   return container;
// }

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

int MainWidget::LoadModelData(const QString& file_path) {
  std::setlocale(LC_NUMERIC, "C");
  QFile file(file_path);
  // Проверка существования файла
  if (!file.exists()) {
    QMessageBox::warning(this, "Ошибка", "Файл не существует");
    return 1;
  }
  // Попытка открытия файла
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::warning(this, "Ошибка", "Не удается открыть файл");
    return 1;
  }

  vertices_.clear();
  edges_.clear();

  // Чтение всего файла в память
  const QByteArray file_data = file.readAll();
  file.close();
  // Таймер для замера производительности
  QElapsedTimer timer;
  timer.start();
  const char* data = file_data.constData();
  const char* end = data + file_data.size();
  const char* ptr = data;

  static std::vector<unsigned> face;
  face.clear();
  face.reserve(16);
  // Основной цикл парсинга
  while (ptr < end) {
    // Пропуск пробелов и управляющих символов
    while (ptr < end &&
           (*ptr == ' ' || *ptr == '\t' || *ptr == '\r' || *ptr == '\n')) {
      ++ptr;
    }
    if (ptr >= end) break;
    // Обработка вершин (строки начинающиеся с "v ")
    if (*ptr == 'v' && (ptr + 1 < end) &&
        (*(ptr + 1) == ' ' || *(ptr + 1) == '\t')) {
      if (ptr + 2 >= end) break;
      const char* line_start = ptr + 2;
      float x, y, z;
      char* next;  // Указатель на следующую позицию
      // Парсинг координаты X
      const char* before_x = line_start;
      x = std::strtof(line_start, &next);
      // if (next == before_x) {
      //     QMessageBox::warning(this, "Ошибка", "Некорректный файл");
      //     return 1;
      //     // ptr = SkipToNextLine(ptr, end);
      //     // continue;
      // }
      // // Парсинг координаты Y
      // const char* before_y = next;
      y = std::strtof(next, &next);
      // if (next == before_y) {
      //     QMessageBox::warning(this, "Ошибка", "Некорректный файл");
      //     return 1;
      //     // ptr = SkipToNextLine(ptr, end);
      //     // continue;
      // }
      // // Парсинг координаты Z
      // const char* before_z = next;
      z = std::strtof(next, &next);
      // if (next == before_z) {
      //     QMessageBox::warning(this, "Ошибка", "Некорректный файл");
      //     return 1;
      //     // ptr = SkipToNextLine(ptr, end);
      //     // continue;
      // }
      if (next == line_start) {
        QMessageBox::warning(this, "Ошибка", "Некорректный файл");
        return 1;
      }
      // Добавление вершины
      vertices_.append(QVector3D(x, y, z));
      ptr = SkipToNextLine(ptr, end);
      continue;
    }
    // New Обработка граней (f)
    else if (*ptr == 'f' && (ptr + 1 < end) &&
             (*(ptr + 1) == ' ' || *(ptr + 1) == '\t')) {
      if (ptr + 2 >= end) break;

      const char* line_start = ptr + 2;
      face.clear();

      // Парсинг каждого значения в строке грани
      const char* token_start = line_start;
      // while (token_start < end && *token_start != '\n' && *token_start !=
      // '\r') {
      while (token_start < end) {
        // Пропуск пробелов в начале токена
        while (token_start < end &&
               (*token_start == ' ' || *token_start == '\t')) {
          ++token_start;
        }
        if (token_start >= end || *token_start == '\n' || *token_start == '\r')
          break;
        // new variant
        int idx = 0;
        bool negative = false;
        const char* num_start = token_start;

        // Обработка знака
        if (*num_start == '-') {
          negative = true;
          ++num_start;
        } else if (*num_start == '+') {
          ++num_start;
        }

        // Парсинг числа
        if (num_start < end && *num_start >= '0' && *num_start <= '9') {
          while (num_start < end && *num_start >= '0' && *num_start <= '9') {
            idx = idx * 10 + (*num_start++ - '0');
          }

          // Преобразование индекса
          unsigned vertex_index;
          if (negative) {
            vertex_index = static_cast<unsigned>(vertices_.size() - idx);
          } else {
            vertex_index = static_cast<unsigned>(idx - 1);
          }

          // Проверка и добавление индекса
          if (vertex_index < static_cast<unsigned>(vertices_.size())) {
            face.push_back(vertex_index);
          }
        }

        while (token_start < end && *token_start != ' ' &&
               *token_start != '\t' && *token_start != '\n' &&
               *token_start != '\r') {
          ++token_start;
        }

        // old variant
        // const char* token_end = token_start;
        // // Поиск конца токена (пробел, табуляция или конец строки)
        // while (token_end < end && *token_end != ' ' && *token_end != '\t' &&
        // *token_end != '\n' && *token_end != '\r') {
        //     token_end++;
        // }
        // // Преобразование строки в число
        // char* next_char;
        // int idx = std::strtol(token_start, &next_char, 10);

        // // Проверка корректности преобразования
        // if (next_char != token_start) {
        //     // OBJ использует 1-индексацию, преобразуем в 0-индексацию
        //     unsigned vertex_index = static_cast<unsigned>(std::abs(idx)) - 1;

        //     // Проверка валидности индекса
        //     if (vertex_index < static_cast<unsigned>(vertices.size())) {
        //         face.append(vertex_index);
        //     }
        // }

        // // Переход к следующему токену
        // token_start = token_end;
      }

      // Сохраняем грань если есть хотя бы 3 вершины
      const size_t face_size = face.size();
      // if (face.size() >= 2) {
      if (face_size >= 2) {
        // Для каждой вершины в грани
        // new variant
        const unsigned int last_index = face[face_size - 1];
        unsigned int prev_index = last_index;

        for (size_t i = 0; i < face_size; ++i) {
          const unsigned int current_index = face[i];

          // Создание упорядоченной пары
          const auto edge = (prev_index < current_index)
                                ? qMakePair(prev_index, current_index)
                                : qMakePair(current_index, prev_index);

          edges_.insert(edge);
          prev_index = current_index;
        }

        // old variant
        // for (int i = 0; i < face.size(); i++) {
        //     // Получаем две соседние вершины
        //     unsigned idx1 = face[i];
        //     unsigned idx2 = face[(i + 1) % face.size()];

        //     // Создаем упорядоченную пару (min, max) для избежания дубликатов
        //     QPair<unsigned,unsigned> edge = (idx1 < idx2) ? qMakePair(idx1,
        //     idx2) : qMakePair(idx2, idx1);
        //     // Добавляем ребро в множество (автоматическое удаление
        //     дубликатов) edges.insert(edge);
        // }
      }

      ptr = SkipToNextLine(ptr, end);
      continue;
    }
    // Пропуск остальных строк
    else {
      ptr = SkipToNextLine(ptr, end);
    }
    // Переход к следующему символу
    if (ptr < end) ++ptr;
  }
  // Замер времени выполнения
  qint64 elapsed_ns = timer.nsecsElapsed();
  double elapsed_ms = elapsed_ns / 1000000.0;
  // Вывод информации в консоль
  // qDebug() << "Оптимизированный парсинг выполнен за:" << elapsed_ms << "мс |
  // Вершин:" << vertices.size();
  qDebug() << "Парсинг выполнен за:" << elapsed_ms
           << "мс | Вершин:" << vertices_.size() << "| Ребер:" << edges_.size();
  // Обновление UI
  // vertex_count_label_->setText(QString("Парсинг: %1 мс | Вершин:
  // %2").arg(elapsed_ms).arg(vertices.size()));
  vertex_count_label_->setText(QString("%1").arg(vertices_.size()));
  edge_count_label_->setText(QString("%1").arg(edges_.size()));

  QVector<QPair<unsigned, unsigned>> edge_vec;
  edge_vec.reserve(edges_.size());
  for (const auto& edge : edges_) {
    edge_vec.append(edge);
  }

  // Передаем данные в OpenGLWidget
  gl_widget_->SetModelData(vertices_, edge_vec);

  return 0;
}

// Пропуск до следующей строки
const char* MainWidget::SkipToNextLine(const char* ptr, const char* end) {
  while (ptr < end && *ptr != '\n') {
    ++ptr;
  }
  return ptr;
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
  QVector3D translation(move_x_->value(), move_y_->value(), move_z_->value());

  QVector3D rotation(rotate_x_->value(), rotate_y_->value(),
                     rotate_z_->value());

  float scale = scale_->value();

  gl_widget_->SetTransformations(translation, rotation, scale);
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