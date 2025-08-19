#ifndef CPP4_3DVIEWER_V2_0_VIEW_CYCLIC_DOUBLE_SPIN_BOX_H
#define CPP4_3DVIEWER_V2_0_VIEW_CYCLIC_DOUBLE_SPIN_BOX_H

#include <QDoubleSpinBox>
#include <QKeyEvent>
#include <QWheelEvent>

namespace s21 {
class CyclicDoubleSpinBox : public QDoubleSpinBox {
  Q_OBJECT
 public:
  explicit CyclicDoubleSpinBox(QWidget *parent = nullptr);

 protected:
  void stepBy(int steps) override;
  void wheelEvent(QWheelEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
};
} // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_VIEW_CYCLIC_DOUBLE_SPIN_BOX_H