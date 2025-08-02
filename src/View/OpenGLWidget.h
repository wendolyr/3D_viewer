#ifndef CPP4_3DVIEWER_V2_0_VIEW_OPENGLWIDGET_H
#define CPP4_3DVIEWER_V2_0_VIEW_OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget();
    void setModelData(const QVector<QVector3D>& vertices, const QVector<QPair<unsigned, unsigned>>& edges); // Получение данных
    void clearModel();
    void setTransformations(const QVector3D& translation, const QVector3D& rotation, float scale);
protected:
    // Переопределенные методы QOpenGLWidget
    void initializeGL() override;   // Инициализация OpenGL
    void resizeGL(int w, int h) override;   //Обработка изменения размера
    void paintGL() override;    // Отрисовка кадра
private:
    QOpenGLShaderProgram *program;  // Шейдерная программа
    QOpenGLBuffer vbo;              // Буфер вершин (Vertex Buffer)
    QOpenGLBuffer ibo;              // Буфер индексов (Index Buffer)
    QMatrix4x4 projection;          // Матрица проекции

    int m_indexCount = 0;   // Количество индексов для отрисовки
    int m_vertexCount = 0;

    QVector3D m_translation;
    QVector3D m_rotation;
    float m_scale;
};

#endif  // CPP4_3DVIEWER_V2_0_VIEW_OPENGLWIDGET_H