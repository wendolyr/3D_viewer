#include "OpenGLWidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      ibo(QOpenGLBuffer::IndexBuffer),
      m_indexCount(0)
{
    // QSurfaceFormat format;
    // format.setVersion(4, 1);
    // // format.setProfile(QSurfaceFormat::CoreProfile);
    // format.setDepthBufferSize(24);
    // setFormat(format);
}

OpenGLWidget::~OpenGLWidget() {
    makeCurrent();
    vbo.destroy();
    ibo.destroy();
    m_indexCount = 0;
    delete program;
    doneCurrent();
}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    program = new QOpenGLShaderProgram(this);
    program->addShaderFromSourceCode(QOpenGLShader::Vertex,
        "attribute vec4 vertexPosition;"
        "uniform mat4 mvpMatrix;"
        "void main() {"
        "   gl_Position = mvpMatrix * vertexPosition;"
        "}");
    program->addShaderFromSourceCode(QOpenGLShader::Fragment,
        "uniform vec3 color;"
        "void main() {"
        "   gl_FragColor = vec4(color, 1.0);"
        "}");
    program->link();
    program->bind();

    // GLfloat vertices[] = {
    //     -1.0f, -1.0f, -1.0f,
    //      1.0f, -1.0f, -1.0f,
    //      1.0f,  1.0f, -1.0f,
    //     -1.0f,  1.0f, -1.0f,
    //     -1.0f, -1.0f,  1.0f,
    //      1.0f, -1.0f,  1.0f,
    //      1.0f,  1.0f,  1.0f,
    //     -1.0f,  1.0f,  1.0f
    // };

    // // Индексы линий (12 ребер = 24 индекса)
    // GLuint indices[] = {
    //     0, 1, 1, 2, 2, 3, 3, 0, // задняя грань
    //     4, 5, 5, 6, 6, 7, 7, 4, // передняя грань
    //     0, 4, 1, 5, 2, 6, 3, 7  // соединения
    // };

    vbo.create();
    // vbo.bind();
    // vbo.allocate(vertices, sizeof(vertices));

    ibo.create();
    // ibo.bind();
    // ibo.allocate(indices, sizeof(indices));

    m_indexCount = 0;
    m_vertexCount = 0;
    // program->enableAttributeArray("vertexPosition");
    // program->setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3);

    // program->setUniformValue("color", QVector3D(1.0f, 1.0f, 1.0f));

    // vbo.release();
    // ibo.release();
    // program->release();

    // glLineWidth(2.0f);
}

void OpenGLWidget::resizeGL(int w, int h) {
    projection.setToIdentity();
    projection.perspective(
        45.0f,
        static_cast<float>(w) / h,
        0.1f,
        10000.0f
    );
}

void OpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program->bind();

    QMatrix4x4 view;
    view.translate(0.0f, 0.0f, -2.5f);
    QMatrix4x4 model;
    QMatrix4x4 mvp = projection * view * model;
    program->setUniformValue("mvpMatrix", mvp);


    if (m_vertexCount > 0) {
        vbo.bind();
        ibo.bind();
        program->enableAttributeArray("vertexPosition");
        program->setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3);

        // Рисуем ребра
        program->setUniformValue("color", QVector3D(0.8f, 0.8f, 1.0f));
        glDrawElements(GL_LINES, m_indexCount, GL_UNSIGNED_INT, nullptr);
        
        // Рисуем вершины
        // program->setUniformValue("color", QVector3D(1.0f, 0.0f, 0.0f));
        // glPointSize(2.0f);
        // glDrawArrays(GL_POINTS, 0, m_vertexCount);

        vbo.release();
        ibo.release();
    }

    // // Статичный куб вариант
    // // Рисуем каркас куба (линии)
    // program->setUniformValue("color", QVector3D(0.8f, 0.8f, 1.0f)); // Цвет линий
    // glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, nullptr);

    // // Рисуем вершины (точки)
    // program->setUniformValue("color", QVector3D(0.8f, 0.8f, 1.0f)); // Красный цвет точек
    // glPointSize(80.0f); // Размер точек
    // glDrawArrays(GL_POINTS, 0, 8); // 8 вершин

    program->release();
}

void OpenGLWidget::setModelData(const QVector<QVector3D>& vertices, const QVector<QPair<unsigned, unsigned>>& edges) {
    if (!isValid()) return; // Проверка инициализации OpenGL
    
    makeCurrent();
    
    // Копируем вершины для нормализации
    QVector<QVector3D> normalizedVertices = vertices;
    normalizeVertices(normalizedVertices);
    
    // Очищаем старые буферы
    vbo.destroy();
    ibo.destroy();
    m_indexCount = 0;
    m_vertexCount = normalizedVertices.size();

    // Создаем VBO
    vbo.create();
    vbo.bind();
    vbo.allocate(normalizedVertices.constData(), normalizedVertices.size() * sizeof(QVector3D));
    
    // Создаем IBO
    QVector<GLuint> indices;
    indices.reserve(edges.size() * 2);
    for (const auto& edge : edges) {
        // Проверка валидности индексов
        if (edge.first < static_cast<unsigned>(normalizedVertices.size()) && 
            edge.second < static_cast<unsigned>(normalizedVertices.size())) {
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
    program->enableAttributeArray("vertexPosition");
    program->setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3);
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

void OpenGLWidget::normalizeVertices(QVector<QVector3D>& vertices) {
    if (vertices.isEmpty()) return;

    // Находим максимальное значение координаты
    float maxVal = 0.0f;
    for (const auto& vertex : vertices) {
        maxVal = std::max(maxVal, std::abs(vertex.x()));
        maxVal = std::max(maxVal, std::abs(vertex.y()));
        maxVal = std::max(maxVal, std::abs(vertex.z()));
    }

    // Масштабируем вершины
    if (maxVal > 0.0f) {
        const float scaleFactor = 1.0f / maxVal;
        for (auto& vertex : vertices) {
            vertex *= scaleFactor;
        }
    }
}

// Старый вариант
// void OpenGLWidget::paintGL() {
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//     program->bind();

//     QMatrix4x4 view;
//     view.translate(0.0f, 0.0f, -5.0f);

//     QMatrix4x4 model;

//     QMatrix4x4 mvp = projection * view * model;
//     program->setUniformValue("mvpMatrix", mvp);

//     program->setUniformValue("color", QVector3D(0.8f, 0.8f, 1.0f));

//     vbo.bind();
//     ibo.bind();
//     program->enableAttributeArray("vertexPosition");

//     glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, nullptr);

//     vbo.release();
//     ibo.release();
//     program->release();
// }