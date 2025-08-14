#include "strategy.h"

#include <iostream>

namespace s21 {

std::vector<std::vector<float>> TransformStrategy::MulSquareMatrix(
    std::vector<std::vector<float>> &matrix_one,
    std::vector<std::vector<float>> &matrix_two) {
  size_t n = matrix_one.size();
  std::vector<std::vector<float>> result(n, std::vector<float>(n, 0.0));
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      for (size_t k = 0; k < n; ++k) {
        result[i][j] += matrix_one[i][k] * matrix_two[k][j];
      }
    }
  }

  return result;
}

void ScaleStrategy::Transform(std::vector<std::vector<float>> &matrix,
                              const Vertex scale) {
  std::cout << "Scale\n";

  std::vector<std::vector<float>> scale_matrix(4, std::vector<float>(4, 0.0));

  for (size_t i = 0; i < scale_matrix.size() - 1; ++i) {
    scale_matrix[i][i] = scale.x;
  }

  scale_matrix[3][3] = 1;

  matrix = MulSquareMatrix(scale_matrix, matrix);
}

void MoveStrategy::Transform(std::vector<std::vector<float>> &matrix,
                             const Vertex axis) {
  std::cout << "Move\n";

  std::vector<std::vector<float>> move_matrix(4, std::vector<float>(4, 0.0));
  for (size_t i = 0; i < move_matrix.size(); ++i) {
    move_matrix[i][i] = 1;
  }

  move_matrix[0][3] = axis.x;
  move_matrix[1][3] = axis.y;
  move_matrix[2][3] = axis.z;

  matrix = MulSquareMatrix(move_matrix, matrix);
}

void RotateStrategy::Transform(std::vector<std::vector<float>> &matrix,
                               const Vertex axis) {
  float rx = axis.x * M_PI / 180;
  float ry = axis.y * M_PI / 180;
  float rz = axis.z * M_PI / 180;

  std::vector<std::vector<float>> Rx = {{1, 0, 0, 0},
                                        {0, cos(rx), -sin(rx), 0},
                                        {0, sin(rx), cos(rx), 0},
                                        {0, 0, 0, 1}};

  std::vector<std::vector<float>> Ry = {{cos(ry), 0, sin(ry), 0},
                                        {0, 1, 0, 0},
                                        {-sin(ry), 0, cos(ry), 0},
                                        {0, 0, 0, 1}};

  std::vector<std::vector<float>> Rz = {{cos(rz), -sin(rz), 0, 0},
                                        {sin(rz), cos(rz), 0, 0},
                                        {0, 0, 1, 0},
                                        {0, 0, 0, 1}};

  auto Rxy = MulSquareMatrix(Ry, Rx);
  std::vector<std::vector<float>> R = MulSquareMatrix(Rz, Rxy);
  matrix = MulSquareMatrix(R, matrix);
  // std::vector<std::vector<float>> rotate_matrix(4, std::vector<float>(4,
  // 0.0)); for (size_t i = 0; i < rotate_matrix.size(); ++i) {
  //   rotate_matrix[i][i] = 1;
  // }

  // float rad_angle_x = axis.x * M_PI / 180;
  // float rad_angle_y = axis.y * M_PI / 180;
  // float rad_angle_z = axis.z * M_PI / 180;

  // float sin_x = sin(rad_angle_x);
  // float cos_x = cos(rad_angle_x);

  // float sin_y = sin(rad_angle_y);
  // float cos_y = cos(rad_angle_y);

  // float sin_z = sin(rad_angle_z);
  // float cos_z = cos(rad_angle_z);

  // rotate_matrix[0][0] = (cos_y * cos_z);
  // rotate_matrix[0][1] = (sin_x * sin_y * cos_z - sin_z * cos_x);
  // rotate_matrix[0][2] = (sin_x * sin_z + cos_x * sin_y * cos_z);
  // rotate_matrix[1][0] = (cos_y * sin_z);
  // rotate_matrix[1][1] = (cos_x * cos_z + sin_x * sin_y * sin_z);
  // rotate_matrix[1][2] = (cos_x * sin_y * sin_z - sin_x * cos_z);
  // rotate_matrix[2][0] = (-sin_y);
  // rotate_matrix[2][1] = (sin_x * cos_y);
  // rotate_matrix[2][2] = (cos_x * cos_y);

  // matrix = MulSquareMatrix(rotate_matrix, matrix);
}

}  // namespace s21