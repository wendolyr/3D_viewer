#include "CyclicDoubleSpinBox.h"
#include <cmath>
#include <QWheelEvent>

CyclicDoubleSpinBox::CyclicDoubleSpinBox(QWidget *parent)
    : QDoubleSpinBox(parent) {}

void CyclicDoubleSpinBox::stepBy(int steps) {
    const double minVal = minimum();
    const double maxVal = maximum();
    const double step = singleStep();
    double newValue = value() + steps * step;

    // Рассчитываем диапазон цикла
    const double range = maxVal - minVal + step;

    if (range <= 0) {
        setValue(newValue);
        return;
    }

    // Корректируем значение с учётом циклического диапазона
    if (newValue > maxVal) {
        newValue = minVal + std::fmod(newValue - minVal, range);
        if (newValue < minVal) newValue += range;
    } else if (newValue < minVal) {
        newValue = maxVal - std::fmod(minVal - newValue, range);
        if (newValue > maxVal) newValue -= range;
    }

    // Убедимся, что значение находится в пределах [minVal, maxVal]
    if (newValue < minVal) newValue = minVal;
    if (newValue > maxVal) newValue = maxVal;
    
    setValue(newValue);
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