#ifndef VIEWER3D_V2_0_VIEWER_H
#define VIEWER3D_V2_0_VIEWER_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <QLabel>

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

    QString m_full_file_name;

    QDoubleSpinBox* m_move_x;
    QDoubleSpinBox* m_move_y;
    QDoubleSpinBox* m_move_z;
    QDoubleSpinBox* m_rotate_x;
    QDoubleSpinBox* m_rotate_y;
    QDoubleSpinBox* m_rotate_z;
    QDoubleSpinBox* m_scale;

    QLabel* m_file_name_label;
    QLabel* m_vertex_count_label;
    QLabel* m_edge_count_label;

    // Создание пользовательского интерфейса
    void SetupUI();
    // Создание виджета управления для оси
    QWidget* CreateAxisWidgets(QDoubleSpinBox*& spin_box, TransformType type, Axis axis);
    // Получение шага для изменения модели
    double GetStepValue(TransformType type) const;
    // Выполнение шага изменения при нажатии
    double DoStep(double value, bool sum, TransformType type);
    // Обновление отображения имени файла
    void UpdateFileNameLabel();
    // Открытие и парсер obj-файла с моделью
    int LoadModelData(const QString& file_path);
    // Пропуск не нужных строк в парсере
    static const char* SkipToNextLine(const char* ptr, const char* end);

private slots:
    // Сброс преобразований
    void ResetTransform();
    // TODO Метод для применения преобразований к модели
};

#endif  // VIEWER3D_V2_0_VIEWER_H