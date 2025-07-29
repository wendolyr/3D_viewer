#ifndef CPP4_3DVIEWER_V2_0_MODEL_STRATEGY_H
#define CPP4_3DVIEWER_V2_0_MODEL_STRATEGY_H

#include <vector>  //change

namespace s21 {

struct Point {
  double x, y, z;
};

class TransformStrategy {
 public:
  virtual ~TransformStrategy() = default;
  virtual void Transform(std::vector<Point> &vertex, double value) = 0;
};

class RotateModel : public TransformStrategy {
 public:
  void Transform(std::vector<Point> &vertex, double degree) override;
};

class ScaleModel : public TransformStrategy {
 public:
  void Transform(std::vector<Point> &vertex, double scale) override;
};

class MoveModel : public TransformStrategy {
 public:
  void Transform(std::vector<Point> &vertex, double abba) override;
};

class Strategy {
 private:
  TransformStrategy *stratege_;
};

}  // namespace s21
#endif