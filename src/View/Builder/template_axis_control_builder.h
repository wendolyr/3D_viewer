#ifndef CPP4_3DVIEWER_V2_0_VIEW_BUILDER_TEMPLATE_AXIS_CONTROL_BUILDER_H
#define CPP4_3DVIEWER_V2_0_VIEW_BUILDER_TEMPLATE_AXIS_CONTROL_BUILDER_H

#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>

#include "../cyclic_double_spin_box.h"

namespace s21 {
template <typename SpinBoxType>
class TemplateAxisControlBuilder {
 public:
  TemplateAxisControlBuilder() = default;

  TemplateAxisControlBuilder& WithoutMinusButton() {
    add_minus_button_ = false;
    return *this;
  }

  TemplateAxisControlBuilder& WithoutPlusButton() {
    add_plus_button_ = false;
    return *this;
  }

  TemplateAxisControlBuilder& WithoutSpinBox() {
    add_spin_box_ = false;
    return *this;
  }

  TemplateAxisControlBuilder& WithRange(double min, double max) {
    range_ = qMakePair(min, max);
    return *this;
  }

  TemplateAxisControlBuilder& WithDefaultValue(double value) {
    default_value_ = value;
    return *this;
  }

  TemplateAxisControlBuilder& WithStep(double step_) {
    this->step_ = step_;
    return *this;
  }

  TemplateAxisControlBuilder& WithDecimals(double decimals_) {
    this->decimals_ = decimals_;
    return *this;
  }

  QWidget* Build(SpinBoxType*& spin_box_ref,
                 std::function<void()> minus_handler = nullptr,
                 std::function<void()> plus_handler = nullptr);

 private:
  bool add_minus_button_ = true;
  bool add_plus_button_ = true;
  bool add_spin_box_ = true;
  QPair<double, double> range_ = {0.0, 100.0};
  double default_value_ = 0.0;
  double step_ = 0.1;
  int decimals_ = 2;
};
} // namespace s21

#include "template_axis_control_builder.tpp"

#endif  // CPP4_3DVIEWER_V2_0_VIEW_BUILDER_TEMPLATE_AXIS_CONTROL_BUILDER_H