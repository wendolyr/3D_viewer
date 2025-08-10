#ifndef CPP4_3DVIEWER_V2_0_VIEW_MAINWIDGET_H
#define CPP4_3DVIEWER_V2_0_VIEW_MAINWIDGET_H

#include <QDoubleSpinBox>
#include <QLabel>
#include <QScrollArea>
#include <QVector3D>
#include <QWidget>

#include "CyclicDoubleSpinBox.h"
#include "OpenGLWidget.h"

class MainWidget : public QWidget {
 public:
  MainWidget(QWidget* parent = nullptr);

 public slots:
  // Слот для загрузки модели
  void LoadModel();

 protected:
  // Обработчик изменения размеров окна
  void resizeEvent(QResizeEvent* event) override;

 private:
  enum class TransformType { Move, Rotate, Scale };
  enum class Axis { X, Y, Z, None };
  OpenGLWidget* gl_widget_;

  QString full_file_name_;

  QDoubleSpinBox* move_x_;
  QDoubleSpinBox* move_y_;
  QDoubleSpinBox* move_z_;
  CyclicDoubleSpinBox* rotate_x_;
  CyclicDoubleSpinBox* rotate_y_;
  CyclicDoubleSpinBox* rotate_z_;
  QDoubleSpinBox* scale_;

  QLabel* file_name_label_;
  QLabel* vertex_count_label_;
  QLabel* edge_count_label_;

  // Для цвета ребер
  QSpinBox* edge_r_color;
  QSpinBox* edge_g_color;
  QSpinBox* edge_b_color;
  QLabel* edge_color_preview_;

  // Для цвета вершин
  QSpinBox* vertex_r_color_;
  QSpinBox* vertex_g_color_;
  QSpinBox* vertex_b_color_;
  QLabel* vertex_color_preview_;

  // Для цвета фона
  QSpinBox* background_color_r_;
  QSpinBox* background_color_g_;
  QSpinBox* background_color_b_;
  QLabel* background_color_preview_;

  QVector<QVector3D> vertices_;            // Вершины
  QSet<QPair<unsigned, unsigned>> edges_;  // Ребра

  // Создание пользовательского интерфейса
  void SetupUI();
  // Создание виджета управления для оси
  QWidget* CreateAxisWidgetsMoveAndScale(QDoubleSpinBox*& spin_box,
                                         TransformType type, Axis axis);
  QWidget* CreateAxisWidgetsRotate(CyclicDoubleSpinBox*& spin_box, Axis axis);
  // Получение шага для изменения модели
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

 private slots:
  // Сброс преобразований
  void ResetTransform();
  // TODO Метод для применения преобразований к модели
  void OnTransformChanged();
};

#endif  // CPP4_3DVIEWER_V2_0_VIEW_MAINWIDGET_H