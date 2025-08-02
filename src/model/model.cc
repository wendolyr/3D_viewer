#include "model.h"

namespace s21 {

FigureModel::FigureModel()
    : shift_{0.0f, 0.0f, 0.0f}, rotation_{0.0f, 0.0f, 0.0f}, scale_{1} {}

void FigureModel::MoveFigure(Vertex shift) {
  MoveStrategy *temp = new MoveStrategy();
  strategy_->SetStrategy(temp);
  strategy_->Transform(vertices_, shift);

  delete temp;
}

void FigureModel::ScaleFigure(double scale) {
  ScaleStrategy *temp = new ScaleStrategy();
  strategy_->SetStrategy(temp);
  strategy_->Transform(vertices_, {scale, 0.0, 0.0});

  delete temp;
}

void FigureModel::RotateFigure(Vertex angle) {
  RotateStrategy *temp = new RotateStrategy();

  strategy_->SetStrategy(temp);
  strategy_->Transform(vertices_, angle);

  delete temp;
}

void FigureModel::ResetParams() {
  MoveFigure({-shift_.x, -shift_.y, -shift_.z});
  RotateFigure({-rotation_.x, -rotation_.y, -rotation_.z});
  ScaleFigure(-scale_);
  scale_ = 0;
  shift_ = {0.0, 0.0, 0.0};
  rotation_ = {0.0, 0.0, 0.0};
}

}  // namespace s21