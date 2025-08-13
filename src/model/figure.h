/**
 * @file figure.h
 * @brief Header file for FigureModel description
 */

#ifndef CPP4_3DVIEWER_V2_0_MODEL_FIGURE_H
#define CPP4_3DVIEWER_V2_0_MODEL_FIGURE_H

#include <unordered_set>
#include <vector>

#include "common_objects.h"
#include "factory.h"
#include "strategy.h"

namespace s21 {

/**
 * @class FigureModel
 * @brief Manages 3D model data and geometric transformations.
 *
 * This class stores vertex/polygon data and applies transformations
 * using the Strategy pattern for move/rotate/scale operations.
 */
class FigureModel {
 public:
  FigureModel();
  ~FigureModel();
  FigureModel& operator=(FigureModel&& other);

  /// Setter for model vertices
  void SetVertices(std::vector<Vertex>& v);

  /// Setter for model edges
  void SetEdges(std::unordered_set<std::pair<unsigned, unsigned>, PairHash>& p);

  /// Setter for model settings (shift, rotate, scale)
  void SetSettings(Params& params);

  /// Getter for model vertices
  const std::vector<Vertex>& GetVertices() const;

  /// Getter for model edges
  const std::unordered_set<std::pair<unsigned, unsigned>, PairHash>& GetEdges()
      const;

  /// Getter for model settings
  const Params GetCurrentSettings() const;

  /// Rotate figure method
  void MoveFigure(std::vector<std::vector<float>>& matrix, Vertex&& shift);

  /// Scale figure method
  void ScaleFigure(std::vector<std::vector<float>>& matrix, float scale);

  /// Rotate figure method
  void RotateFigure(std::vector<std::vector<float>>& matrix, Vertex&& angle);

  /// Resets all transformations to identity
  void ResetSettings();

 private:
  std::vector<Vertex> vertices_;  ///< Transformed vertices
  std::unordered_set<std::pair<unsigned, unsigned>, PairHash>
      edges_;  ///< Edges

  std::unique_ptr<Context> strategy_;  ///< Transformation strategy context

  Vertex shift_;     ///< Shift along three axes
  Vertex rotation_;  ///< Rotation along three axes
  float scale_;      ///< Current scale
};

}  // namespace s21

#endif