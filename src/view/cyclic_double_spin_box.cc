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

  // Рассчитываем диапазон цикла
  const double range = max_val - min_val + step;

  if (range <= 0) {
    setValue(new_value);
    return;
  }

  // Корректируем значение с учётом циклического диапазона
  if (new_value > max_val) {
    new_value = min_val + std::fmod(new_value - min_val, range);
    if (new_value < min_val) new_value += range;
  } else if (new_value < min_val) {
    new_value = max_val - std::fmod(min_val - new_value, range);
    if (new_value > max_val) new_value -= range;
  }

  // Убедимся, что значение находится в пределах [min_val, max_val]
  if (new_value < min_val) new_value = min_val;
  if (new_value > max_val) new_value = max_val;

  setValue(new_value);
}

// Обработчик колесика мыши
void CyclicDoubleSpinBox::wheelEvent(QWheelEvent *event) {
  // Определяем направление прокрутки
  int steps = (event->angleDelta().y() > 0) ? 1 : -1;

  // Вызываем stepBy с нужным количеством шагов
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
} // namespace s21