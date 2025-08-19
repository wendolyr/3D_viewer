#ifndef CPP4_3DVIEWER_V2_0_VIEW_BUILDER_COLOR_WIDGET_BUILDER_H
#define CPP4_3DVIEWER_V2_0_VIEW_BUILDER_COLOR_WIDGET_BUILDER_H

#include <QColor>
#include <QLabel>
#include <QSpinBox>
#include <QWidget>

namespace s21 {
class ColorWidgetBuilder {
 public:
  ColorWidgetBuilder& SetLabel(const QString& label);
  ColorWidgetBuilder& SetDefaultColor(const QColor& color);
  QWidget* Build(QSpinBox*& r_ref, QSpinBox*& g_ref, QSpinBox*& b_ref,
                 QLabel*& preview_ref);

 private:
  QString label_;
  QColor default_color_ = Qt::black;
};
}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_VIEW_BUILDER_COLOR_WIDGET_BUILDER_H