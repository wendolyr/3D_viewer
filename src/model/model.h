#ifndef CPP4_3DVIEWER_V2_0_MODEL_MODEL_H
#define CPP4_3DVIEWER_V2_0_MODEL_MODEL_H

#include <unordered_set>
#include <vector>

#include "common_objects.h"
#include "factory.h"
#include "strategy.h"

namespace s21 {

class FigureModel {
 public:
  FigureModel();
  ~FigureModel();
  void SetVertices(std::vector<Vertex>& v);
  void SetPolygons(
      std::unordered_set<std::pair<unsigned, unsigned>, PairHash>& p);
  void SetSettings(Params& params);

  const std::vector<Vertex>& GetVertices() const;
  const std::unordered_set<std::pair<unsigned, unsigned>, PairHash>&
  GetPolygons() const;
  const Params GetCurrentSettings() const;

  void MoveFigure(Vertex shift);
  void ScaleFigure(float scale);
  void RotateFigure(Vertex angle);

  void ResetSettings();

 private:
  // std::vector<Vertex> start_;
  std::vector<Vertex> vertices_;
  std::unordered_set<std::pair<unsigned, unsigned>, PairHash> polygons_;

  std::unique_ptr<Context> strategy_;

  Vertex shift_;
  Vertex rotation_;
  float scale_;
};

}  // namespace s21

#endif