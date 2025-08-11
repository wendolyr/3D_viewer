#ifndef CPP4_3DVIEWER_V2_0_MODEL_FACADE_H
#define CPP4_3DVIEWER_V2_0_MODEL_FACADE_H

#include "../model/figure.h"
#include "../model/file_manager.h"

namespace s21 {

class Facade {
 public:
  Facade() = default;
  ~Facade() = default;

  FileError ParseFile(const std::string& file);
  void SaveModel(ViewParams& view_params);
  void LoadLastState(ViewParams& view_params);

  const std::vector<Vertex>& GetVertices() const;
  const std::unordered_set<std::pair<unsigned, unsigned>, PairHash>&
  GetPolygons() const;
  const Params GetCurrentSettings() const;

  void MoveFigure(Vertex&& shift);
  void ScaleFigure(float scale);
  void RotateFigure(Vertex&& angle);

  void ResetSettings();

 private:
  FigureModel model_;
  FileManager data_;
};

}  // namespace s21

#endif