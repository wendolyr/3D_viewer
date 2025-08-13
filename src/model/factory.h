/**
 * @file factory.h
 * @brief Header file for factory pattern members
 */

#ifndef CPP4_3DVIEWER_V2_0_MODEL_FACTORY_H
#define CPP4_3DVIEWER_V2_0_MODEL_FACTORY_H

#include "strategy.h"

namespace s21 {

/**
 * @class StrategyFactory
 * @brief Abstract base class for transformation strategy factories.
 */
class StrategyFactory {
 public:
  virtual std::unique_ptr<TransformStrategy> CreateStrategy() = 0;
};

/// Creates MoveStrategy instances for translation operations.
class MoveStrategyCreator : public StrategyFactory {
 public:
  std::unique_ptr<TransformStrategy> CreateStrategy() override {
    return std::make_unique<MoveStrategy>();
  }
};

/// Creates RotateStrategy instances for rotation operations.
class RotateStrategyCreator : public StrategyFactory {
 public:
  std::unique_ptr<TransformStrategy> CreateStrategy() override {
    return std::make_unique<RotateStrategy>();
  }
};

/// Creates ScaleStrategy instances for scaling operations.
class ScaleStrategyCreator : public StrategyFactory {
 public:
  std::unique_ptr<TransformStrategy> CreateStrategy() override {
    return std::make_unique<ScaleStrategy>();
  }
};

}  // namespace s21
#endif