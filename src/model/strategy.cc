#include "strategy.h"

namespace s21 {

void ScaleStrategy::Transform(std::vector<Vertex> &vertices,
                              const Vertex scale) {
  for (Vertex i : vertices) {
    i.x *= scale.x;
    i.y *= scale.x;
    i.z *= scale.x;
  }
}

void MoveStrategy::Transform(std::vector<Vertex> &vertices, const Vertex axis) {
  for (Vertex i : vertices) {
    i.x += axis.x;
    i.y += axis.y;
    i.z += axis.z;
  }
}

void RotateStrategy::Transform(std::vector<Vertex> &vertices,
                               const Vertex axis) {
  double rad_angle_x = axis.x * M_PI / 180;
  double rad_angle_y = axis.y * M_PI / 180;
  double rad_angle_z = axis.z * M_PI / 180;

  double calc_sin_x = sin(rad_angle_x);
  double calc_cos_x = cos(rad_angle_x);

  double calc_sin_y = sin(rad_angle_y);
  double calc_cos_y = cos(rad_angle_y);

  double calc_sin_z = sin(rad_angle_z);
  double calc_cos_z = cos(rad_angle_z);

  for (Vertex i : vertices) {
    if (rad_angle_x > 1e-7) {
      i.y = i.y * calc_cos_x - i.z * calc_sin_x;
      i.z = i.y * calc_sin_x + i.z * calc_cos_x;
    } else if (rad_angle_y > 1e-7) {
      i.x = i.x * calc_cos_y + i.z * calc_sin_y;
      i.z = -i.x * calc_sin_y + i.z * calc_cos_y;
    } else {
      i.x = i.x * calc_cos_z - i.y * calc_sin_z;
      i.y = i.x * calc_sin_z + i.y * calc_cos_z;
    }
  }
}

}  // namespace s21