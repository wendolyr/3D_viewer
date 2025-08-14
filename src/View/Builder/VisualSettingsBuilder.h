#ifndef CPP4_3DVIEWER_V2_0_VIEW_BUILDER_VISUALSETTINGSBUILDER_H
#define CPP4_3DVIEWER_V2_0_VIEW_BUILDER_VISUALSETTINGSBUILDER_H

#include <QColor>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QList>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

namespace s21 {
class VisualSettingsBuilder {
 public:
  explicit VisualSettingsBuilder();

  VisualSettingsBuilder& AddComboBox(const QString& label,
                                     QComboBox*& combo_ref,
                                     const QStringList& items = {});
  VisualSettingsBuilder& AddColorWidget(
      const QString& label, QSpinBox*& r_ref, QSpinBox*& g_ref,
      QSpinBox*& b_ref, QLabel*& preview_ref,
      const QColor& default_color = Qt::black);
  VisualSettingsBuilder& AddDoubleSpinBox(const QString& label,
                                          QDoubleSpinBox*& spin_ref,
                                          double min = 0.0, double max = 10.0,
                                          double step = 1.0, int decimals = 2);

  QGroupBox* Build(const QString& title);

  VisualSettingsBuilder& SetMargins(int left = 0, int top = 0, int right = 0,
                                    int bottom = 0);
  VisualSettingsBuilder& SetSpacing(int spacing = 0);

 private:
  QVBoxLayout* main_layout_;
  QGroupBox* group_box_;
};
}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_VIEW_BUILDER_VISUALSETTINGSBUILDER_H