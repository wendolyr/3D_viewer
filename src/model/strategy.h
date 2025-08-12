/**
 * @file strategy.h
 * @brief Header file for strategy pattern members
 */

#ifndef CPP4_3DVIEWER_V2_0_MODEL_STRATEGY_H
#define CPP4_3DVIEWER_V2_0_MODEL_STRATEGY_H

#include <cmath>
#include <thread>
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

  /// Applies transformation to vertices using the parameter value
  virtual void Transform(std::vector<Vertex> &vertices, const Vertex value) = 0;

  /**
   * @brief Applies a transformation function to a vector of vertices in
   * parallel using multiple threads
   *
   * @param vertices - Reference to a vector of vertices to be transformed
   * @param worker - Lambda that implements the actual transformation
   *
   * @note For small vector, the overhead of thread creation might outweigh
   * parallelization benefits
   */

  void ParallelTransform(std::vector<Vertex> &vertices,
                         const std::function<void(size_t, size_t)> &worker);
};

/**
 * @brief Strategy for rotation transformations
 */
class RotateStrategy : public TransformStrategy {
 public:
  void Transform(std::vector<Vertex> &vertices, const Vertex angles) override;
};

/**
 * @brief Strategy for uniform scaling transformations
 */
class ScaleStrategy : public TransformStrategy {
 public:
  void Transform(std::vector<Vertex> &vertices, const Vertex scale) override;
};

/**
 * @brief Strategy for translation transformations
 */
class MoveStrategy : public TransformStrategy {
 public:
  void Transform(std::vector<Vertex> &vertices, const Vertex shift) override;
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
  void Transform(std::vector<Vertex> &vertices, const Vertex param) {
    if (strategy_) {
      strategy_->Transform(vertices, param);
    }
  }

 private:
  std::unique_ptr<TransformStrategy> strategy_;  ///< current strategy
};

}  // namespace s21
#endif