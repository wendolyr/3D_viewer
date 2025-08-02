#ifndef CPP4_3DVIEWER_V2_0_VIEW_CYCLICDOUBLESPINBOX_H
#define CPP4_3DVIEWER_V2_0_VIEW_CYCLICDOUBLESPINBOX_H

#include <QDoubleSpinBox>
#include <QWheelEvent>
#include <QKeyEvent>

class CyclicDoubleSpinBox : public QDoubleSpinBox {
    Q_OBJECT
public:
    explicit CyclicDoubleSpinBox(QWidget *parent = nullptr);

protected:
    void stepBy(int steps) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // CPP4_3DVIEWER_V2_0_VIEW_CYCLICDOUBLESPINBOX_H