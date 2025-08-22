#ifndef CPP4_3DVIEWER_V2_0_VIEW_ENUM_H
#define CPP4_3DVIEWER_V2_0_VIEW_ENUM_H

namespace s21 {
/**
 * @enum TransformType
 * @brief Типы преобразований 3D-модели
 */
enum class TransformType {
  kMove,    ///< Перемещение модели
  kRotate,  ///< Вращение модели
  kScale    ///< Масштабирование модели
};
/**
 * @enum Axis
 * @brief Оси координат для преобразований
 */
enum class Axis {
  kX,    ///< Ось X
  kY,    ///< Ось Y
  kZ,    ///< Ось Z
  kNone  ///< Без конкретной оси (для равномерного масштабирования)
};
}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_VIEW_ENUM_H