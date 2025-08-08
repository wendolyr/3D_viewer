#ifndef CPP4_3DVIEWER_V2_0_MODEL_FACTORY_H
#define CPP4_3DVIEWER_V2_0_MODEL_FACTORY_H

#include "strategy.h"

namespace s21 {

class StrategyFactory {
 public:
  virtual std::unique_ptr<TransformStrategy> CreateStrategy() = 0;
};

class MoveStrategyCreator : public StrategyFactory {
 public:
  std::unique_ptr<TransformStrategy> CreateStrategy() override {
    return std::make_unique<MoveStrategy>();
  }
};

class RotateStrategyCreator : public StrategyFactory {
 public:
  std::unique_ptr<TransformStrategy> CreateStrategy() override {
    return std::make_unique<RotateStrategy>();
  }
};

class ScaleStrategyCreator : public StrategyFactory {
 public:
  std::unique_ptr<TransformStrategy> CreateStrategy() override {
    return std::make_unique<ScaleStrategy>();
  }
};

}  // namespace s21
#endif