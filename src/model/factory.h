#ifndef CPP4_3DVIEWER_V2_0_MODEL_FACTORY_H
#define CPP4_3DVIEWER_V2_0_MODEL_FACTORY_H

#include "strategy.h"

namespace s21 {

class StrategyFactory {
 public:
  virtual TransformStrategy *CreateStrategy() = 0;
};

class MoveStrategyCreator : public StrategyFactory {
 public:
  TransformStrategy *CreateStrategy() override { return new MoveStrategy; }
};

class RotateStrategyCreator : public StrategyFactory {
 public:
  TransformStrategy *CreateStrategy() override { return new RotateStrategy; }
};

class ScaleStrategyCreator : public StrategyFactory {
 public:
  TransformStrategy *CreateStrategy() override { return new ScaleStrategy; }
};

}  // namespace s21
#endif