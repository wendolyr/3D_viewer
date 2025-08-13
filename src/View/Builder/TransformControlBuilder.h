#ifndef CPP4_3DVIEWER_V2_0_VIEW_BUILDER_TRANSFORMCONTROLBUILDER_H
#define CPP4_3DVIEWER_V2_0_VIEW_BUILDER_TRANSFORMCONTROLBUILDER_H

#include <QFormLayout>
#include <QGroupBox>

#include "../Enum.h"
#include "TemplateAxisControlBuilder.h"

template <typename SpinBoxType>
class TransformBuilder {
 public:
  explicit TransformBuilder(TransformType type);

  TransformBuilder& AddAxis(Axis axis, float step, SpinBoxType*& widgetRef,
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

#include "TransformControlBuilder.tpp"

#endif  // CPP4_3DVIEWER_V2_0_VIEW_BUILDER_TRANSFORMCONTROLBUILDER_H