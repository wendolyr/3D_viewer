#include "facade.h"

namespace s21 {

int Facade::ParseFile(std::string& file) {
  return data_.ParseFile(file, model_);
}

void Facade::SaveModel(std::string& file_name) {
  data_.SaveModel(file_name, model_);
}

void Facade::LoadLastState() { data_.LoadLastState(model_); }

std::vector<Vertex>& Facade::GetVertices() { return model_.GetVertices(); }

std::set<std::pair<unsigned, unsigned>>& Facade::GetPolygons() {
  return model_.GetPolygons();
}

Params Facade::GetCurrentSettings() { return model_.GetCurrentSettings(); }

void Facade::MoveFigure(Vertex shift) { model_.MoveFigure(shift); }

void Facade::ScaleFigure(double scale) { model_.ScaleFigure(scale); }

void Facade::RotateFigure(Vertex angle) { model_.RotateFigure(angle); }

void Facade::ResetSettings() { model_.ResetSettings(); }

}  // namespace s21