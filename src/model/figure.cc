#include "figure.h"

#include <iostream>

namespace s21 {

FigureModel::FigureModel()
    : shift_{0.0, 0.0, 0.0},
      rotation_{0.0, 0.0, 0.0},
      scale_{1.0},
      rotation_quaternion_() {
  strategy_ = std::make_unique<Context>();
}

FigureModel::~FigureModel() {}

FigureModel &FigureModel::operator=(FigureModel &&other) {
  vertices_ = std::move(other.vertices_);
  edges_ = std::move(other.edges_);
  strategy_ = std::move(other.strategy_);
  shift_ = std::move(other.shift_);
  rotation_ = std::move(other.rotation_);
  scale_ = std::move(other.scale_);

  return *this;
}

void FigureModel::MoveFigure(std::vector<std::vector<float>> &matrix,
                             Vertex &&shift) {
  MoveStrategyCreator creator;
  auto temp = creator.CreateStrategy();
  strategy_->SetStrategy(std::move(temp));

  strategy_->Transform(matrix, shift);

  shift_ = shift;
}

void FigureModel::ScaleFigure(std::vector<std::vector<float>> &matrix,
                              float scale) {
  ScaleStrategyCreator creator;
  auto temp = creator.CreateStrategy();
  strategy_->SetStrategy(std::move(temp));

  strategy_->Transform(matrix, {scale, 0.0, 0.0});

  scale_ = scale;
}

void FigureModel::RotateFigure(std::vector<std::vector<float>> &matrix,
                               Vertex &&angle) {
  // RotateStrategyCreator creator;
  // auto temp = creator.CreateStrategy();
  // strategy_->SetStrategy(std::move(temp));
  // strategy_->Transform(matrix, angle);

  Quaternion new_rotation =
      Quaternion::FromEuler((angle.y - rotation_.y) * M_PI / 180.0f,
                            (angle.z - rotation_.z) * M_PI / 180.0f,
                            (angle.x - rotation_.x) * M_PI / 180.0f);

  rotation_quaternion_ = new_rotation * rotation_quaternion_;
  std::vector<std::vector<float>> rotation_matrix =
      rotation_quaternion_.ToMatrix();

  // Применяем матрицу вращения
  matrix = TransformStrategy::MulSquareMatrix(rotation_matrix, matrix);

  rotation_ = angle;
}

void FigureModel::SetVertices(std::vector<Vertex> &v) { vertices_ = v; }

void FigureModel::SetEdges(
    std::unordered_set<std::pair<unsigned, unsigned>, PairHash> &p) {
  edges_ = p;
}

const std::vector<Vertex> &FigureModel::GetVertices() const {
  return vertices_;
}

const std::unordered_set<std::pair<unsigned, unsigned>, PairHash> &
FigureModel::GetEdges() const {
  return edges_;
}

const Params FigureModel::GetCurrentSettings() const {
  return {shift_, rotation_, scale_};
}

void FigureModel::SetSettings(Params &params) {
  shift_ = params.shift;
  rotation_ = params.rotation;
  scale_ = params.scale;
  rotation_quaternion_ = Quaternion();
}

void FigureModel::ResetSettings() {
  scale_ = 1.0;
  shift_ = {0.0, 0.0, 0.0};
  rotation_ = {0.0, 0.0, 0.0};
  rotation_quaternion_ = Quaternion();
}

}  // namespace s21