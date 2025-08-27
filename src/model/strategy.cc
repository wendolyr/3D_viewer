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
  std::vector<std::vector<float>> scale_matrix(4, std::vector<float>(4, 0.0));

  for (size_t i = 0; i < scale_matrix.size() - 1; ++i) {
    scale_matrix[i][i] = scale.x;
  }

  scale_matrix[3][3] = 1;

  matrix = MulSquareMatrix(scale_matrix, matrix);
}

void MoveStrategy::Transform(std::vector<std::vector<float>> &matrix,
                             const Vertex axis) {
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
  Quaternion new_rotation =
      Quaternion::FromEuler((axis.y) * M_PI / 180.0f, (axis.z) * M_PI / 180.0f,
                            (axis.x) * M_PI / 180.0f);

  rotation_quaternion_ = new_rotation * rotation_quaternion_;

  float norm = sqrt(rotation_quaternion_.x * rotation_quaternion_.x +
                    rotation_quaternion_.y * rotation_quaternion_.y +
                    rotation_quaternion_.z * rotation_quaternion_.z +
                    rotation_quaternion_.w * rotation_quaternion_.w);
  if (norm > 0.0f) {
    rotation_quaternion_.x /= norm;
    rotation_quaternion_.y /= norm;
    rotation_quaternion_.z /= norm;
    rotation_quaternion_.w /= norm;
  }

  std::vector<std::vector<float>> rotation_matrix =
      rotation_quaternion_.ToMatrix();

  matrix = MulSquareMatrix(rotation_matrix, matrix);
}

void RotateStrategy::ResetRotation() { rotation_quaternion_ = Quaternion(); }

void RotateStrategy::SetRotation(Quaternion &q) { rotation_quaternion_ = q; }

Quaternion RotateStrategy::GetRotation() { return rotation_quaternion_; }

}  // namespace s21