#include "facade.h"

namespace s21 {

FileError Facade::ParseFile(const std::string& file) {
  return data_.ParseFile(file, model_);
}

void Facade::SaveModel(ViewParams& view_params) {
  data_.SaveModel(model_, view_params);
}

bool Facade::LoadLastState(ViewParams& view_params) {
  return data_.LoadLastState(model_, view_params);
}

const std::vector<Vertex>& Facade::GetVertices() const {
  return model_.GetVertices();
}

const std::unordered_set<std::pair<unsigned, unsigned>, PairHash>&
Facade::GetEdges() const {
  return model_.GetEdges();
}

const Params Facade::GetCurrentSettings() const {
  return model_.GetCurrentSettings();
}

void Facade::MoveFigure(std::vector<std::vector<float>>& matrix,
                        Vertex&& shift) {
  model_.MoveFigure(matrix, std::move(shift));
}

void Facade::ScaleFigure(std::vector<std::vector<float>>& matrix, float scale) {
  model_.ScaleFigure(matrix, scale);
}

void Facade::RotateFigure(std::vector<std::vector<float>>& matrix,
                          Vertex&& angle) {
  model_.RotateFigure(matrix, std::move(angle));
}

void Facade::ResetSettings() { model_.ResetSettings(); }

}  // namespace s21