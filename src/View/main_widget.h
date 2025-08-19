#ifndef CPP4_3DVIEWER_V2_0_VIEW_MAIN_WIDGET_H
#define CPP4_3DVIEWER_V2_0_VIEW_MAIN_WIDGET_H

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollArea>
#include <QTimer>
#include <QVector3D>
#include <QWidget>

#include "../controller/facade.h"
#include "cyclic_double_spin_box.h"
#include "enum.h"
#include "opengl_widget.h"

namespace s21 {
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
  Facade control_;
  OpenGLWidget* gl_widget_;

  QString full_file_name_;

  QDoubleSpinBox *move_x_, *move_y_, *move_z_;
  CyclicDoubleSpinBox *rotate_x_, *rotate_y_, *rotate_z_;
  QDoubleSpinBox* scale_;

  QLabel* file_name_label_;
  QLabel* vertex_count_label_;
  QLabel* edge_count_label_;

  QRadioButton *parallel_btn_, *central_btn_;

  // Для цвета ребер
  QSpinBox *edge_r_color_, *edge_g_color_, *edge_b_color_;
  QLabel* edge_color_preview_;
  QDoubleSpinBox* edge_thickness_;
  QComboBox* edge_type_combo_;

  // Для цвета вершин
  QSpinBox *vertex_r_color_, *vertex_g_color_, *vertex_b_color_;
  QLabel* vertex_color_preview_;
  QDoubleSpinBox* vertex_size_;
  QComboBox* vertex_display_combo_;

  // Для цвета фона
  QSpinBox *background_color_r_, *background_color_g_, *background_color_b_;
  QLabel* background_color_preview_;

  QVector<QVector3D> vertices_;            // Вершины
  QSet<QPair<unsigned, unsigned>> edges_;  // Ребра

  // Создание пользовательского интерфейса
  void SetupUI();
  // Создание групп интерфейса
  QGroupBox* CreateLoadGroup();
  QGroupBox* CreateInfoGroup();
  QGroupBox* CreateRecordGroup();
  QGroupBox* CreateProjectionGroup();
  QGroupBox* CreateResetGroup();
  QScrollArea* CreateScrollArea();
  void CreateMainLayout();
  void CreateSidebar();
  QGroupBox* CreateMoveGroup();
  QGroupBox* CreateRotateGroup();
  QGroupBox* CreateScaleGroup();
  QGroupBox* CreateEdgeSettingsGroup();
  QGroupBox* CreateVertexSettingsGroup();
  QGroupBox* CreateBackgroundSettingsGroup();
  void CreateConnections();
  // Получение шага для изменения модели
  double GetStepValue(TransformType type) const;
  // Выполнение шага изменения при нажатии
  double DoStep(double value, bool sum, TransformType type);
  // Обновление отображения имени файла
  void UpdateFileNameLabel();

  // Методы обновления настроек граней, вершин и заднего фона
  void UpdateEdgeSettings();
  void UpdateVertexSettings();
  void UpdateBackground();

  void ChangeValue(QDoubleSpinBox* spinBox, TransformType type, bool increase) {
    spinBox->setValue(DoStep(spinBox->value(), increase, type));
  }

  // RECORDING
  bool is_recording_ = false;
  QList<QImage> gif_frames_;
  QTimer timer;
  int frame_counter_ = 0;
  void SaveGif();

 private slots:
  void OnWheelScrolled(int delta);
  void HandleRotationDelta(float dx, float dy);
  void HandleTranslationDelta(float dx, float dy, float dz);
  // Сброс преобразований
  void ResetTransform();
  void ResetDisplay();
  // TODO Метод для применения преобразований к модели
  void OnTransformChanged();

  // RECORDING
  void RecordGif();
  void StopRecording();
  void CaptureFrame();
  void SaveJPEG();
  void SaveBMP();
};
} // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_VIEW_MAIN_WIDGET_H