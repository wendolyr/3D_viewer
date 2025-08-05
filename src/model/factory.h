#ifndef CPP4_3DVIEWER_V2_0_MODEL_FACTORY_H
#define CPP4_3DVIEWER_V2_0_MODEL_FACTORY_H

#include "strategy.h"

namespace s21 {

class StrategyFactory {
 public:
  enum class StrategyType { kMove, kRotate, kScale };

  static TransformStrategy *CreateStrategy(StrategyType type) {
    if (type == StrategyType::kMove) {
      MoveStrategy *strategy = new MoveStrategy();
      return strategy;
    } else if (type == StrategyType::kRotate) {
      RotateStrategy *strategy = new RotateStrategy();
      return strategy;
    } else if (type == StrategyType::kScale) {
      ScaleStrategy *strategy = new ScaleStrategy();
      return strategy;
    }
  }
};

}  // namespace s21
#endif