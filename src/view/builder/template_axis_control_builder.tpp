#include "template_axis_control_builder.h"

namespace s21 {
template <typename SpinBoxType>
QWidget* TemplateAxisControlBuilder<SpinBoxType>::Build(
    SpinBoxType*& spin_box_ref, std::function<void()> minus_handler,
    std::function<void()> plus_handler) {
  QWidget* container = new QWidget();
  QHBoxLayout* layout = new QHBoxLayout(container);
  layout->setContentsMargins(0, 0, 0, 0);

  QPushButton* btn_minus = nullptr;
  QPushButton* btn_plus = nullptr;
  spin_box_ref = nullptr;

  btn_minus = new QPushButton("-", container);
  btn_minus->setFixedWidth(25);
  layout->addWidget(btn_minus);

  SpinBoxType* spin_box = new SpinBoxType(container);
  spin_box->setRange(range_.first, range_.second);
  spin_box->setDecimals(decimals_);
  spin_box->setValue(default_value_);
  spin_box->setSingleStep(step_);
  spin_box->setButtonSymbols(QAbstractSpinBox::NoButtons);
  layout->addWidget(spin_box);
  spin_box_ref = spin_box;

  btn_plus = new QPushButton("+", container);
  btn_plus->setFixedWidth(25);
  layout->addWidget(btn_plus);

  if (btn_minus && minus_handler) {
    QObject::connect(btn_minus, &QPushButton::clicked, minus_handler);
  }

  if (btn_plus && plus_handler) {
    QObject::connect(btn_plus, &QPushButton::clicked, plus_handler);
  }

  return container;
}
}  // namespace s21