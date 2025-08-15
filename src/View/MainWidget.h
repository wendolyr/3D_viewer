#ifndef CPP4_3DVIEWER_V2_0_VIEW_MAINWIDGET_H
#define CPP4_3DVIEWER_V2_0_VIEW_MAINWIDGET_H

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollArea>
#include <QVector3D>
#include <QWidget>

#include "../controller/facade.h"
#include "CyclicDoubleSpinBox.h"
#include "Enum.h"
#include "OpenGLWidget.h"

class MainWidget : public QWidget {
 public:
  MainWidget(QWidget* parent = nullptr);
  ~MainWidget();

 public slots:
  // Слот для загрузки модели
  void LoadModel();

 protected:
  // Обработчик изменения размеров окна
  void resizeEvent(QResizeEvent* event) override;
  void showEvent(QShowEvent* event) override;

 private:
  s21::Facade control_;
  OpenGLWidget* gl_widget_;

  QString full_file_name_;

  QDoubleSpinBox *move_x_, *move_y_, *move_z_;
  CyclicDoubleSpinBox *rotate_x_, *rotate_y_, *rotate_z_;
  QDoubleSpinBox* scale_;

  QLabel* file_name_label_;
  QLabel* vertex_count_label_;
  QLabel* edge_count_label_;

  // Для цвета ребер
  QSpinBox *edge_r_color, *edge_g_color, *edge_b_color;
  QLabel* edge_color_preview_;
  QDoubleSpinBox* edge_thickness_;  // Добавлено
  QComboBox* edge_type_combo_;      // Добавлено

  // Для цвета вершин
  QSpinBox *vertex_r_color_, *vertex_g_color_, *vertex_b_color_;
  QLabel* vertex_color_preview_;
  QDoubleSpinBox* vertex_size_;      // Добавлено
  QComboBox* vertex_display_combo_;  // Добавлено

  // Для цвета фона
  QSpinBox *background_color_r_, *background_color_g_, *background_color_b_;
  QLabel* background_color_preview_;

  QVector<QVector3D> vertices_;            // Вершины
  QSet<QPair<unsigned, unsigned>> edges_;  // Ребра

  // Создание пользовательского интерфейса
  void SetupUI();
  // Создание групп интерфейса
  QGroupBox* CreateLoadGroup(QPushButton*& load_btn, QLabel*& file_name_label);
  QGroupBox* CreateInfoGroup(QLabel*& vertex_count, QLabel*& edge_count);
  QGroupBox* CreateRecordGroup(QPushButton*& gif_btn, QPushButton*& screen_btn);
  QGroupBox* CreateProjectionGroup(QRadioButton*& parallel_btn,
                                   QRadioButton*& central_btn);
  // Создание виджета управления для оси
  // QWidget* CreateAxisWidgetsMoveAndScale(QDoubleSpinBox*& spin_box,
  //                                        TransformType type, Axis axis);
  // QWidget* CreateAxisWidgetsRotate(CyclicDoubleSpinBox*& spin_box, Axis
  // axis); Получение шага для изменения модели
  double GetStepValue(TransformType type) const;
  // Выполнение шага изменения при нажатии
  double DoStep(double value, bool sum, TransformType type);
  // Обновление отображения имени файла
  void UpdateFileNameLabel();
  // Открытие и парсер obj-файла с моделью
  int LoadModelData(const QString& file_path);
  // Пропуск не нужных строк в парсере
  inline const char* SkipToNextLine(const char* ptr, const char* end);

  // возможно стоит убрать
  QWidget* CreateColorWidget(QSpinBox*& r, QSpinBox*& g, QSpinBox*& b,
                             QLabel*& preview, const QString& labelText);
  // Методы обновления настроек граней, вершин и заднего фона
  void UpdateEdgeSettings();
  void UpdateVertexSettings();
  void UpdateBackground();

  void ChangeValue(QDoubleSpinBox* spinBox, TransformType type, bool increase) {
    spinBox->setValue(DoStep(spinBox->value(), increase, type));
  }

 private slots:
  void OnWheelScrolled(int delta);
  // Сброс преобразований
  void ResetTransform();
  void ResetDisplay();
  // TODO Метод для применения преобразований к модели
  void OnTransformChanged();
};

#endif  // CPP4_3DVIEWER_V2_0_VIEW_MAINWIDGET_H