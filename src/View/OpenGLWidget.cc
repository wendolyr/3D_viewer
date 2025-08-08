#include "OpenGLWidget.h"

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
    qDebug() << "OpenGL version:" << (const char*)glGetString(GL_VERSION);
    qDebug() << "GLSL version:" << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

    vao.create();
    vao.bind();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    program = new QOpenGLShaderProgram(this);
    program->addShaderFromSourceCode(QOpenGLShader::Vertex,
        "#version 410 core\n"
        "layout(location = 0) in vec3 vertexPosition;"
        "uniform mat4 mvpMatrix;"
        "void main() {"
        "   gl_Position = mvpMatrix * vec4(vertexPosition, 1.0);"
        "}");
    
    program->addShaderFromSourceCode(QOpenGLShader::Fragment,
        "#version 410 core\n"
        "uniform vec3 color;"
        "out vec4 fragColor;"
        "void main() {"
        "   fragColor = vec4(color, 1.0);"
        "}");
    // program->addShaderFromSourceCode(QOpenGLShader::Vertex,
    //     "attribute vec4 vertexPosition;"
    //     "uniform mat4 mvpMatrix;"
    //     "void main() {"
    //     "   gl_Position = mvpMatrix * vertexPosition;"
    //     "}");
    // program->addShaderFromSourceCode(QOpenGLShader::Fragment,
    //     "uniform vec3 color;"
    //     "void main() {"
    //     "   gl_FragColor = vec4(color, 1.0);"
    //     "}");
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
    } else { // Orthographic
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
    // Устанавливаем цвет фона
    glClearColor(m_bgColor.x(), m_bgColor.y(), m_bgColor.z(), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program->bind();

    QMatrix4x4 view;
    view.translate(0.0f, 0.0f, -15.0f);
    QMatrix4x4 model;

    // Нужно будет перетащить в модель
    model.translate(m_translation);
    model.rotate(m_rotation.x(), 1.0f, 0.0f, 0.0f);
    model.rotate(m_rotation.y(), 0.0f, 1.0f, 0.0f);
    model.rotate(m_rotation.z(), 0.0f, 0.0f, 1.0f);
    model.scale(m_scale);

    QMatrix4x4 mvp = projection * view * model;
    program->setUniformValue("mvpMatrix", mvp);


    if (m_vertexCount > 0) {
        vbo.bind();
        ibo.bind();
        // program->enableAttributeArray("vertexPosition");
        // program->setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3);

        program->enableAttributeArray(0);
        program->setAttributeBuffer(0, GL_FLOAT, 0, 3);

        // Рисуем ребра
        glLineWidth(m_edgeThickness);
        program->setUniformValue("color", m_edgeColor);
        glDrawElements(GL_LINES, m_indexCount, GL_UNSIGNED_INT, nullptr);
        
        // Рисуем вершины
        if (m_vertexDisplay != None) {
            glPointSize(m_vertexSize);
            program->setUniformValue("color", m_vertexColor);
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
    
    // Очищаем старые буферы
    vbo.destroy();
    ibo.destroy();
    m_indexCount = 0;
    m_vertexCount = vertices.size();

    // Создаем VBO
    vbo.create();
    vbo.bind();
    vbo.allocate(vertices.constData(), vertices.size() * sizeof(QVector3D));
    
    // Создаем IBO
    QVector<GLuint> indices;
    indices.reserve(edges.size() * 2);
    for (const auto& edge : edges) {
        // Проверка валидности индексов
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
    
    // Настраиваем атрибуты
    program->bind();
    vbo.bind();
    // program->enableAttributeArray("vertexPosition");
    // program->setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3);
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
    resizeGL(width(), height()); // Пересчитываем проекцию
    update();
}

void OpenGLWidget::setEdgeSettings(EdgeType type, const QVector3D& color, float thickness) {
    m_edgeType = type;
    m_edgeColor = color;
    m_edgeThickness = thickness;
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