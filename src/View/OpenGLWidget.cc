#include "OpenGLWidget.h"
#include <QDebug>

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      ibo(QOpenGLBuffer::IndexBuffer),
      m_indexCount(0),
      m_translation(0.0f, 0.0f, 0.0f),
      m_rotation(0.0f, 0.0f, 0.0f),
      m_scale(1.0f),
      m_lineProgram(nullptr),
      m_pointProgram(nullptr)
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
    delete m_lineProgram;    // Удаляем новую программу
    delete m_pointProgram;   // Удаляем новую программу
    // delete program;
    doneCurrent();
}

// void OpenGLWidget::initializeGL() {
//     initializeOpenGLFunctions();
//     glEnable(GL_PROGRAM_POINT_SIZE); // Для использования gl_PointSize из шейдера
    
//     vao.create();
//     vao.bind();
    
//     glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//     glEnable(GL_DEPTH_TEST);

//     // Удаляем старую программу
//     delete program;
    
//     // Создаем новую шейдерную программу
//     program = new QOpenGLShaderProgram(this);
    
//     // Вершинный шейдер
//     program->addShaderFromSourceCode(QOpenGLShader::Vertex,
//         "#version 410 core\n"
//         "layout(location = 0) in vec3 a_position;\n"
//         "flat out vec3 start_pos;\n"
//         "out vec3 vert_pos;\n"
//         "uniform mat4 u_mvp;\n"
//         "uniform float u_vertex_size;\n"
//         "void main() {\n"
//         "    vec4 pos = u_mvp * vec4(a_position, 1.0);\n"
//         "    vert_pos = pos.xyz / pos.w;\n"
//         "    start_pos = vert_pos;\n"
//         "    gl_PointSize = u_vertex_size;\n"
//         "    gl_Position = pos;\n"
//         "}");
    
//     // Фрагментный шейдер
//     program->addShaderFromSourceCode(QOpenGLShader::Fragment,
//         "#version 410 core\n"
//         "flat in vec3 start_pos;\n"
//         "in vec3 vert_pos;\n"
//         "uniform vec2 u_resolution;\n"
//         "uniform vec3 u_color;\n"
//         "uniform float u_dash_size;\n"
//         "uniform float u_gap_size;\n"
//         "uniform bool use_dashing;\n"
//         "uniform int u_vertex_display;\n"
//         "out vec4 fragColor;\n"
//         "void main() {\n"
//         "    if (u_vertex_display == 1) {\n"
//         "        vec2 coord = gl_PointCoord.xy - vec2(0.5);\n"
//         "        if (dot(coord, coord) > 0.25) {\n"
//         "            discard;\n"
//         "        }\n"
//         "    }\n"
//         "    if (use_dashing) {\n"
//         "        vec2 dir = (vert_pos.xy - start_pos.xy) * u_resolution/2.0;\n"
//         "        float dist = length(dir);\n"
//         "        float cycle = u_dash_size + u_gap_size;\n"
//         "        if (fract(dist / cycle) > u_dash_size/cycle) {\n"
//         "            discard;\n"
//         "        }\n"
//         "    }\n"
//         "    fragColor = vec4(u_color, 1.0);\n"
//         "}");
    
//     program->link();
//     program->bind();

//     vbo.create();
//     ibo.create();

//     m_indexCount = 0;
//     m_vertexCount = 0;
//     resizeGL(width(), height());
// }

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);

    vao.create();
    vao.bind();

    // Программа для линий
    m_lineProgram = new QOpenGLShaderProgram(this);
    
    // Вершинный шейдер для линий
    m_lineProgram->addShaderFromSourceCode(QOpenGLShader::Vertex,
        "#version 410 core\n"
        "layout(location = 0) in vec3 a_position;\n"
        "out vec3 v_position;\n"
        "uniform mat4 u_mvp;\n"
        "void main() {\n"
        "    v_position = a_position;\n"
        "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
        "}");
    
    // Геометрический шейдер для линий
    m_lineProgram->addShaderFromSourceCode(QOpenGLShader::Geometry,
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
    m_lineProgram->addShaderFromSourceCode(QOpenGLShader::Fragment,
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
    
    m_lineProgram->link();
    
    // Программа для точек
    m_pointProgram = new QOpenGLShaderProgram(this);
    
    // Вершинный шейдер для точек
    m_pointProgram->addShaderFromSourceCode(QOpenGLShader::Vertex,
        "#version 410 core\n"
        "layout(location = 0) in vec3 a_position;\n"
        "uniform mat4 u_mvp;\n"
        "uniform float u_vertex_size;\n"
        "void main() {\n"
        "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
        "    gl_PointSize = u_vertex_size;\n"
        "}");
    
    // Фрагментный шейдер для точек
    m_pointProgram->addShaderFromSourceCode(QOpenGLShader::Fragment,
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
    
    m_pointProgram->link();

    // Инициализация буферов
    vbo.create();
    ibo.create();
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

// void OpenGLWidget::paintGL() {
//     vao.bind();
//     glClearColor(m_bgColor.x(), m_bgColor.y(), m_bgColor.z(), 1.0f);
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//     program->bind();

//     QMatrix4x4 view;
//     view.translate(0.0f, 0.0f, -15.0f);
//     QMatrix4x4 model;
//     model.translate(m_translation);
//     model.rotate(m_rotation.x(), 1.0f, 0.0f, 0.0f);
//     model.rotate(m_rotation.y(), 0.0f, 1.0f, 0.0f);
//     model.rotate(m_rotation.z(), 0.0f, 0.0f, 1.0f);
//     model.scale(m_scale);
//     QMatrix4x4 mvp = projection * view * model;

//     // Передаем параметры в шейдеры
//     program->setUniformValue("u_mvp", mvp);
//     program->setUniformValue("u_resolution", QVector2D(width(), height()));
//     program->setUniformValue("u_vertex_size", m_vertexSize);

//     if (m_vertexCount > 0) {
//         vbo.bind();
//         ibo.bind();
//         program->enableAttributeArray(0);
//         program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));

//         // Отрисовка ребер
//         program->setUniformValue("u_vertex_display", 0);
//         program->setUniformValue("use_dashing", m_edgeType == Dashed);
//         program->setUniformValue("u_dash_size", m_dashSize);
//         program->setUniformValue("u_gap_size", m_gapSize);
//         program->setUniformValue("u_color", m_edgeColor);
//         glDrawElements(GL_LINES, m_indexCount, GL_UNSIGNED_INT, nullptr);

//         // Отрисовка вершин
//         if (m_vertexDisplay != None) {
//             program->setUniformValue("u_vertex_display", m_vertexDisplay);
//             program->setUniformValue("use_dashing", false); // Отключаем пунктир
//             program->setUniformValue("u_color", m_vertexColor);
//             glDrawArrays(GL_POINTS, 0, m_vertexCount);
//         }
//         vbo.release();
//         ibo.release();
//     }

//     program->release();
//     vao.release();
// }

void OpenGLWidget::paintGL() {
    vao.bind();
    glClearColor(m_bgColor.x(), m_bgColor.y(), m_bgColor.z(), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 view;
    view.translate(0.0f, 0.0f, -15.0f);
    QMatrix4x4 model;
    model.translate(m_translation);
    model.rotate(m_rotation.x(), 1.0f, 0.0f, 0.0f);
    model.rotate(m_rotation.y(), 0.0f, 1.0f, 0.0f);
    model.rotate(m_rotation.z(), 0.0f, 0.0f, 1.0f);
    model.scale(m_scale);
    QMatrix4x4 mvp = projection * view * model;

    // ===== Отрисовка линий =====
    if (m_indexCount > 0) {
        m_lineProgram->bind();
        
        // Передаем параметры
        m_lineProgram->setUniformValue("u_mvp", mvp);
        m_lineProgram->setUniformValue("u_resolution", QVector2D(width(), height()));
        m_lineProgram->setUniformValue("u_thickness", m_edgeThickness);
        m_lineProgram->setUniformValue("u_color", m_edgeColor);
        m_lineProgram->setUniformValue("use_dashing", m_edgeType == Dashed);
        m_lineProgram->setUniformValue("u_dash_size", m_dashSize);
        m_lineProgram->setUniformValue("u_gap_size", m_gapSize);

        // Атрибуты вершин
        vbo.bind();
        ibo.bind();
        m_lineProgram->enableAttributeArray(0);
        m_lineProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));
        
        // Рисуем линии
        glDrawElements(GL_LINES, m_indexCount, GL_UNSIGNED_INT, nullptr);
        
        vbo.release();
        ibo.release();
        m_lineProgram->release();
    }

    // ===== Отрисовка точек =====
    if (m_vertexDisplay != None && m_vertexCount > 0) {
        m_pointProgram->bind();
        
        // Передаем параметры
        m_pointProgram->setUniformValue("u_mvp", mvp);
        m_pointProgram->setUniformValue("u_vertex_size", m_vertexSize);
        m_pointProgram->setUniformValue("u_color", m_vertexColor);
        m_pointProgram->setUniformValue("u_vertex_display", m_vertexDisplay);

        // Атрибуты вершин
        vbo.bind();
        m_pointProgram->enableAttributeArray(0);
        m_pointProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));
        
        // Рисуем точки
        glDrawArrays(GL_POINTS, 0, m_vertexCount);
        
        vbo.release();
        m_pointProgram->release();
    }

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
    
    // program->bind();
    // vbo.bind();
    // program->enableAttributeArray(0);
    // program->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    // vbo.release();
    // program->release();
    // Настройка для линии
    m_lineProgram->bind();
    vbo.bind();
    m_lineProgram->enableAttributeArray(0);
    m_lineProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));
    vbo.release();
    m_lineProgram->release();
    
    // Настройка для точек
    m_pointProgram->bind();
    vbo.bind();
    m_pointProgram->enableAttributeArray(0);
    m_pointProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));
    vbo.release();
    m_pointProgram->release();
    
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