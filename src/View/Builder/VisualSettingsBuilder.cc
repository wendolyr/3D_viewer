#include "VisualSettingsBuilder.h"

#include <QHBoxLayout>
#include <QLabel>

#include "ColorWidgetBuilder.h"

namespace s21 {
VisualSettingsBuilder::VisualSettingsBuilder() {
  group_box_ = new QGroupBox();
  main_layout_ = new QVBoxLayout(group_box_);
  main_layout_->setContentsMargins(0, 0, 0, 0);
}

VisualSettingsBuilder& VisualSettingsBuilder::AddComboBox(
    const QString& label, QComboBox*& combo_ref, const QStringList& items) {
  QWidget* container = new QWidget();
  QHBoxLayout* layout = new QHBoxLayout(container);
  layout->setContentsMargins(0, 0, 0, 0);

  layout->addWidget(new QLabel(label));
  combo_ref = new QComboBox();
  combo_ref->addItems(items);
  layout->addWidget(combo_ref);

  main_layout_->addWidget(container);
  return *this;
}

VisualSettingsBuilder& VisualSettingsBuilder::AddColorWidget(
    const QString& label, QSpinBox*& r_ref, QSpinBox*& g_ref, QSpinBox*& b_ref,
    QLabel*& preview_ref, const QColor& default_color) {
  QWidget* color_widget = ColorWidgetBuilder()
                              .SetLabel(label)
                              .SetDefaultColor(default_color)
                              .Build(r_ref, g_ref, b_ref, preview_ref);
  main_layout_->addWidget(color_widget);
  return *this;
}

VisualSettingsBuilder& VisualSettingsBuilder::AddDoubleSpinBox(
    const QString& label, QDoubleSpinBox*& spin_ref, double min, double max,
    double step, int decimals) {
  QWidget* container = new QWidget();
  QHBoxLayout* layout = new QHBoxLayout(container);
  layout->setContentsMargins(0, 0, 0, 0);

  layout->addWidget(new QLabel(label));
  spin_ref = new QDoubleSpinBox();
  spin_ref->setRange(min, max);
  spin_ref->setSingleStep(step);
  spin_ref->setDecimals(decimals);
  layout->addWidget(spin_ref);

  main_layout_->addWidget(container);
  return *this;
}

VisualSettingsBuilder& VisualSettingsBuilder::SetMargins(int l, int t, int r,
                                                         int b) {
  main_layout_->setContentsMargins(l, t, r, b);
  return *this;
}

VisualSettingsBuilder& VisualSettingsBuilder::SetSpacing(int spacing) {
  main_layout_->setSpacing(spacing);
  return *this;
}

QGroupBox* VisualSettingsBuilder::Build(const QString& title) {
  group_box_->setTitle(title);
  return group_box_;
}
}  // namespace s21