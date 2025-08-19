#ifndef CPP4_3DVIEWER_V2_0_VIEW_BUILDER_TRANSFORM_CONTROL_BUILDER_H
#define CPP4_3DVIEWER_V2_0_VIEW_BUILDER_TRANSFORM_CONTROL_BUILDER_H

#include <QFormLayout>
#include <QGroupBox>

#include "../enum.h"
#include "template_axis_control_builder.h"

namespace s21 {
template <typename SpinBoxType>
class TransformBuilder {
 public:
  explicit TransformBuilder(TransformType type);

  TransformBuilder& AddAxis(Axis axis, double step, SpinBoxType*& widgetRef,
                            std::function<void()> minusHandler = nullptr,
                            std::function<void()> plusHandler = nullptr);

  QGroupBox* Build() { return group_; }

 private:
  QString GetTitle() const;
  QString GetAxisLabel(Axis axis) const;
  double GetMinValue() const;
  double GetMaxValue() const;
  int GetDecimals() const;

  TransformType type_;
  QGroupBox* group_;
  QFormLayout* layout_;
};
} // namespace s21

#include "transform_control_builder.tpp"

#endif  // CPP4_3DVIEWER_V2_0_VIEW_BUILDER_TRANSFORM_CONTROL_BUILDER_H