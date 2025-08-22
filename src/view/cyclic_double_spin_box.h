#ifndef CPP4_3DVIEWER_V2_0_VIEW_CYCLIC_DOUBLE_SPIN_BOX_H
#define CPP4_3DVIEWER_V2_0_VIEW_CYCLIC_DOUBLE_SPIN_BOX_H

#include <QDoubleSpinBox>
#include <QKeyEvent>
#include <QWheelEvent>

namespace s21 {
/**
 * @class CyclicDoubleSpinBox
 * @brief Циклический спинбокс для работы с угловыми значениями
 * @details Наследует QDoubleSpinBox и добавляет циклическое поведение для
 * значений, что особенно полезно для угловых значений (0-360 градусов)
 */
class CyclicDoubleSpinBox : public QDoubleSpinBox {
  Q_OBJECT
 public:
  /**
   * @brief Конструктор циклического спинбокса
   * @param parent Родительский виджет
   */
  explicit CyclicDoubleSpinBox(QWidget *parent = nullptr);

 protected:
  /**
   * @brief Выполняет шаг изменения значения
   * @param steps Количество шагов для изменения (может быть отрицательным)
   */
  void stepBy(int steps) override;
  /**
   * @brief Обрабатывает события колесика мыши
   * @param event Событие колесика мыши
   */
  void wheelEvent(QWheelEvent *event) override;
  /**
   * @brief Обрабатывает нажатия клавиш
   * @param event Событие нажатия клавиши
   */
  void keyPressEvent(QKeyEvent *event) override;
};
}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_VIEW_CYCLIC_DOUBLE_SPIN_BOX_H