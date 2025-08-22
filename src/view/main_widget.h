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
/**
 * @class MainWidget
 * @brief Главный виджет приложения 3D-просмотрщика
 * @details Содержит весь пользовательский интерфейс и управляет взаимодействием
 *          между пользователем и OpenGL виджетом
 */
class MainWidget : public QWidget {
 public:
  /**
   * @brief Конструктор главного виджета
   * @param parent Родительский виджет
   */
  MainWidget(QWidget* parent = nullptr);
  /**
   * @brief Деструктор главного виджета
   */
  ~MainWidget();

 public slots:
  /**
   * @brief Загружает модель из файла
   */
  void LoadModel();

 protected:
  /**
   * @brief Обрабатывает изменение размера окна
   * @param event Событие изменения размера
   */
  void resizeEvent(QResizeEvent* event) override;
  /**
   * @brief Обрабатывает событие показа виджета
   * @param event Событие показа
   */
  void showEvent(QShowEvent* event) override;

 private:
  Facade control_;  ///< Фасад для работы с контроллером
  OpenGLWidget* gl_widget_;  ///< OpenGL виджет для отображения модели
  QString full_file_name_;  ///< Полное имя файла модели
  QDoubleSpinBox *move_x_, *move_y_, *move_z_;  ///< Спинбоксы для перемещения
  CyclicDoubleSpinBox *rotate_x_, *rotate_y_,
      *rotate_z_;          ///< Спинбоксы для вращения
  QDoubleSpinBox* scale_;  ///< Спинбокс для масштабирования
  QLabel* file_name_label_;  ///< Метка с именем файла
  QLabel* vertex_count_label_;  ///< Метка с количеством вершин
  QLabel* edge_count_label_;  ///< Метка с количеством ребер
  QRadioButton *parallel_btn_, *central_btn_;  ///< Радиокнопки выбора проекции
  QSpinBox *edge_r_color_, *edge_g_color_,
      *edge_b_color_;           ///< Спинбоксы цвета ребер
  QLabel* edge_color_preview_;  ///< Превью цвета ребер
  QDoubleSpinBox* edge_thickness_;  ///< Толщина ребер
  QComboBox* edge_type_combo_;      ///< Комбобокс типа ребер
  QSpinBox *vertex_r_color_, *vertex_g_color_,
      *vertex_b_color_;  ///< Спинбоксы цвета вершин
  QLabel* vertex_color_preview_;  ///< Превью цвета вершин
  QDoubleSpinBox* vertex_size_;   ///< Размер вершин
  QComboBox* vertex_display_combo_;  ///< Комбобокс отображения вершин
  QSpinBox *background_color_r_, *background_color_g_,
      *background_color_b_;  ///< Спинбоксы цвета фона
  QLabel* background_color_preview_;  ///< Превью цвета фона
  bool is_recording_ = false;         ///< Флаг записи GIF
  QList<QImage> gif_frames_;          ///< Кадры для GIF
  QTimer timer;                       ///< Таймер для записи GIF
  int frame_counter_ = 0;             ///< Счетчик кадров

  /**
   * @brief Настраивает пользовательский интерфейс
   */
  void SetupUI();
  /**
   * @brief Создает группу загрузки модели
   * @return Указатель на созданную группу
   */
  QGroupBox* CreateLoadGroup();
  /**
   * @brief Создает группу информации о модели
   * @return Указатель на созданную группу
   */
  QGroupBox* CreateInfoGroup();
  /**
   * @brief Создает группу записи
   * @return Указатель на созданную группу
   */
  QGroupBox* CreateRecordGroup();
  /**
   * @brief Создает группу выбора проекции
   * @return Указатель на созданную группу
   */
  QGroupBox* CreateProjectionGroup();
  /**
   * @brief Создает группу сброса настроек
   * @return Указатель на созданную группу
   */
  QGroupBox* CreateResetGroup();
  /**
   * @brief Создает область прокрутки для настроек
   * @return Указатель на созданную область прокрутки
   */
  QScrollArea* CreateScrollArea();
  /**
   * @brief Создает основной layout
   */
  void CreateMainLayout();
  /**
   * @brief Создает боковую панель
   */
  void CreateSidebar();
  /**
   * @brief Создает группу управления перемещением
   * @return Указатель на созданную группу
   */
  QGroupBox* CreateMoveGroup();
  /**
   * @brief Создает группу управления вращением
   * @return Указатель на созданную группу
   */
  QGroupBox* CreateRotateGroup();
  /**
   * @brief Создает группу управления масштабированием
   * @return Указатель на созданную группу
   */
  QGroupBox* CreateScaleGroup();
  /**
   * @brief Создает группу настроек ребер
   * @return Указатель на созданную группу
   */
  QGroupBox* CreateEdgeSettingsGroup();
  /**
   * @brief Создает группу настроек вершин
   * @return Указатель на созданную группу
   */
  QGroupBox* CreateVertexSettingsGroup();
  /**
   * @brief Создает группу настроек фона
   * @return Указатель на созданную группу
   */
  QGroupBox* CreateBackgroundSettingsGroup();
  /**
   * @brief Создает соединения между сигналами и слотами
   */
  void CreateConnections();
  /**
   * @brief Возвращает значение шага для преобразования
   * @param type Тип преобразования
   * @return Значение шага
   */
  double GetStepValue(TransformType type) const;
  /**
   * @brief Выполняет шаг изменения значения
   * @param value Текущее значение
   * @param sum Направление изменения (true - увеличение, false - уменьшение)
   * @param type Тип преобразования
   * @return Новое значение
   */
  double DoStep(double value, bool sum, TransformType type);
  /**
   * @brief Обновляет отображение имени файла
   */
  void UpdateFileNameLabel();
  /**
   * @brief Обновляет настройки отображения ребер
   */
  void UpdateEdgeSettings();
  /**
   * @brief Обновляет настройки отображения вершин
   */
  void UpdateVertexSettings();
  /**
   * @brief Обновляет цвет фона
   */
  void UpdateBackground();
  /**
   * @brief Изменяет значение в спинбоксе
   * @param spinBox Указатель на спинбокс
   * @param type Тип преобразования
   * @param increase Направление изменения (true - увеличение, false -
   * уменьшение)
   */
  void ChangeValue(QDoubleSpinBox* spinBox, TransformType type, bool increase) {
    spinBox->setValue(DoStep(spinBox->value(), increase, type));
  }
  /**
   * @brief Сохраняет анимированный GIF
   */
  void SaveGif();

 private slots:
  /**
   * @brief Обрабатывает прокрутку колесика мыши
   * @param delta Значение прокрутки
   */
  void OnWheelScrolled(int delta);
  /**
   * @brief Обрабатывает изменение вращения
   * @param dx Изменение по оси X
   * @param dy Изменение по оси Y
   */
  void HandleRotationDelta(float dx, float dy);
  /**
   * @brief Обрабатывает изменение перемещения
   * @param dx Изменение по оси X
   * @param dy Изменение по оси Y
   * @param dz Изменение по оси Z
   */
  void HandleTranslationDelta(float dx, float dy, float dz);
  /**
   * @brief Сбрасывает преобразования модели
   */
  void ResetTransform();
  /**
   * @brief Сбрасывает настройки отображения
   */
  void ResetDisplay();
  /**
   * @brief Обрабатывает изменение преобразований
   */
  void OnTransformChanged();
  /**
   * @brief Начинает запись GIF
   */
  void RecordGif();
  /**
   * @brief Останавливает запись GIF
   */
  void StopRecording();
  /**
   * @brief Захватывает кадр для GIF
   */
  void CaptureFrame();
  /**
   * @brief Сохраняет изображение в формате JPEG
   */
  void SaveJPEG();
  /**
   * @brief Сохраняет изображение в формате BMP
   */
  void SaveBMP();
};
}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_VIEW_MAIN_WIDGET_H