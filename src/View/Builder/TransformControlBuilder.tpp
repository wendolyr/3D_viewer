#include "TransformControlBuilder.h"

template <typename SpinBoxType>
TransformBuilder<SpinBoxType>::TransformBuilder(TransformType type)
    : type_(type) {
  group_ = new QGroupBox(GetTitle());
  layout_ = new QFormLayout(group_);
}

template <typename SpinBoxType>
TransformBuilder<SpinBoxType>& TransformBuilder<SpinBoxType>::AddAxis(
    Axis axis, double step, SpinBoxType*& widgetRef,
    std::function<void()> minusHandler, std::function<void()> plusHandler) {
  QString label = GetAxisLabel(axis);
  if (type_ == TransformType::Scale) {
    auto axis_widget = TemplateAxisControlBuilder<SpinBoxType>()
                           .WithRange(GetMinValue(), GetMaxValue())
                           .WithDefaultValue(1.0)
                           .WithStep(step)
                           .WithDecimals(GetDecimals())
                           .Build(widgetRef, minusHandler, plusHandler);
    layout_->addRow(label, axis_widget);
  } else {
    auto axis_widget = TemplateAxisControlBuilder<SpinBoxType>()
                           .WithRange(GetMinValue(), GetMaxValue())
                           .WithStep(step)
                           .WithDecimals(GetDecimals())
                           .Build(widgetRef, minusHandler, plusHandler);
    layout_->addRow(label, axis_widget);
  }

  return *this;
}

template <typename SpinBoxType>
QString TransformBuilder<SpinBoxType>::GetTitle() const {
  switch (type_) {
    case TransformType::Move:
      return "Перемещение";
    case TransformType::Rotate:
      return "Поворот";
    case TransformType::Scale:
      return "Масштабирование";
  }
}

template <typename SpinBoxType>
QString TransformBuilder<SpinBoxType>::GetAxisLabel(Axis axis) const {
  switch (axis) {
    case Axis::X:
      return "По X:";
    case Axis::Y:
      return "По Y:";
    case Axis::Z:
      return "По Z:";
    case Axis::None:
      return "Коэффициент:";
  }
}

template <typename SpinBoxType>
double TransformBuilder<SpinBoxType>::GetMinValue() const {
  switch (type_) {
    case TransformType::Move:
      return -100000.0;
    case TransformType::Rotate:
      return 0.0;
    case TransformType::Scale:
      return 0.01;
  }
}

template <typename SpinBoxType>
double TransformBuilder<SpinBoxType>::GetMaxValue() const {
  switch (type_) {
    case TransformType::Move:
      return 100000.0;
    case TransformType::Rotate:
      return 359.0;
    case TransformType::Scale:
      return 1000.0;
  }
}

template <typename SpinBoxType>
int TransformBuilder<SpinBoxType>::GetDecimals() const {
  switch (type_) {
    case TransformType::Rotate:
      return 0;
    default:
      return 2;
  }
}