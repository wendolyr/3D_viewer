#include "figure.h"

namespace s21 {

FigureModel::FigureModel()
    : shift_{0.0, 0.0, 0.0}, rotation_{0.0, 0.0, 0.0}, scale_{1} {
  strategy_ = std::make_unique<Context>();
}

FigureModel::~FigureModel() {}

FigureModel &FigureModel::operator=(FigureModel &&other) {
  original_ = std::move(other.original_);
  vertices_ = std::move(other.vertices_);
  edges_ = std::move(other.edges_);
  strategy_ = std::move(other.strategy_);
  shift_ = std::move(other.shift_);
  rotation_ = std::move(other.rotation_);
  scale_ = std::move(other.scale_);

  return *this;
}

void FigureModel::MoveFigure(Vertex &&shift) {
  shift_ += shift;
  MoveStrategyCreator creator;
  auto temp = creator.CreateStrategy();
  strategy_->SetStrategy(std::move(temp));
  strategy_->Transform(vertices_, shift);
}

void FigureModel::ScaleFigure(float scale) {
  scale_ = scale;
  ScaleStrategyCreator creator;
  auto temp = creator.CreateStrategy();
  strategy_->SetStrategy(std::move(temp));

  std::vector<Vertex> orig = original_;
  strategy_->Transform(orig, {scale, 0.0, 0.0});
  vertices_ = orig;
}

void FigureModel::RotateFigure(Vertex &&angle) {
  rotation_ += angle;
  RotateStrategyCreator creator;
  auto temp = creator.CreateStrategy();
  strategy_->SetStrategy(std::move(temp));
  strategy_->Transform(vertices_, angle);
}

void FigureModel::SetVertices(std::vector<Vertex> &v) {
  original_ = v;
  vertices_ = v;
}

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
}

void FigureModel::ResetSettings() {
  vertices_ = original_;
  scale_ = 0;
  shift_ = {0.0, 0.0, 0.0};
  rotation_ = {0.0, 0.0, 0.0};
}

}  // namespace s21