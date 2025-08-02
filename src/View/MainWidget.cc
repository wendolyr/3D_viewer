#include "MainWidget.h"
#include "OpenGLWidget.h"
#include "CyclicDoubleSpinBox.h"

#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QFormLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QResizeEvent>
#include <QMessageBox>
#include <QElapsedTimer>

// public
MainWidget::MainWidget(QWidget *parent) : QWidget(parent), m_full_file_name("") {
    MainWidget::SetupUI();
    m_gl_widget->clearModel();
}

// public slots
void MainWidget::LoadModel() {
    // Открываем диалог выбора файла
    QString file_name = QFileDialog::getOpenFileName(
        this,
        tr("Выберите файл модели"),
        QDir::homePath(),
        tr("OBJ Files (*.obj);;All Files (*)")
    );

    if (!file_name.isEmpty()) {
        // Извлекаем только имя файла для отображения
        QFileInfo file_info(file_name);
        m_full_file_name = file_info.fileName();

        if (!MainWidget::LoadModelData(file_name)) { 
            MainWidget::UpdateFileNameLabel();
            MainWidget::ResetTransform();
        }
    }
}

// protected
void MainWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    // При изменении размера пересчитываем образанное имя
    MainWidget::UpdateFileNameLabel();
}

// private
void MainWidget::SetupUI() {
    // Главная горизонтальная разметка
    QHBoxLayout* main_layout = new QHBoxLayout(this);
    main_layout->setContentsMargins(10, 10, 10, 10);
    main_layout->setSpacing(15);

    // Область для OpenGL (пока заглушка)
    // QWidget* gl_widget = new QWidget;
    m_gl_widget = new OpenGLWidget;
    m_gl_widget->setMinimumSize(640, 480);
    main_layout->addWidget(m_gl_widget, 7); // 70% ширины

    // Боковая панель управления
    QWidget* sidebar = new QWidget;
    QVBoxLayout* sidebar_layout = new QVBoxLayout(sidebar);
    sidebar_layout->setAlignment(Qt::AlignTop); // уточнить
    sidebar_layout->setContentsMargins(5, 5, 5, 5);
    sidebar_layout->setSpacing(10);

    // Группа загрузки модели
    QGroupBox* load_group = new QGroupBox("Загрузка модели");
    QVBoxLayout* load_layout = new QVBoxLayout;
    QPushButton* load_btn = new QPushButton("Загрузить OBJ файл");
    m_file_name_label = new QLabel("Файл не выбран");
    m_file_name_label->setFrameStyle(QFrame::Panel | QFrame::Sunken); // уточнить
    m_file_name_label->setStyleSheet("padding: 3px; background-color: #F0F0F0; color: #0d0c0c;");
    m_file_name_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred); // уточнить

    load_layout->addWidget(m_file_name_label);
    load_layout->addWidget(load_btn);

    connect(load_btn, &QPushButton::clicked, this, &MainWidget::MainWidget::LoadModel);

    load_group->setLayout(load_layout);

    // Группа информации о модели
    QGroupBox* info_group = new QGroupBox("Информация о модели");
    QFormLayout* info_layout = new QFormLayout;
    m_vertex_count_label = new QLabel("0");
    m_edge_count_label = new QLabel("0");
    info_layout->addRow("Вершин:", m_vertex_count_label);
    info_layout->addRow("Ребер:", m_edge_count_label);
    info_group->setLayout(info_layout);

    // Группа перемещения
    QGroupBox* move_group = new QGroupBox("Перемещение");
    QFormLayout* move_layout = new QFormLayout;
    move_layout->addRow("По X:", MainWidget::CreateAxisWidgetsMoveAndScale(m_move_x, TransformType::Move, Axis::X));
    move_layout->addRow("По Y:", MainWidget::CreateAxisWidgetsMoveAndScale(m_move_y, TransformType::Move, Axis::Y));
    move_layout->addRow("По Z:", MainWidget::CreateAxisWidgetsMoveAndScale(m_move_z, TransformType::Move, Axis::Z));
    move_group->setLayout(move_layout);

    // Группа поворота
    QGroupBox* rotate_group = new QGroupBox("Поворот");
    QFormLayout* rotate_layout = new QFormLayout;
    rotate_layout->addRow("По X (°):", MainWidget::CreateAxisWidgetsRotate(m_rotate_x, Axis::X));
    rotate_layout->addRow("По Y (°):", MainWidget::CreateAxisWidgetsRotate(m_rotate_y, Axis::Y));
    rotate_layout->addRow("По Z (°):", MainWidget::CreateAxisWidgetsRotate(m_rotate_z, Axis::Z));
    rotate_group->setLayout(rotate_layout);

    // Группа масштабирования
    QGroupBox* scale_group = new QGroupBox("Масштабирование");
    QFormLayout* scale_layout = new QFormLayout;
    scale_layout->addRow("Коэффицент:", MainWidget::CreateAxisWidgetsMoveAndScale(m_scale, TransformType::Scale, Axis::None));
    scale_group->setLayout(scale_layout);

    // Группа сброса преобразований
    QGroupBox* reset_group = new QGroupBox("Сброс");
    QVBoxLayout* reset_layout = new QVBoxLayout;
    QPushButton* reset_btn = new QPushButton("Сбросить преобразования");
    reset_layout->addWidget(reset_btn);
    connect(reset_btn, &QPushButton::clicked, [this]() {
        MainWidget::ResetTransform();
    });
    reset_group->setLayout(reset_layout);

    // Сборка боковой панели
    sidebar_layout->addWidget(load_group);
    sidebar_layout->addWidget(info_group);
    sidebar_layout->addWidget(move_group);
    sidebar_layout->addWidget(rotate_group);
    sidebar_layout->addWidget(scale_group);
    sidebar_layout->addWidget(reset_group);
    sidebar_layout->addStretch(1); // Растягивающий элемент

    // Добавление боковой панели в главную разметку (30% ширины)
    main_layout->addWidget(sidebar, 3);

    connect(m_move_x, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this, &MainWidget::OnTransformChanged);
    connect(m_move_y, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this, &MainWidget::OnTransformChanged);
    connect(m_move_z, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this, &MainWidget::OnTransformChanged);
    connect(m_rotate_x, QOverload<double>::of(&CyclicDoubleSpinBox::valueChanged),
        this, &MainWidget::OnTransformChanged);
    connect(m_rotate_y, QOverload<double>::of(&CyclicDoubleSpinBox::valueChanged),
        this, &MainWidget::OnTransformChanged);
    connect(m_rotate_z, QOverload<double>::of(&CyclicDoubleSpinBox::valueChanged),
        this, &MainWidget::OnTransformChanged);
    connect(m_scale, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this, &MainWidget::OnTransformChanged);
}

QWidget* MainWidget::CreateAxisWidgetsMoveAndScale(
    QDoubleSpinBox*& spin_box,
    TransformType type,
    Axis axis
) {
    QWidget* container = new QWidget;
    QHBoxLayout* h_layout = new QHBoxLayout(container);
    h_layout->setContentsMargins(0, 0, 0, 0); // Убираем отступы

    // Кнопка уменьшения значения
    QPushButton* btn_minus = new QPushButton("-");
    // Кнопка увеличения значения
    QPushButton* btn_plus = new QPushButton("+");
    // Фиксируем ширину кнопок
    btn_minus->setFixedWidth(25);
    btn_plus->setFixedWidth(25);
    spin_box = new QDoubleSpinBox;
    spin_box->setDecimals(2);

    // Настройка диапозонов в зависимости от типа преобразования
    if (type == TransformType::Move) {
        spin_box->setRange(-1000000.0, 1000000.0);
        spin_box->setValue(0.0);
    } else {
        spin_box->setRange(0.01, 1000.0);
        spin_box->setValue(1.0);
    }
    spin_box->setSingleStep(MainWidget::GetStepValue(type));
    spin_box->setButtonSymbols(QAbstractSpinBox::NoButtons); // Скрываем кнопки

    // Компоновка элементов
    h_layout->addWidget(btn_minus);
    h_layout->addWidget(spin_box);
    h_layout->addWidget(btn_plus);

    // Обработчик кнопок +/-
    connect(btn_minus, &QPushButton::clicked, [this, spin_box, type]() {
        spin_box->setValue(DoStep(spin_box->value(), false, type));
    });
    connect(btn_plus, &QPushButton::clicked, [this, spin_box, type]() {
        spin_box->setValue(DoStep(spin_box->value(), true, type));
    });

    return container;
}

QWidget* MainWidget::CreateAxisWidgetsRotate(
    CyclicDoubleSpinBox*& spin_box,
    Axis axis
) {
    QWidget* container = new QWidget;
    QHBoxLayout* h_layout = new QHBoxLayout(container);
    h_layout->setContentsMargins(0, 0, 0, 0); // Убираем отступы

    // Кнопка уменьшения значения
    QPushButton* btn_minus = new QPushButton("-");
    // Кнопка увеличения значения
    QPushButton* btn_plus = new QPushButton("+");
    // Фиксируем ширину кнопок
    btn_minus->setFixedWidth(25);
    btn_plus->setFixedWidth(25);
    spin_box = new CyclicDoubleSpinBox;
    spin_box->setDecimals(0);

    // Настройка диапозонов в зависимости от типа преобразования
    spin_box->setRange(0.0, 359.0); // Диапазон [0, 359]
    spin_box->setValue(0.0);
    spin_box->setDecimals(0);
    spin_box->setSingleStep(MainWidget::GetStepValue(TransformType::Rotate));
    spin_box->setButtonSymbols(QAbstractSpinBox::NoButtons); // Скрываем кнопки

    // Компоновка элементов
    h_layout->addWidget(btn_minus);
    h_layout->addWidget(spin_box);
    h_layout->addWidget(btn_plus);

    // Обработчик кнопок +/-
    connect(btn_minus, &QPushButton::clicked, [this, spin_box]() {
        spin_box->setValue(DoStep(spin_box->value(), false, TransformType::Rotate));
    });
    connect(btn_plus, &QPushButton::clicked, [this, spin_box]() {
        spin_box->setValue(DoStep(spin_box->value(), true, TransformType::Rotate));
    });

    return container;
}

double MainWidget::GetStepValue(TransformType type) const {
    switch (type) {
    case TransformType::Move: return 0.25;
    case TransformType::Rotate: return 1.0;
    case TransformType::Scale: return 0.05;
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
    QFontMetrics metrics(m_file_name_label->font());
    int max_width = m_file_name_label->width() - 10; // Учет отступов
    
    // Сокращение текста с многоточием при необходимости
    QString short_name = metrics.elidedText(m_full_file_name, Qt::ElideRight, max_width);

    m_file_name_label->setText(short_name);
    m_file_name_label->setToolTip(m_full_file_name); // Полное имя в подсказке
    
    // Обработка случая отсутствия файла
    if (m_full_file_name.isEmpty()) {
        m_file_name_label->setText("Файл не выбран");
        m_file_name_label->setToolTip("");
    }
}

int MainWidget::LoadModelData(const QString& file_path) {
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

    vertices.clear();
    edges.clear();

    // Чтение всего файла в память
    const QByteArray fileData = file.readAll();
    // fileData += '\0';
    file.close();
    // Таймер для замера производительности
    QElapsedTimer timer;
    timer.start();
    const char* data = fileData.constData();
    const char* end = data + fileData.size();
    const char* ptr = data;
    // Предварительное выделение памяти
    vertices.reserve(fileData.size() / 80); // Эвристическое значение
    edges.reserve(fileData.size() / 40);

    static std::vector<unsigned> face;
    face.clear();
    face.reserve(16);
    // Основной цикл парсинга
    while (ptr < end) {
        // Пропуск пробелов и управляющих символов
        while (ptr < end && (*ptr == ' ' || *ptr == '\t' || *ptr == '\r' || *ptr == '\n')) {
            ++ptr;
        }
        if (ptr >= end) break;
        // Обработка вершин (строки начинающиеся с "v ")
        if (*ptr == 'v' && (ptr + 1 < end) && (*(ptr + 1) == ' ' || *(ptr + 1) == '\t')) {
            if (ptr + 2 >= end) break;
            const char* line_start = ptr + 2;
            float x, y, z;
            char* next; // Указатель на следующую позицию
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
            vertices.append(QVector3D(x, y, z));
            ptr = SkipToNextLine(ptr, end);
            continue;
        }
        // New Обработка граней (f)
        else if (*ptr == 'f' && (ptr + 1 < end) && (*(ptr + 1) == ' ' || *(ptr + 1) == '\t')) {
            if (ptr + 2 >= end) break;
            
            const char* line_start = ptr + 2;
            face.clear();
            
            // Парсинг каждого значения в строке грани
            const char* token_start = line_start;
            // while (token_start < end && *token_start != '\n' && *token_start != '\r') {
            while (token_start < end) {
                // Пропуск пробелов в начале токена
                while (token_start < end && (*token_start == ' ' || *token_start == '\t')) {
                    ++token_start;
                }
                if (token_start >= end || *token_start == '\n' || *token_start == '\r') break;
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
                        vertex_index = static_cast<unsigned>(vertices.size() - idx);
                    } else {
                        vertex_index = static_cast<unsigned>(idx - 1);
                    }
                    
                    // Проверка и добавление индекса
                    if (vertex_index < static_cast<unsigned>(vertices.size())) {
                        face.push_back(vertex_index);
                    }
                }

                while (token_start < end && *token_start != ' ' && 
                       *token_start != '\t' && *token_start != '\n' && *token_start != '\r') {
                    ++token_start;
                }


                // old variant
                // const char* token_end = token_start;
                // // Поиск конца токена (пробел, табуляция или конец строки)
                // while (token_end < end && *token_end != ' ' && *token_end != '\t' && *token_end != '\n' && *token_end != '\r') {
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
                    
                    edges.insert(edge);
                    prev_index = current_index;
                }


                // old variant
                // for (int i = 0; i < face.size(); i++) {
                //     // Получаем две соседние вершины
                //     unsigned idx1 = face[i];
                //     unsigned idx2 = face[(i + 1) % face.size()];

                //     // Создаем упорядоченную пару (min, max) для избежания дубликатов
                //     QPair<unsigned,unsigned> edge = (idx1 < idx2) ? qMakePair(idx1, idx2) : qMakePair(idx2, idx1);
                //     // Добавляем ребро в множество (автоматическое удаление дубликатов)
                //     edges.insert(edge);
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
    // qDebug() << "Оптимизированный парсинг выполнен за:" << elapsed_ms << "мс | Вершин:" << vertices.size();
    qDebug() << "Парсинг выполнен за:" << elapsed_ms << "мс | Вершин:" << vertices.size() << "| Ребер:" << edges.size();
    // Обновление UI
    // m_vertex_count_label->setText(QString("Парсинг: %1 мс | Вершин: %2").arg(elapsed_ms).arg(vertices.size()));
    m_vertex_count_label->setText(QString("Парсинг: %1 мс | Вершин: %2")
                                  .arg(elapsed_ms, 0, 'f', 3)
                                  .arg(vertices.size()));
    m_edge_count_label->setText(QString("%1").arg(edges.size()));

    QVector<QPair<unsigned, unsigned>> edgeVec;
    edgeVec.reserve(edges.size());
    for (const auto& edge : edges) {
        edgeVec.append(edge);
    }

    // Передаем данные в OpenGLWidget
    m_gl_widget->setModelData(vertices, edgeVec);

    return 0;
}

// Пропуск до следующей строки
const char* MainWidget::SkipToNextLine(const char* ptr, const char* end) {
    while (ptr < end && *ptr != '\n') {
        ++ptr;
    }
    return ptr;
}

// private slots
void MainWidget::ResetTransform() {
    m_move_x->setValue(0.0);
    m_move_y->setValue(0.0);
    m_move_z->setValue(0.0);

    m_rotate_x->setValue(0.0);
    m_rotate_y->setValue(0.0);
    m_rotate_z->setValue(0.0);

    m_scale->setValue(1.0);

    OnTransformChanged();
}

void MainWidget::OnTransformChanged() {
    QVector3D translation(
        m_move_x->value(),
        m_move_y->value(),
        m_move_z->value()
    );
    
    QVector3D rotation(
        m_rotate_x->value(),
        m_rotate_y->value(),
        m_rotate_z->value()
    );
    
    float scale = m_scale->value();
    
    m_gl_widget->setTransformations(translation, rotation, scale);
}