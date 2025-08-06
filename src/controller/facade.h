#ifndef CPP4_3DVIEWER_V2_0_MODEL_FACADE_H
#define CPP4_3DVIEWER_V2_0_MODEL_FACADE_H

#include "../model/file_manager.h"
#include "../model/model.h"

namespace s21 {

class Facade {
 public:
  Facade() = default;
  ~Facade() = default;

  int ParseFile(std::string& file);
  void SaveModel(std::string& file_name);
  void LoadLastState();

  const std::vector<Vertex>& GetVertices() const;
  const std::set<std::pair<unsigned, unsigned>>& GetPolygons() const;
  const Params GetCurrentSettings() const;

  void MoveFigure(Vertex shift);
  void ScaleFigure(float scale);
  void RotateFigure(Vertex angle);

  void ResetSettings();

 private:
  FigureModel model_;
  FileManager data_;
};

}  // namespace s21

#endif