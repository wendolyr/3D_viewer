#ifndef CPP4_3DVIEWER_V2_0_VIEW_BUILDER_COLOR_WIDGET_BUILDER_H
#define CPP4_3DVIEWER_V2_0_VIEW_BUILDER_COLOR_WIDGET_BUILDER_H

#include <QColor>
#include <QLabel>
#include <QSpinBox>
#include <QWidget>

namespace s21 {
/**
 * @class ColorWidgetBuilder
 * @brief Строитель виджета выбора цвета с компонентами RGB
 *
 * Позволяет создавать составные виджеты для выбора цвета через спинбоксы RGB
 * с предпросмотром выбранного цвета.
 */
class ColorWidgetBuilder {
 public:
  /**
   * @brief Устанавливает текстовую метку для виджета
   * @param label Текст метки
   * @return Ссылка на текущий объект для цепочки вызовов
   */
  ColorWidgetBuilder& SetLabel(const QString& label);
  /**
   * @brief Устанавливает начальный цвет
   * @param color Начальный цвет (по умолчанию черный)
   * @return Ссылка на текущий объект для цепочки вызовов
   */
  ColorWidgetBuilder& SetDefaultColor(const QColor& color);
  /**
   * @brief Создает готовый виджет
   * @param[out] r_ref Ссылка на указатель спинбокса для красного компонента
   * @param[out] g_ref Ссылка на указатель спинбокса для зеленого компонента
   * @param[out] b_ref Ссылка на указатель спинбокса для синего компонента
   * @param[out] preview_ref Ссылка на указатель метки предпросмотра цвета
   * @return Указатель на созданный виджет-контейнер
   */
  QWidget* Build(QSpinBox*& r_ref, QSpinBox*& g_ref, QSpinBox*& b_ref,
                 QLabel*& preview_ref);

 private:
  QString label_;  ///< Текст метки для виджета
  QColor default_color_ = Qt::black;  ///< Начальный цвет по умолчанию
};
}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_VIEW_BUILDER_COLOR_WIDGET_BUILDER_H