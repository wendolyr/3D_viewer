#ifndef CPP4_3DVIEWER_V2_0_VIEW_BUILDER_TEMPLATE_AXIS_CONTROL_BUILDER_H
#define CPP4_3DVIEWER_V2_0_VIEW_BUILDER_TEMPLATE_AXIS_CONTROL_BUILDER_H

#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>

#include "../cyclic_double_spin_box.h"

namespace s21 {
/**
 * @class TemplateAxisControlBuilder
 * @brief Шаблонный строитель элементов управления для осей
 * @tparam SpinBoxType Тип спинбокса (QDoubleSpinBox или CyclicDoubleSpinBox)
 *
 * Позволяет гибко создавать блоки управления с кнопками +/- и спинбоксом
 * с настраиваемыми параметрами.
 */
template <typename SpinBoxType>
class TemplateAxisControlBuilder {
 public:
  TemplateAxisControlBuilder() = default;
  /**
   * @brief Устанавливает диапазон значений
   * @param min Минимальное значение
   * @param max Максимальное значение
   */
  TemplateAxisControlBuilder& WithRange(double min, double max) {
    range_ = qMakePair(min, max);
    return *this;
  }
  /**
   * @brief Устанавливает начальное значение
   * @param value Значение по умолчанию
   */
  TemplateAxisControlBuilder& WithDefaultValue(double value) {
    default_value_ = value;
    return *this;
  }
  /**
   * @brief Устанавливает шаг изменения значения
   * @param step_ Величина шага
   */
  TemplateAxisControlBuilder& WithStep(double step_) {
    this->step_ = step_;
    return *this;
  }
  /**
   * @brief Устанавливает точность отображения
   * @param decimals_ Количество знаков после запятой
   */
  TemplateAxisControlBuilder& WithDecimals(double decimals_) {
    this->decimals_ = decimals_;
    return *this;
  }
  /**
   * @brief Создает элемент управления
   * @param[out] spin_box_ref Ссылка на указатель созданного спинбокса
   * @param minus_handler Обработчик для кнопки уменьшения
   * @param plus_handler Обработчик для кнопки увеличения
   */
  QWidget* Build(SpinBoxType*& spin_box_ref,
                 std::function<void()> minus_handler = nullptr,
                 std::function<void()> plus_handler = nullptr);

 private:
  QPair<double, double> range_ = {0.0, 100.0};  ///< Диапазон значений спинбокса
  double default_value_ = 0.0;  ///< Значение по умолчанию
  double step_ = 0.1;           ///< Шаг изменения значения
  int decimals_ = 2;  ///< Количество знаков после запятой
};
}  // namespace s21

#include "template_axis_control_builder.tpp"

#endif  // CPP4_3DVIEWER_V2_0_VIEW_BUILDER_TEMPLATE_AXIS_CONTROL_BUILDER_H