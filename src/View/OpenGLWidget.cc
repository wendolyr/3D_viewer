#include "OpenGLWidget.h"

#include <QDebug>

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent),
      vbo_(QOpenGLBuffer::VertexBuffer),
      ibo_(QOpenGLBuffer::IndexBuffer),
      translation_(0.0f, 0.0f, 0.0f),
      rotation_(0.0f, 0.0f, 0.0f),
      scale_(1.0f),
      line_program_(nullptr),
      point_program_(nullptr) {
  projection_type_ = Central;
  edge_type_ = Solid;
  edge_color_ = QVector3D(0.8f, 0.8f, 1.0f);
  edge_thickness_ = 1.0f;

  vertex_display_ = Circle;
  vertex_color_ = QVector3D(1.0f, 0.0f, 0.0f);
  vertex_size_ = 2.0f;

  background_color_ = QVector3D(0.0f, 0.0f, 0.0f);
}

OpenGLWidget::~OpenGLWidget() {
  makeCurrent();
  vbo_.destroy();
  ibo_.destroy();
  vao_.destroy();
  delete line_program_;
  delete point_program_;
  doneCurrent();
}

void OpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_DEPTH_TEST);

  vao_.create();
  vao_.bind();

  // Программа для линий
  line_program_ = new QOpenGLShaderProgram(this);
  // Вершинный шейдер для линий
  line_program_->addShaderFromSourceCode(
      QOpenGLShader::Vertex,
      "#version 410 core\n"
      "layout(location = 0) in vec3 a_position;\n"
      "out vec3 v_position;\n"
      "uniform mat4 u_mvp;\n"
      "void main() {\n"
      "    v_position = a_position;\n"
      "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
      "}");
  // Геометрический шейдер для линий
  line_program_->addShaderFromSourceCode(
      QOpenGLShader::Geometry,
      "#version 410 core\n"
      "layout(lines) in;\n"
      "layout(triangle_strip, max_vertices = 4) out;\n"
      "in vec3 v_position[];\n"
      "out vec3 g_position;\n"
      "flat out vec3 start;\n"
      "flat out vec3 end;\n"
      "out vec2 screen_pos;\n"
      "flat out vec2 screen_start;\n"
      "flat out vec2 screen_end;\n"
      "uniform vec2 u_resolution;\n"
      "uniform float u_thickness;\n"
      "void main() {\n"
      "    vec4 p0 = gl_in[0].gl_Position;\n"
      "    vec4 p1 = gl_in[1].gl_Position;\n"
      "    screen_start = (p0.xy / p0.w) * u_resolution;\n"
      "    screen_end = (p1.xy / p1.w) * u_resolution;\n"
      "    vec2 screen0 = screen_start;\n"
      "    vec2 screen1 = screen_end;\n"
      "    vec2 dir = normalize(screen1 - screen0);\n"
      "    vec2 normal = vec2(-dir.y, dir.x);\n"
      "    normal /= u_resolution;\n"
      "    normal *= u_thickness;\n"
      "    start = v_position[0];\n"
      "    end = v_position[1];\n"
      "    gl_Position = p0 + vec4(normal * p0.w, 0, 0);\n"
      "    g_position = v_position[0];\n"
      "    screen_pos = (gl_Position.xy / gl_Position.w) * u_resolution;\n"
      "    EmitVertex();\n"
      "    gl_Position = p0 - vec4(normal * p0.w, 0, 0);\n"
      "    g_position = v_position[0];\n"
      "    screen_pos = (gl_Position.xy / gl_Position.w) * u_resolution;\n"
      "    EmitVertex();\n"
      "    gl_Position = p1 + vec4(normal * p1.w, 0, 0);\n"
      "    g_position = v_position[1];\n"
      "    screen_pos = (gl_Position.xy / gl_Position.w) * u_resolution;\n"
      "    EmitVertex();\n"
      "    gl_Position = p1 - vec4(normal * p1.w, 0, 0);\n"
      "    g_position = v_position[1];\n"
      "    screen_pos = (gl_Position.xy / gl_Position.w) * u_resolution;\n"
      "    EmitVertex();\n"
      "    EndPrimitive();\n"
      "}");
  // Фрагментный шейдер для линий
  line_program_->addShaderFromSourceCode(
      QOpenGLShader::Fragment,
      "#version 410 core\n"
      "in vec3 g_position;\n"
      "flat in vec3 start;\n"
      "flat in vec3 end;\n"
      "in vec2 screen_pos;\n"
      "flat in vec2 screen_start;\n"
      "flat in vec2 screen_end;\n"
      "uniform vec3 u_color;\n"
      "uniform float u_dash_size;\n"
      "uniform float u_gap_size;\n"
      "uniform bool use_dashing;\n"
      "out vec4 fragColor;\n"
      "void main() {\n"
      "    if (use_dashing) {\n"
      "        vec2 dir = normalize(screen_end - screen_start);\n"
      "        float pos = dot(screen_pos - screen_start, dir);\n"
      "        float cycle = u_dash_size + u_gap_size;\n"
      "        if (fract(pos / cycle) > (u_dash_size / cycle)) {\n"
      "            discard;\n"
      "        }\n"
      "    }\n"
      "    fragColor = vec4(u_color, 1.0);\n"
      "}");
  line_program_->link();

  // Программа для точек
  point_program_ = new QOpenGLShaderProgram(this);
  // Вершинный шейдер для точек
  point_program_->addShaderFromSourceCode(
      QOpenGLShader::Vertex,
      "#version 410 core\n"
      "layout(location = 0) in vec3 a_position;\n"
      "uniform mat4 u_mvp;\n"
      "uniform float u_vertex_size;\n"
      "void main() {\n"
      "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
      "    gl_PointSize = u_vertex_size;\n"
      "}");
  // Фрагментный шейдер для точек
  point_program_->addShaderFromSourceCode(
      QOpenGLShader::Fragment,
      "#version 410 core\n"
      "uniform vec3 u_color;\n"
      "uniform int u_vertex_display;\n"
      "out vec4 fragColor;\n"
      "void main() {\n"
      "    if (u_vertex_display == 1) {\n"
      "        vec2 coord = gl_PointCoord.xy - vec2(0.5);\n"
      "        if (dot(coord, coord) > 0.25) discard;\n"
      "    }\n"
      "    fragColor = vec4(u_color, 1.0);\n"
      "}");
  point_program_->link();

  resizeGL(width(), height());
}

void OpenGLWidget::resizeGL(int w, int h) {
  projection_.setToIdentity();
  float aspect = static_cast<float>(w) / h;

  if (projection_type_ == Central) {
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
  // Модель нужно вынести из view и все её преобразования в том-числе
  // QMatrix4x4 model;
  // model.translate(translation_);
  // model.rotate(rotation_.x(), 1.0f, 0.0f, 0.0f);
  // model.rotate(rotation_.y(), 0.0f, 1.0f, 0.0f);
  // model.rotate(rotation_.z(), 0.0f, 0.0f, 1.0f);
  // model.scale(scale_);

  QMatrix4x4 mvp = projection_ * view * model_;

  // ===== Отрисовка линий =====
  if (index_count_ > 0) {
    line_program_->bind();

    // Передаем параметры
    line_program_->setUniformValue("u_mvp", mvp);
    line_program_->setUniformValue("u_resolution",
                                   QVector2D(width(), height()));
    line_program_->setUniformValue("u_thickness", edge_thickness_);
    line_program_->setUniformValue("u_color", edge_color_);
    line_program_->setUniformValue("use_dashing", edge_type_ == Dashed);
    line_program_->setUniformValue("u_dash_size", dash_size_);
    line_program_->setUniformValue("u_gap_size", gap_size_);

    // Рисуем линии
    glDrawElements(GL_LINES, index_count_, GL_UNSIGNED_INT, nullptr);

    line_program_->release();
  }

  // ===== Отрисовка точек =====
  if (vertex_display_ != None && vertex_count_ > 0) {
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

  vbo_.release();
  if (ibo_.isCreated()) ibo_.release();
  vao_.release();
}

// void OpenGLWidget::SetModelData(
//     const QVector<QVector3D>& vertices,
//     const QVector<QPair<unsigned, unsigned>>& edges) {
void OpenGLWidget::SetModelData(
    const std::vector<s21::Vertex>& vertices,
    const std::unordered_set<std::pair<unsigned, unsigned>, s21::PairHash>&
        edges) {
  makeCurrent();
  vao_.bind();

  vbo_.destroy();
  ibo_.destroy();

  index_count_ = 0;
  vertex_count_ = vertices.size();

  vbo_.create();
  vbo_.bind();
  // vbo_.allocate(vertices.constData(), vertices.size() * sizeof(QVector3D));
  vbo_.allocate(vertices.data(), vertices.size() * sizeof(s21::Vertex));

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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(s21::Vertex), nullptr);

  vbo_.release();
  if (ibo_.isCreated()) ibo_.release();
  vao_.release();

  doneCurrent();
  update();
}

void OpenGLWidget::ClearModel() {
  makeCurrent();
  vbo_.destroy();
  ibo_.destroy();
  index_count_ = 0;
  vertex_count_ = 0;
  doneCurrent();
  update();
}

void OpenGLWidget::SetTransformations(const QVector3D& translation,
                                      const QVector3D& rotation, float scale) {
  translation_ = translation;
  rotation_ = rotation;
  scale_ = scale;
  update();
}

void OpenGLWidget::NewSetTransformations(
    std::vector<std::vector<float>>& matrix) {
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
  emit wheelScrolled(event->angleDelta().y());
  event->accept();
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    last_mouse_pos_ = event->pos();
    is_rotating_ = true;
    setCursor(Qt::ClosedHandCursor);  // Изменяем курсор
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
    emit rotationDeltaChanged(delta_x, delta_y);
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
  } else {
    event->ignore();
  }
}