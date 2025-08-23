#include "cyclic_double_spin_box.h"

#include <QWheelEvent>
#include <cmath>

namespace s21 {
CyclicDoubleSpinBox::CyclicDoubleSpinBox(QWidget *parent)
    : QDoubleSpinBox(parent) {}

void CyclicDoubleSpinBox::stepBy(int steps) {
  const double min_val = minimum();
  const double max_val = maximum();
  const double step = singleStep();
  double new_value = value() + steps * step;

  const double range = max_val - min_val + step;

  if (new_value >= min_val && new_value <= max_val) {
    setValue(new_value);
    return;
  }

  double normalized_value = std::fmod(new_value - min_val, range);
  if (normalized_value < 0) {
    normalized_value += range;
  }
  normalized_value += min_val;
  if (normalized_value > max_val) {
    normalized_value -= range;
  }

  setValue(normalized_value);
}

void CyclicDoubleSpinBox::wheelEvent(QWheelEvent *event) {
  // Определяем направление прокрутки
  int steps = (event->angleDelta().y() > 0) ? 1 : -1;

  stepBy(steps);

  // Принимаем событие, чтобы предотвратить стандартную обработку
  event->accept();
}

void CyclicDoubleSpinBox::keyPressEvent(QKeyEvent *event) {
  // Обрабатываем стрелки вверх/вниз
  if (event->key() == Qt::Key_Up) {
    stepBy(1);
    event->accept();
  } else if (event->key() == Qt::Key_Down) {
    stepBy(-1);
    event->accept();
  } else {
    // Для остальных клавиш используем стандартную обработку
    QDoubleSpinBox::keyPressEvent(event);
  }
}
}  // namespace s21