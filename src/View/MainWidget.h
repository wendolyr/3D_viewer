#ifndef CPP4_3DVIEWER_V2_0_VIEW_MAINWIDGET_H
#define CPP4_3DVIEWER_V2_0_VIEW_MAINWIDGET_H

#include "OpenGLWidget.h"
#include "CyclicDoubleSpinBox.h"

#include <QWidget>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QVector3D>

class MainWidget : public QWidget {
public:
    MainWidget(QWidget *parent = nullptr);

public slots:
    // Слот для загрузки модели
    void LoadModel();

protected:
    // Обработчик изменения размеров окна
    void resizeEvent(QResizeEvent *event) override;

private:
    enum class TransformType { Move, Rotate, Scale };
    enum class Axis { X, Y, Z, None };
    OpenGLWidget* m_gl_widget;

    QString m_full_file_name;

    QDoubleSpinBox* m_move_x;
    QDoubleSpinBox* m_move_y;
    QDoubleSpinBox* m_move_z;
    CyclicDoubleSpinBox* m_rotate_x;
    CyclicDoubleSpinBox* m_rotate_y;
    CyclicDoubleSpinBox* m_rotate_z;
    QDoubleSpinBox* m_scale;

    QLabel* m_file_name_label;
    QLabel* m_vertex_count_label;
    QLabel* m_edge_count_label;

    QVector<QVector3D> vertices;    // Вершины
    QSet<QPair<unsigned, unsigned>> edges;  // Ребра

    // Создание пользовательского интерфейса
    void SetupUI();
    // Создание виджета управления для оси
    QWidget* CreateAxisWidgetsMoveAndScale(QDoubleSpinBox*& spin_box, TransformType type, Axis axis);
    QWidget* CreateAxisWidgetsRotate(CyclicDoubleSpinBox*& spin_box, Axis axis);
    // Получение шага для изменения модели
    double GetStepValue(TransformType type) const;
    // Выполнение шага изменения при нажатии
    double DoStep(double value, bool sum, TransformType type);
    // Обновление отображения имени файла
    void UpdateFileNameLabel();
    // Открытие и парсер obj-файла с моделью
    int LoadModelData(const QString& file_path);
    // Пропуск не нужных строк в парсере
    inline const char* SkipToNextLine(const char* ptr, const char* end);

private slots:
    // Сброс преобразований
    void ResetTransform();
    // TODO Метод для применения преобразований к модели
    void OnTransformChanged();
};

#endif  // CPP4_3DVIEWER_V2_0_VIEW_MAINWIDGET_H