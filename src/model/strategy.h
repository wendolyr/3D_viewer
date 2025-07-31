#ifndef CPP4_3DVIEWER_V2_0_MODEL_STRATEGY_H
#define CPP4_3DVIEWER_V2_0_MODEL_STRATEGY_H

#include <vector>

#include "common_objects.h"

namespace s21 {

class TransformStrategy {
 public:
  virtual ~TransformStrategy() = default;
  virtual void Transform(std::vector<Vertex> &vertices, const Vertex value) = 0;
};

class RotateStrategy : public TransformStrategy {
 public:
  void Transform(std::vector<Vertex> &vertices, const Vertex angles) override;
};

class ScaleStrategy : public TransformStrategy {
 public:
  void Transform(std::vector<Vertex> &vertices, const Vertex scale) override;
};

class MoveStrategy : public TransformStrategy {
 public:
  void Transform(std::vector<Vertex> &vertices, const Vertex shift) override;
};

class Context {
 public:
  Context() {};
  void SetStrategy(TransformStrategy *s) { strategy_ = s; }

  void Transform(std::vector<Vertex> &vertices, const Vertex param) {
    if (strategy_) {
      strategy_->Transform(vertices, param);
    }
  }

 private:
  TransformStrategy *strategy_;
};

}  // namespace s21
#endif