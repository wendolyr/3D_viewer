#include "model.h"

namespace s21 {

FigureModel::FigureModel()
    : shift_{0.0, 0.0, 0.0}, rotation_{0.0, 0.0, 0.0}, scale_{1} {}

void FigureModel::MoveFigure(Vertex shift) {
  auto temp =
      StrategyFactory::CreateStrategy(StrategyFactory::StrategyType::kMove);
  strategy_->SetStrategy(temp);
  strategy_->Transform(vertices_, shift);

  delete temp;
}

void FigureModel::ScaleFigure(float scale) {
  auto temp =
      StrategyFactory::CreateStrategy(StrategyFactory::StrategyType::kScale);
  strategy_->SetStrategy(temp);
  strategy_->Transform(vertices_, {scale, 0.0, 0.0});

  delete temp;
}

void FigureModel::RotateFigure(Vertex angle) {
  auto temp =
      StrategyFactory::CreateStrategy(StrategyFactory::StrategyType::kRotate);
  strategy_->SetStrategy(temp);
  strategy_->Transform(vertices_, angle);

  delete temp;
}

void FigureModel::SetVertices(std::vector<Vertex> &v) { vertices_ = v; }

void FigureModel::SetPolygons(std::set<std::pair<unsigned, unsigned>> &p) {
  polygons_ = p;
}

const std::vector<Vertex> &FigureModel::GetVertices() const {
  return vertices_;
}

const std::set<std::pair<unsigned, unsigned>> &FigureModel::GetPolygons()
    const {
  return polygons_;
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
  /**
   * not right logic
   */
  MoveFigure({-shift_.x, -shift_.y, -shift_.z});
  RotateFigure({-rotation_.x, -rotation_.y, -rotation_.z});
  ScaleFigure(-scale_);
  scale_ = 0;
  shift_ = {0.0, 0.0, 0.0};
  rotation_ = {0.0, 0.0, 0.0};
}

}  // namespace s21