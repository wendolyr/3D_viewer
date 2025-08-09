#include "OpenGLWidget.h"
#include <QDebug>

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      ibo(QOpenGLBuffer::IndexBuffer),
      m_indexCount(0),
      m_translation(0.0f, 0.0f, 0.0f),
      m_rotation(0.0f, 0.0f, 0.0f),
      m_scale(1.0f)
{
    m_projectionType = Central;
    m_edgeType = Solid;
    m_edgeColor = QVector3D(0.8f, 0.8f, 1.0f);
    m_edgeThickness = 1.0f;
    m_dashSize = 10.0f;
    m_gapSize = 5.0f;
    
    m_vertexDisplay = Circle;
    m_vertexColor = QVector3D(1.0f, 0.0f, 0.0f);
    m_vertexSize = 2.0f;
    
    m_bgColor = QVector3D(0.0f, 0.0f, 0.0f);
}

OpenGLWidget::~OpenGLWidget() {
    makeCurrent();
    vbo.destroy();
    ibo.destroy();
    vao.destroy();
    m_indexCount = 0;
    delete program;
    doneCurrent();
}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_PROGRAM_POINT_SIZE); // Для использования gl_PointSize из шейдера
    
    vao.create();
    vao.bind();
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // Удаляем старую программу
    delete program;
    
    // Создаем новую шейдерную программу
    program = new QOpenGLShaderProgram(this);
    
    // Вершинный шейдер
    program->addShaderFromSourceCode(QOpenGLShader::Vertex,
        "#version 410 core\n"
        "layout(location = 0) in vec3 a_position;\n"
        "flat out vec3 start_pos;\n"
        "out vec3 vert_pos;\n"
        "uniform mat4 u_mvp;\n"
        "uniform float u_vertex_size;\n"
        "void main() {\n"
        "    vec4 pos = u_mvp * vec4(a_position, 1.0);\n"
        "    vert_pos = pos.xyz / pos.w;\n"
        "    start_pos = vert_pos;\n"
        "    gl_PointSize = u_vertex_size;\n"
        "    gl_Position = pos;\n"
        "}");
    
    // Фрагментный шейдер
    program->addShaderFromSourceCode(QOpenGLShader::Fragment,
        "#version 410 core\n"
        "flat in vec3 start_pos;\n"
        "in vec3 vert_pos;\n"
        "uniform vec2 u_resolution;\n"
        "uniform vec3 u_color;\n"
        "uniform float u_dash_size;\n"
        "uniform float u_gap_size;\n"
        "uniform bool use_dashing;\n"
        "out vec4 fragColor;\n"
        "void main() {\n"
        "    if (use_dashing) {\n"
        "        vec2 dir = (vert_pos.xy - start_pos.xy) * u_resolution/2.0;\n"
        "        float dist = length(dir);\n"
        "        float cycle = u_dash_size + u_gap_size;\n"
        "        if (fract(dist / cycle) > u_dash_size/cycle) {\n"
        "            discard;\n"
        "        }\n"
        "    }\n"
        "    fragColor = vec4(u_color, 1.0);\n"
        "}");
    
    program->link();
    program->bind();

    vbo.create();
    ibo.create();

    m_indexCount = 0;
    m_vertexCount = 0;
    resizeGL(width(), height());
}
void OpenGLWidget::resizeGL(int w, int h) {
    projection.setToIdentity();
    float aspect = static_cast<float>(w) / h;
    
    if (m_projectionType == Central) {
        projection.perspective(45.0f, aspect, 0.1f, 100000.0f);
    } else {
        float viewSize = 5.0f;
        projection.ortho(-viewSize * aspect, 
                         viewSize * aspect,
                         -viewSize, 
                         viewSize,
                         0.1f, 100000.0f);
    }
}

void OpenGLWidget::paintGL() {
    vao.bind();
    glClearColor(m_bgColor.x(), m_bgColor.y(), m_bgColor.z(), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program->bind();

    QMatrix4x4 view;
    view.translate(0.0f, 0.0f, -15.0f);
    QMatrix4x4 model;
    model.translate(m_translation);
    model.rotate(m_rotation.x(), 1.0f, 0.0f, 0.0f);
    model.rotate(m_rotation.y(), 0.0f, 1.0f, 0.0f);
    model.rotate(m_rotation.z(), 0.0f, 0.0f, 1.0f);
    model.scale(m_scale);
    QMatrix4x4 mvp = projection * view * model;

    // Передаем параметры в шейдеры
    program->setUniformValue("u_mvp", mvp);
    program->setUniformValue("u_resolution", QVector2D(width(), height()));
    program->setUniformValue("u_vertex_size", m_vertexSize);

    if (m_vertexCount > 0) {
        vbo.bind();
        ibo.bind();
        program->enableAttributeArray(0);
        program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));

        // Отрисовка ребер
        program->setUniformValue("use_dashing", m_edgeType == Dashed);
        program->setUniformValue("u_dash_size", m_dashSize);
        program->setUniformValue("u_gap_size", m_gapSize);
        program->setUniformValue("u_color", m_edgeColor);
        glLineWidth(m_edgeThickness);
        glDrawElements(GL_LINES, m_indexCount, GL_UNSIGNED_INT, nullptr);

        // Отрисовка вершин
        if (m_vertexDisplay != None) {
            program->setUniformValue("use_dashing", false); // Отключаем пунктир
            program->setUniformValue("u_color", m_vertexColor);
            glDrawArrays(GL_POINTS, 0, m_vertexCount);
        }

        vbo.release();
        ibo.release();
    }

    program->release();
    vao.release();
}

void OpenGLWidget::setModelData(const QVector<QVector3D>& vertices, const QVector<QPair<unsigned, unsigned>>& edges) {
    makeCurrent();
    vao.bind();
    
    vbo.destroy();
    ibo.destroy();
    m_indexCount = 0;
    m_vertexCount = vertices.size();

    vbo.create();
    vbo.bind();
    vbo.allocate(vertices.constData(), vertices.size() * sizeof(QVector3D));
    
    QVector<GLuint> indices;
    indices.reserve(edges.size() * 2);
    for (const auto& edge : edges) {
        if (edge.first < static_cast<unsigned>(vertices.size()) && 
            edge.second < static_cast<unsigned>(vertices.size())) {
            indices.append(edge.first);
            indices.append(edge.second);
        }
    }

    ibo.create();
    ibo.bind();
    ibo.allocate(indices.constData(), indices.size() * sizeof(GLuint));
    ibo.release();
    
    m_indexCount = indices.size();
    
    program->bind();
    vbo.bind();
    program->enableAttributeArray(0);
    program->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    vbo.release();
    program->release();
    
    doneCurrent();
    update();
}

void OpenGLWidget::clearModel() {
    makeCurrent();
    vbo.destroy();
    ibo.destroy();
    m_indexCount = 0;
    m_vertexCount = 0;
    doneCurrent();
    update();
}

void OpenGLWidget::setTransformations(const QVector3D& translation, const QVector3D& rotation, float scale) {
    m_translation = translation;
    m_rotation = rotation;
    m_scale = scale;
    update();
}

void OpenGLWidget::setProjectionType(ProjectionType type) {
    m_projectionType = type;
    resizeGL(width(), height());
    update();
}

void OpenGLWidget::setEdgeSettings(EdgeType type, const QVector3D& color, 
                                  float thickness, float dash_size, float gap_size) {
    m_edgeType = type;
    m_edgeColor = color;
    m_edgeThickness = thickness;
    m_dashSize = dash_size;
    m_gapSize = gap_size;
    update();
}

void OpenGLWidget::setVertexSettings(VertexDisplay display, const QVector3D& color, float size) {
    m_vertexDisplay = display;
    m_vertexColor = color;
    m_vertexSize = size;
    update();
}
void OpenGLWidget::setBackgroundColor(const QVector3D& color) {
    m_bgColor = color;
    update();
}