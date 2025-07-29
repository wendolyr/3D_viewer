#include "strategy.h"

namespace s21 {

void ScaleModel::Transform(std::vector<Point> &vertex, double scale) {
  for (Point i : vertex) {
    i.x *= scale;
    i.y *= scale;
    i.z *= scale;
  }
}

}  // namespace s21