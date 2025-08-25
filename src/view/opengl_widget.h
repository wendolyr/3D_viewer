#ifndef CPP4_3DVIEWER_V2_0_VIEW_OPENGL_WIDGET_H
#define CPP4_3DVIEWER_V2_0_VIEW_OPENGL_WIDGET_H

#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QWheelEvent>
#include <unordered_set>

#include "../controller/facade.h"
#include "shader_manager.h"

namespace s21 {
/**
 * @class OpenGLWidget
 * @brief Виджет для отображения 3D-моделей с использованием OpenGL
 * @details Обеспечивает рендеринг 3D-моделей, управление камерой и настройки
 * отображения
 */
class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
 public:
  /**
   * @enum ProjectionType
   * @brief Типы проекций для отображения 3D-модели
   */
  enum ProjectionType {
    kCentral,  ///< Центральная проекция (перспективная)
    kParallel  ///< Параллельная проекция (ортографическая)
  };

  /**
   * @enum EdgeType
   * @brief Типы отображения ребер модели
   */
  enum EdgeType {
    kSolid,  ///< Сплошные линии
    kDashed  ///< Пунктирные линии
  };

  /**
   * @enum VertexDisplay
   * @brief Способы отображения вершин модели
   */
  enum VertexDisplay {
    kNone,    ///< Вершины не отображаются
    kCircle,  ///< Вершины отображаются как круги
    kSquare  ///< Вершины отображаются как квадраты
  };

  /**
   * @brief Конструктор OpenGL виджета
   * @param parent Родительский виджет
   */
  explicit OpenGLWidget(QWidget* parent = nullptr);
  /**
   * @brief Деструктор OpenGL виджета
   */
  ~OpenGLWidget();
  /**
   * @brief Устанавливает данные модели для отображения
   * @param vertices Вектор вершин модели
   * @param edges Набор ребер модели
   */
  void SetModelData(
      const std::vector<Vertex>& vertices,
      const std::unordered_set<std::pair<unsigned, unsigned>, PairHash>& edges);
  /**
   * @brief Устанавливает преобразования для модели через матрицу
   * @param matrix Матрица преобразования 4x4
   */
  void SetTransformations(std::vector<std::vector<float>>& matrix);
  /**
   * @brief Устанавливает тип проекции
   * @param type Тип проекции (Central или Parallel)
   */
  void SetProjectionType(ProjectionType type);
  /**
   * @brief Устанавливает настройки отображения ребер
   * @param type Тип линий (Solid или Dashed)
   * @param color Цвет ребер
   * @param thickness Толщина линий
   * @param dash_size Размер штриха (для пунктирных линий)
   * @param gap_size Размер промежутка (для пунктирных линий)
   */
  void SetEdgeSettings(EdgeType type, const QVector3D& color, float thickness,
                       float dash_size = 10.0f, float gap_size = 5.0f);
  /**
   * @brief Устанавливает настройки отображения вершин
   * @param display Способ отображения вершин
   * @param color Цвет вершин
   * @param size Размер вершин
   */
  void SetVertexSettings(VertexDisplay display, const QVector3D& color,
                         float size);
  /**
   * @brief Устанавливает цвет фона
   * @param color Цвет фона
   */
  void SetBackgroundColor(const QVector3D& color);
  /**
   * @brief Возвращает текущий тип проекции
   * @return Текущий тип проекции
   */
  ProjectionType GetProjectionType() const;
 signals:
  /**
   * @brief Сигнал о прокрутке колесика мыши
   * @param delta Значение прокрутки
   */
  void WheelScrolled(int delta);
  /**
   * @brief Сигнал об изменении вращения
   * @param dx Изменение по оси X
   * @param dy Изменение по оси Y
   */
  void RotationDeltaChanged(float dx, float dy);
  /**
   * @brief Сигнал об изменении перемещения
   * @param dx Изменение по оси X
   * @param dy Изменение по оси Y
   * @param dz Изменение по оси Z (опционально)
   */
  void TranslationDeltaChanged(float dx, float dy, float dz = 0);

 protected:
  /**
   * @brief Обрабатывает события колесика мыши
   * @param event Событие колесика мыши
   */
  void wheelEvent(QWheelEvent* event) override;
  /**
   * @brief Обрабатывает нажатия кнопок мыши
   * @param event Событие нажатия кнопки мыши
   */
  void mousePressEvent(QMouseEvent* event) override;
  /**
   * @brief Обрабатывает перемещение мыши
   * @param event Событие перемещения мыши
   */
  void mouseMoveEvent(QMouseEvent* event) override;
  /**
   * @brief Обрабатывает отпускание кнопок мыши
   * @param event Событие отпускания кнопки мыши
   */
  void mouseReleaseEvent(QMouseEvent* event) override;
  /**
   * @brief Инициализирует OpenGL контекст
   */
  void initializeGL() override;
  /**
   * @brief Обрабатывает изменение размера виджета
   * @param w Новая ширина
   * @param h Новая высота
   */
  void resizeGL(int w, int h) override;
  /**
   * @brief Выполняет отрисовку OpenGL сцены
   */
  void paintGL() override;

 private:
  QMatrix4x4 model_;  ///< Матрица модели
  std::unique_ptr<ShaderManager> shader_manager_;  ///< Менеджер шейдеров
  QOpenGLBuffer vbo_;             ///< Вершинный буфер
  QOpenGLBuffer ibo_;             ///< Индексный буфер
  QOpenGLVertexArrayObject vao_;  ///< Vertex Array Object
  QMatrix4x4 projection_;         ///< Матрица проекции
  QVector3D translation_;         ///< Вектор перемещения
  QVector3D rotation_;            ///< Вектор вращения
  float scale_;  ///< Коэффициент масштабирования
  int index_count_ = 0;             ///< Количество индексов
  int vertex_count_ = 0;            ///< Количество вершин
  ProjectionType projection_type_;  ///< Тип проекции
  EdgeType edge_type_;    ///< Тип отображения ребер
  QVector3D edge_color_;  ///< Цвет ребер
  float edge_thickness_;  ///< Толщина ребер
  float dash_size_ = 10.0f;  ///< Размер штриха (для пунктира)
  float gap_size_ = 5.0f;  ///< Размер промежутка (для пунктира)
  VertexDisplay vertex_display_;  ///< Способ отображения вершин
  QVector3D vertex_color_;      ///< Цвет вершин
  float vertex_size_;           ///< Размер вершин
  QVector3D background_color_;  ///< Цвет фона
  QPoint last_mouse_pos_;       ///< Последняя позиция мыши
  bool is_rotating_ = false;    ///< Флаг вращения
  float rotation_sensitivity_ = 0.3f;  ///< Чувствительность вращения
  bool is_panning_ = false;  ///< Флаг перемещения
};
}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_VIEW_OPENGL_WIDGET_H