#include "transform_control_builder.h"

namespace s21 {
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
  if (type_ == TransformType::kScale) {
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
    case TransformType::kMove:
      return "Перемещение";
    case TransformType::kRotate:
      return "Поворот";
    case TransformType::kScale:
      return "Масштабирование";
  }
}

template <typename SpinBoxType>
QString TransformBuilder<SpinBoxType>::GetAxisLabel(Axis axis) const {
  switch (axis) {
    case Axis::kX:
      return "По X:";
    case Axis::kY:
      return "По Y:";
    case Axis::kZ:
      return "По Z:";
    case Axis::kNone:
      return "Коэффициент:";
  }
}

template <typename SpinBoxType>
double TransformBuilder<SpinBoxType>::GetMinValue() const {
  switch (type_) {
    case TransformType::kMove:
      return -100000.0;
    case TransformType::kRotate:
      return 0.0;
    case TransformType::kScale:
      return 0.01;
  }
}

template <typename SpinBoxType>
double TransformBuilder<SpinBoxType>::GetMaxValue() const {
  switch (type_) {
    case TransformType::kMove:
      return 100000.0;
    case TransformType::kRotate:
      return 360.0;
    case TransformType::kScale:
      return 1000.0;
  }
}

template <typename SpinBoxType>
int TransformBuilder<SpinBoxType>::GetDecimals() const {
  switch (type_) {
    case TransformType::kRotate:
      return 0;
    default:
      return 2;
  }
}
}  // namespace s21