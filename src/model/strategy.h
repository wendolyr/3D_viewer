/**
 * @file strategy.h
 * @brief Header file for strategy pattern members
 */

#ifndef CPP4_3DVIEWER_V2_0_MODEL_STRATEGY_H
#define CPP4_3DVIEWER_V2_0_MODEL_STRATEGY_H

#include <cmath>
#include <memory>
#include <vector>

#include "common_objects.h"

namespace s21 {

/**
 * @class TransformStrategy
 * @brief Abstract base for geometric transformation strategies.
 */
class TransformStrategy {
 public:
  virtual ~TransformStrategy() = default;

  /**
   * @brief Applies transformation to vertices using the parameter value
   */
  virtual void Transform(std::vector<std::vector<float>> &matrix,
                         const Vertex value) = 0;

  /**
   * @brief Method for multiplying two matrices
   */
  static std::vector<std::vector<float>> MulSquareMatrix(
      std::vector<std::vector<float>> &matrix_one,
      std::vector<std::vector<float>> &matrix_two);
};

/**
 * @brief Strategy for rotation transformations
 */
class RotateStrategy : public TransformStrategy {
 public:
  void Transform(std::vector<std::vector<float>> &matrix,
                 const Vertex angles) override;

  /**
   * @brief Set the quaternion to its default value
   */
  static void ResetRotation();

  /**
   * @brief Set the quaternion to a specific value
   */
  static void SetRotation(Quaternion &q);

  /**
   * @brief Get current quaternion
   */
  static Quaternion GetRotation();

 private:
  inline static Quaternion rotation_quaternion_ =
      Quaternion();  ///< Accumulate all rotations
};

/**
 * @brief Strategy for uniform scaling transformations
 */
class ScaleStrategy : public TransformStrategy {
 public:
  void Transform(std::vector<std::vector<float>> &matrix,
                 const Vertex scale) override;
};

/**
 * @brief Strategy for translation transformations
 */
class MoveStrategy : public TransformStrategy {
 public:
  void Transform(std::vector<std::vector<float>> &matrix,
                 const Vertex shift) override;
};

/**
 * @class Context
 * @brief Executes transformations using the current strategy.
 *
 * Implements the Strategy pattern context which delegates
 * transformation tasks to concrete strategy objects.
 */
class Context {
 public:
  Context() {};

  /**
   * @brief Sets the active transformation strategy
   */
  void SetStrategy(std::unique_ptr<TransformStrategy> &&s) {
    strategy_ = std::move(s);
  }

  /**
   * @brief Applies transformation using the current strategy
   */
  void Transform(std::vector<std::vector<float>> &matrix, const Vertex param) {
    if (strategy_) {
      strategy_->Transform(matrix, param);
    }
  }

 private:
  std::unique_ptr<TransformStrategy> strategy_;  ///< current strategy
};

}  // namespace s21
#endif