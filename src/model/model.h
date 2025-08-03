#ifndef CPP4_3DVIEWER_V2_0_MODEL_MODEL_H
#define CPP4_3DVIEWER_V2_0_MODEL_MODEL_H

#include <set>
#include <vector>

#include "common_objects.h"
#include "strategy.h"

namespace s21 {

class FigureModel {
 public:
  FigureModel();
  void SetVertices(std::vector<Vertex>& v);
  void SetPolygons(std::set<std::pair<unsigned, unsigned>>& p);
  void SetSettings(Params& params);
  std::vector<Vertex>& GetVertices();
  std::set<std::pair<unsigned, unsigned>>& GetPolygons();
  Params GetCurrentSettings();

  void MoveFigure(Vertex shift);
  void ScaleFigure(double scale);
  void RotateFigure(Vertex angle);

  void ResetSettings();

 private:
  // std::vector<Vertex> start_;
  std::vector<Vertex> vertices_;
  std::set<std::pair<unsigned, unsigned>> polygons_;

  Context* strategy_;

  Vertex shift_;
  Vertex rotation_;
  double scale_;
};

}  // namespace s21

#endif