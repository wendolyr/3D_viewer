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
    enum ProjectionType { Perspective, Orthographic };
    enum EdgeType { Solid, Dashed };
    enum VertexDisplay { None, Circle, Square };

    explicit OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget();
    void setModelData(const QVector<QVector3D>& vertices, const QVector<QPair<unsigned, unsigned>>& edges); // Получение данных
    void clearModel();
    void setTransformations(const QVector3D& translation, const QVector3D& rotation, float scale);

    void setProjectionType(ProjectionType type);
    void setEdgeSettings(EdgeType type, const QVector3D& color, float thickness);
    void setVertexSettings(VertexDisplay display, const QVector3D& color, float size);
    void setBackgroundColor(const QVector3D& color);
protected:
    // Переопределенные методы QOpenGLWidget
    void initializeGL() override;   // Инициализация OpenGL
    void resizeGL(int w, int h) override;   //Обработка изменения размера
    void paintGL() override;    // Отрисовка кадра
private:
    // QOpenGLShaderProgram *m_program_edge;     // Для линий
    // QOpenGLShaderProgram *m_program_points; // Для точек
    QOpenGLShaderProgram *program;
    QOpenGLBuffer vbo;              // Буфер вершин (Vertex Buffer)
    QOpenGLBuffer ibo;              // Буфер индексов (Index Buffer)
    QMatrix4x4 projection;          // Матрица проекции

    int m_indexCount = 0;   // Количество индексов для отрисовки
    int m_vertexCount = 0;

    QVector3D m_translation;
    QVector3D m_rotation;
    float m_scale;

    ProjectionType m_projectionType;
    EdgeType m_edgeType;
    QVector3D m_edgeColor;
    float m_edgeThickness;
    
    VertexDisplay m_vertexDisplay;
    QVector3D m_vertexColor;
    float m_vertexSize;
    
    QVector3D m_bgColor;
};

#endif  // CPP4_3DVIEWER_V2_0_VIEW_OPENGLWIDGET_H