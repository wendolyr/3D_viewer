#include "color_widget_builder.h"

#include <QHBoxLayout>
#include <QLabel>

namespace s21 {
ColorWidgetBuilder& ColorWidgetBuilder::SetLabel(const QString& label) {
  label_ = label;
  return *this;
}

ColorWidgetBuilder& ColorWidgetBuilder::SetDefaultColor(const QColor& color) {
  default_color_ = color;
  return *this;
}

QWidget* ColorWidgetBuilder::Build(QSpinBox*& r_ref, QSpinBox*& g_ref,
                                   QSpinBox*& b_ref, QLabel*& preview_ref) {
  QWidget* container = new QWidget();
  QHBoxLayout* layout = new QHBoxLayout(container);
  layout->setContentsMargins(0, 0, 0, 0);

  QLabel* color_label = new QLabel(label_);
  color_label->setMinimumWidth(65);

  r_ref = new QSpinBox;
  r_ref->setRange(0, 255);
  r_ref->setValue(default_color_.red());
  r_ref->setMaximumWidth(50);

  g_ref = new QSpinBox;
  g_ref->setRange(0, 255);
  g_ref->setValue(default_color_.green());
  g_ref->setMaximumWidth(50);

  b_ref = new QSpinBox;
  b_ref->setRange(0, 255);
  b_ref->setValue(default_color_.blue());
  b_ref->setMaximumWidth(50);

  preview_ref = new QLabel;
  preview_ref->setFixedSize(20, 20);
  preview_ref->setStyleSheet(
      QString("background-color: %1; border: 1px solid gray;")
          .arg(default_color_.name()));

  layout->addWidget(color_label);
  layout->addWidget(new QLabel("R:"));
  layout->addWidget(r_ref);
  layout->addWidget(new QLabel("G:"));
  layout->addWidget(g_ref);
  layout->addWidget(new QLabel("B:"));
  layout->addWidget(b_ref);
  layout->addWidget(preview_ref);

  return container;
}

}  // namespace s21