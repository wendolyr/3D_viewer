#ifndef CPP4_3DVIEWER_V2_0_MODEL_FACADE_H
#define CPP4_3DVIEWER_V2_0_MODEL_FACADE_H

#include "file_manager.h"
#include "model.h"

namespace s21 {

class Facade {
 public:
  int ParseFile(std::string& file);
  void SaveModel(std::string& file_name);
  void LoadLastState();

  std::vector<Vertex>& GetVertices();
  std::set<std::pair<unsigned, unsigned>>& GetPolygons();
  Params GetCurrentSettings();

  void MoveFigure(Vertex shift);
  void ScaleFigure(double scale);
  void RotateFigure(Vertex angle);

  void ResetSettings();

 private:
  FigureModel model_;
  FileManager data_;
};

}  // namespace s21

#endif