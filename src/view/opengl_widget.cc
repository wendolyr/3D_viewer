#include "opengl_widget.h"

#include <QDebug>

namespace s21 {
OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent),
      vbo_(QOpenGLBuffer::VertexBuffer),
      ibo_(QOpenGLBuffer::IndexBuffer),
      translation_(0.0f, 0.0f, 0.0f),
      rotation_(0.0f, 0.0f, 0.0f),
      scale_(1.0f) {
  shader_manager_ = std::make_unique<ShaderManager>();
  projection_type_ = kCentral;
  edge_type_ = kSolid;
  edge_color_ = QVector3D(0.8f, 0.8f, 1.0f);
  edge_thickness_ = 1.0f;

  vertex_display_ = kCircle;
  vertex_color_ = QVector3D(1.0f, 0.0f, 0.0f);
  vertex_size_ = 2.0f;

  background_color_ = QVector3D(0.0f, 0.0f, 0.0f);
}

OpenGLWidget::~OpenGLWidget() {
  makeCurrent();
  vbo_.destroy();
  ibo_.destroy();
  vao_.destroy();
  doneCurrent();
}

void OpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_DEPTH_TEST);

  vao_.create();
  vao_.bind();

  if (!shader_manager_->Initialize()) {
    qWarning() << "Failed to initialize shaders";
    return;
  }

  resizeGL(width(), height());
}

void OpenGLWidget::resizeGL(int w, int h) {
  projection_.setToIdentity();
  float aspect = static_cast<float>(w) / h;

  if (projection_type_ == kCentral) {
    projection_.perspective(45.0f, aspect, 0.1f, 10000.0f);
  } else {
    float viewSize = 5.0f;
    projection_.ortho(-viewSize * aspect, viewSize * aspect, -viewSize,
                      viewSize, 0.1f, 10000.0f);
  }
}

void OpenGLWidget::paintGL() {
  vao_.bind();
  vbo_.bind();
  if (ibo_.isCreated()) ibo_.bind();
  // Очистка заднего фона нужным цветом
  glClearColor(background_color_.x(), background_color_.y(),
               background_color_.z(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  QMatrix4x4 view;
  view.translate(0.0f, 0.0f, -15.0f);

  QMatrix4x4 mvp = projection_ * view * model_;

  if (index_count_ > 0) {
    auto line_program_ = shader_manager_->GetShader(ShaderManager::kLINE);
    if (line_program_) {
      line_program_->bind();

      // Передаем параметры
      line_program_->setUniformValue("u_mvp", mvp);
      line_program_->setUniformValue("u_resolution",
                                     QVector2D(width(), height()));
      line_program_->setUniformValue("u_thickness", edge_thickness_);
      line_program_->setUniformValue("u_color", edge_color_);
      line_program_->setUniformValue("use_dashing", edge_type_ == kDashed);
      line_program_->setUniformValue("u_dash_size", dash_size_);
      line_program_->setUniformValue("u_gap_size", gap_size_);

      // Рисуем линии
      glDrawElements(GL_LINES, index_count_, GL_UNSIGNED_INT, nullptr);

      line_program_->release();
    }
  }

  if (vertex_display_ != kNone && vertex_count_ > 0) {
    auto point_program_ = shader_manager_->GetShader(ShaderManager::kPOINT);
    if (point_program_) {
      point_program_->bind();

      // Передаем параметры
      point_program_->setUniformValue("u_mvp", mvp);
      point_program_->setUniformValue("u_vertex_size", vertex_size_);
      point_program_->setUniformValue("u_color", vertex_color_);
      point_program_->setUniformValue("u_vertex_display", vertex_display_);

      // Рисуем точки
      glDrawArrays(GL_POINTS, 0, vertex_count_);

      point_program_->release();
    }
  }

  vbo_.release();
  if (ibo_.isCreated()) ibo_.release();
  vao_.release();
}

void OpenGLWidget::SetModelData(
    const std::vector<Vertex>& vertices,
    const std::unordered_set<std::pair<unsigned, unsigned>, PairHash>& edges) {
  makeCurrent();
  vao_.bind();

  vbo_.destroy();
  ibo_.destroy();

  index_count_ = 0;
  vertex_count_ = vertices.size();

  vbo_.create();
  vbo_.bind();
  vbo_.allocate(vertices.data(), vertices.size() * sizeof(Vertex));

  QVector<GLuint> indices;
  for (const auto& edge : edges) {
    if (edge.first < static_cast<unsigned>(vertices.size()) &&
        edge.second < static_cast<unsigned>(vertices.size())) {
      indices.append(edge.first);
      indices.append(edge.second);
    }
  }
  index_count_ = indices.size();
  if (index_count_ > 0) {
    ibo_.create();
    ibo_.bind();
    ibo_.allocate(indices.constData(), indices.size() * sizeof(GLuint));
  }

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

  vbo_.release();
  if (ibo_.isCreated()) ibo_.release();
  vao_.release();

  doneCurrent();
  update();
}

void OpenGLWidget::SetTransformations(std::vector<std::vector<float>>& matrix) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      model_(i, j) = matrix[i][j];
    }
  }
  update();
}

void OpenGLWidget::SetProjectionType(ProjectionType type) {
  projection_type_ = type;
  resizeGL(width(), height());
  update();
}

auto OpenGLWidget::GetProjectionType() const -> ProjectionType {
  return projection_type_;
}

void OpenGLWidget::SetEdgeSettings(EdgeType type, const QVector3D& color,
                                   float thickness, float dash_size,
                                   float gap_size) {
  edge_type_ = type;
  edge_color_ = color;
  edge_thickness_ = thickness;
  dash_size_ = dash_size;
  gap_size_ = gap_size;
  update();
}

void OpenGLWidget::SetVertexSettings(VertexDisplay display,
                                     const QVector3D& color, float size) {
  vertex_display_ = display;
  vertex_color_ = color;
  vertex_size_ = size;
  update();
}
void OpenGLWidget::SetBackgroundColor(const QVector3D& color) {
  background_color_ = color;
  update();
}

void OpenGLWidget::wheelEvent(QWheelEvent* event) {
  emit WheelScrolled(event->angleDelta().y());
  event->accept();
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    last_mouse_pos_ = event->pos();
    is_rotating_ = true;
    setCursor(Qt::ClosedHandCursor);  // Изменяем курсор
    event->accept();
  } else if (event->button() == Qt::MiddleButton) {
    last_mouse_pos_ = event->pos();
    is_panning_ = true;
    setCursor(Qt::SizeAllCursor);  // Курсор перемещения
    event->accept();
  } else {
    event->ignore();
  }
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
  if (is_rotating_) {
    QPoint delta = event->pos() - last_mouse_pos_;
    last_mouse_pos_ = event->pos();

    // Рассчитываем углы вращения
    float delta_x = rotation_sensitivity_ * delta.y();  // Вращение вокруг X
    float delta_y = rotation_sensitivity_ * delta.x();  // Вращение вокруг Y

    // Генерируем сигнал с дельтой вращения
    emit RotationDeltaChanged(delta_x, delta_y);
    event->accept();
  } else if (is_panning_) {
    QPoint delta = event->pos() - last_mouse_pos_;
    last_mouse_pos_ = event->pos();

    float sensitivity = 0.01f;
    float dx = delta.x() * sensitivity;
    float dy = -delta.y() * sensitivity;  // Инвертируем ось Y

    // Если зажат Shift - двигаем по оси Z
    float dz = 0;
    if (event->modifiers() & Qt::ShiftModifier) {
      dz = dy;
      dy = 0;
    }

    emit TranslationDeltaChanged(dx, dy, dz);
    event->accept();
  } else {
    event->ignore();
  }
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    is_rotating_ = false;
    setCursor(Qt::ArrowCursor);  // Восстанавливаем курсор
    event->accept();
  } else if (event->button() == Qt::MiddleButton) {
    is_panning_ = false;
    setCursor(Qt::ArrowCursor);
    event->accept();
  } else {
    event->ignore();
  }
}
}  // namespace s21