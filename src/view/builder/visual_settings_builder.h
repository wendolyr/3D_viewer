#ifndef CPP4_3DVIEWER_V2_0_VIEW_BUILDER_VISUAL_SETTINGS_BUILDER_H
#define CPP4_3DVIEWER_V2_0_VIEW_BUILDER_VISUAL_SETTINGS_BUILDER_H

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
/**
 * @class VisualSettingsBuilder
 * @brief Строитель группы визуальных настроек
 *
 * Позволяет создавать группы настроек с комбинированными списками,
 * элементами выбора цвета и числовыми полями ввода.
 */
class VisualSettingsBuilder {
 public:
  /**
   * @brief Конструктор по умолчанию
   */
  explicit VisualSettingsBuilder();
  /**
   * @brief Добавляет комбинированный список
   * @param label Текст метки
   * @param[out] combo_ref Ссылка на указатель созданного комбобокса
   * @param items Список элементов комбобокса
   */
  VisualSettingsBuilder& AddComboBox(const QString& label,
                                     QComboBox*& combo_ref,
                                     const QStringList& items = {});
  /**
   * @brief Добавляет виджет выбора цвета
   * @param label Текст метки
   * @param[out] r_ref Ссылка на указатель спинбокса для красного компонента
   * @param[out] g_ref Ссылка на указатель спинбокса для зеленого компонента
   * @param[out] b_ref Ссылка на указатель спинбокса для синего компонента
   * @param[out] preview_ref Ссылка на указатель метки предпросмотра
   * @param default_color Начальный цвет
   */
  VisualSettingsBuilder& AddColorWidget(
      const QString& label, QSpinBox*& r_ref, QSpinBox*& g_ref,
      QSpinBox*& b_ref, QLabel*& preview_ref,
      const QColor& default_color = Qt::black);
  /**
   * @brief Добавляет числовое поле ввода
   * @param label Текст метки
   * @param[out] spin_ref Ссылка на указатель созданного спинбокса
   * @param min Минимальное значение
   * @param max Максимальное значение
   * @param step Шаг изменения
   * @param decimals Точность отображения
   */
  VisualSettingsBuilder& AddDoubleSpinBox(const QString& label,
                                          QDoubleSpinBox*& spin_ref,
                                          double min = 0.0, double max = 10.0,
                                          double step = 1.0, int decimals = 2);
  /**
   * @brief Устанавливает отступы layout'а
   */
  VisualSettingsBuilder& SetMargins(int left = 0, int top = 0, int right = 0,
                                    int bottom = 0);
  /**
   * @brief Устанавливает интервал между элементами
   * @param spacing Величина интервала
   */
  VisualSettingsBuilder& SetSpacing(int spacing = 0);
  /**
   * @brief Завершает создание группы настроек
   * @param title Заголовок группы настроек
   * @return Готовая группа настроек
   */
  QGroupBox* Build(const QString& title);

 private:
  QVBoxLayout* main_layout_;  ///< Основной layout группы
  QGroupBox* group_box_;  ///< Указатель на группу настроек
};
}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_VIEW_BUILDER_VISUAL_SETTINGS_BUILDER_H