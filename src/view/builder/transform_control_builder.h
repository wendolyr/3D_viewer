#ifndef CPP4_3DVIEWER_V2_0_VIEW_BUILDER_TRANSFORM_CONTROL_BUILDER_H
#define CPP4_3DVIEWER_V2_0_VIEW_BUILDER_TRANSFORM_CONTROL_BUILDER_H

#include <QFormLayout>
#include <QGroupBox>

#include "../enum.h"
#include "template_axis_control_builder.h"

namespace s21 {
/**
 * @class TransformBuilder
 * @brief Строитель группы элементов управления преобразованиями
 * @tparam SpinBoxType Тип спинбокса для управления
 *
 * Создает группу с элементами управления для перемещения, поворота или
 * масштабирования с поддержкой нескольких осей.
 */
template <typename SpinBoxType>
class TransformBuilder {
 public:
  /**
   * @brief Конструктор с указанием типа преобразования
   * @param type Тип преобразования (Move/Rotate/Scale)
   */
  explicit TransformBuilder(TransformType type);
  /**
   * @brief Добавляет ось управления
   * @param axis Ось управления (X/Y/Z/None)
   * @param step Шаг изменения значения
   * @param[out] widgetRef Ссылка на указатель созданного виджета
   * @param minusHandler Обработчик кнопки уменьшения
   * @param plusHandler Обработчик кнопки увеличения
   */
  TransformBuilder& AddAxis(Axis axis, double step, SpinBoxType*& widgetRef,
                            std::function<void()> minusHandler = nullptr,
                            std::function<void()> plusHandler = nullptr);
  /**
   * @brief Завершает создание группы элементов управления
   * @return Готовая группа элементов управления
   */
  QGroupBox* Build() { return group_; }

 private:
  /**
   * @brief Получает заголовок группы в зависимости от типа преобразования
   * @return Локализованный заголовок группы
   */
  QString GetTitle() const;
  /**
   * @brief Получает метку оси в зависимости от типа оси
   * @param axis Ось для получения метки
   * @return Локализованная метка оси
   */
  QString GetAxisLabel(Axis axis) const;
  /**
   * @brief Получает минимальное значение в зависимости от типа преобразования
   * @return Минимальное значение для спинбокса
   */
  double GetMinValue() const;
  /**
   * @brief Получает максимальное значение в зависимости от типа преобразования
   * @return Максимальное значение для спинбокса
   */
  double GetMaxValue() const;
  /**
   * @brief Получает точность отображения в зависимости от типа преобразования
   * @return Количество знаков после запятой
   */
  int GetDecimals() const;

  TransformType
      type_;  ///< Тип преобразования (перемещение, поворот, масштабирование)
  QGroupBox* group_;  ///< Указатель на группу элементов управления
  QFormLayout* layout_;  ///< Указатель на layout группы
};
}  // namespace s21

#include "transform_control_builder.tpp"

#endif  // CPP4_3DVIEWER_V2_0_VIEW_BUILDER_TRANSFORM_CONTROL_BUILDER_H