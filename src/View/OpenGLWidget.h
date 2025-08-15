#ifndef CPP4_3DVIEWER_V2_0_VIEW_OPENGLWIDGET_H
#define CPP4_3DVIEWER_V2_0_VIEW_OPENGLWIDGET_H

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

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
 public:
  enum ProjectionType { Central, Parallel };
  enum EdgeType { Solid, Dashed };
  enum VertexDisplay { None, Circle, Square };

  explicit OpenGLWidget(QWidget* parent = nullptr);
  ~OpenGLWidget();
  // void SetModelData(const QVector<QVector3D>& vertices,
  //                   const QVector<QPair<unsigned, unsigned>>& edges);
  void SetModelData(const std::vector<s21::Vertex>& vertices,
                    const std::unordered_set<std::pair<unsigned, unsigned>,
                                             s21::PairHash>& edges);
  void ClearModel();
  void SetTransformations(const QVector3D& translation,
                          const QVector3D& rotation, float scale);
  void NewSetTransformations(std::vector<std::vector<float>>& matrix);

  void SetProjectionType(ProjectionType type);
  void SetEdgeSettings(EdgeType type, const QVector3D& color, float thickness,
                       float dash_size = 10.0f, float gap_size = 5.0f);
  void SetVertexSettings(VertexDisplay display, const QVector3D& color,
                         float size);
  void SetBackgroundColor(const QVector3D& color);

  ProjectionType GetProjectionType() const;
 signals:
  void wheelScrolled(int delta);
  void rotationDeltaChanged(float dx, float dy);
  void translationDeltaChanged(float dx, float dy, float dz = 0);

 protected:
  // Обработка действий мыши
  void wheelEvent(QWheelEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

 private:
  QMatrix4x4 model_;
  // Шейдерные программы и буфферы
  QOpenGLShaderProgram* line_program_;   // Шейдерная программа для линий
  QOpenGLShaderProgram* point_program_;  // Шейдерная программа для точек
  QOpenGLBuffer vbo_;
  QOpenGLBuffer ibo_;
  QOpenGLVertexArrayObject vao_;

  QMatrix4x4 projection_;
  QVector3D translation_;
  QVector3D rotation_;
  float scale_;

  int index_count_ = 0;
  int vertex_count_ = 0;

  ProjectionType projection_type_;  // Тип проекции объекта
  // Параметры отображения ребер
  EdgeType edge_type_;
  QVector3D edge_color_;
  float edge_thickness_;
  float dash_size_ = 10.0f;
  float gap_size_ = 5.0f;
  // Параметры отображения вершин
  VertexDisplay vertex_display_;
  QVector3D vertex_color_;
  float vertex_size_;

  QVector3D background_color_;  // Цвет заднего фона

  // параметры вращения мыши
  QPoint last_mouse_pos_;
  bool is_rotating_ = false;
  float rotation_sensitivity_ = 0.3f;

  bool is_panning_ = false;
};

#endif  // CPP4_3DVIEWER_V2_0_VIEW_OPENGLWIDGET_H