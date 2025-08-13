#include "strategy.h"

#include <iostream>

namespace s21 {

void ScaleStrategy::Transform(std::vector<std::vector<float>> &matrix,
                              const Vertex scale) {
  std::cout << "Scale\n";
  std::vector<std::vector<float>> temp(4, std::vector<float>(4, 0.0f));

  for (size_t i = 0; i < temp.size() - 1; ++i) {
    temp[i][i] = scale.x;
  }

  temp[3][3] = 1;

  matrix = std::move(temp);
}

void MoveStrategy::Transform(std::vector<std::vector<float>> &matrix,
                             const Vertex axis) {
  std::cout << "Move\n";

  std::vector<std::vector<float>> temp(4, std::vector<float>(4, 0.0f));
  for (size_t i = 0; i < temp.size(); ++i) {
    temp[i][i] = 1;
  }

  temp[0][3] = axis.x;
  temp[1][3] = axis.y;
  temp[2][3] = axis.z;

  matrix = std::move(temp);
}

void RotateStrategy::Transform(std::vector<std::vector<float>> &matrix,
                               const Vertex axis) {
  std::vector<std::vector<float>> temp(4, std::vector<float>(4, 0.0f));

  float rad_angle_x = axis.x * M_PI / 180;
  float rad_angle_y = axis.y * M_PI / 180;
  float rad_angle_z = axis.z * M_PI / 180;

  float sin_x = sin(rad_angle_x);
  float cos_x = cos(rad_angle_x);

  float sin_y = sin(rad_angle_y);
  float cos_y = cos(rad_angle_y);

  float sin_z = sin(rad_angle_z);
  float cos_z = cos(rad_angle_z);

  temp[0][0] = cos_y * cos_z;
  temp[0][1] = sin_x * sin_y * cos_z - sin_z * cos_x;
  temp[0][2] = sin_x * sin_z + cos_x * sin_y * cos_z;
  temp[1][0] = cos_y * sin_z;
  temp[1][1] = cos_x * cos_z + sin_x * sin_y * sin_z;
  temp[1][2] = cos_x * sin_y * sin_z - sin_x * cos_z;
  temp[2][0] = -sin_y;
  temp[2][1] = sin_x * cos_y;
  temp[2][2] = cos_x * cos_y;

  temp[3][3] = 1;

  matrix = std::move(temp);
}

}  // namespace s21