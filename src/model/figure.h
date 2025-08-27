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
  /**
   * @brief Model constructor
   */
  FigureModel();

  /**
   * @brief Model destructor
   */
  ~FigureModel();

  /**
   * @brief overload operator "=" for FigureModel
   */
  FigureModel& operator=(FigureModel&& other);

  /**
   * @brief Setter for model vertices
   * @param v Reference to the parsed model vertices
   */
  void SetVertices(std::vector<Vertex>& v);

  /**
   * @brief Setter for model edges
   * @param p Reference to the parsed model edges
   */
  void SetEdges(std::unordered_set<std::pair<unsigned, unsigned>, PairHash>& p);

  /**
   * @brief Setter for model settings
   * @param params A structure containing current shift, rotate, scale and
   * quaternion
   */
  void SetSettings(Params& params);

  /**
   * @brief Getter for model vertices
   * @returns Vector of the model vertices
   */
  const std::vector<Vertex>& GetVertices() const;

  /**
   * @brief Getter for model edges
   * @returns Unordered set of the model edges
   */
  const std::unordered_set<std::pair<unsigned, unsigned>, PairHash>& GetEdges()
      const;

  /**
   * @brief Getter for current settings
   * @returns Params struct which contains shift, rotate, scale, quaternion
   */
  const Params GetCurrentSettings() const;

  /**
   * @brief Calculates a 4x4 representation matrix for the current axis offset
   * @param matrix - reference to the current representation matrix
   * @param shift - current offset in x, y, z
   * @warning matrix must be 4x4!
   */
  void MoveFigure(std::vector<std::vector<float>>& matrix, Vertex&& shift);

  /**
   * @brief Calculates a 4x4 representation matrix for the current scale
   * @param matrix - reference to the current representation matrix
   * @param scale - current scale factor
   * @warning matrix must be 4x4!
   */
  void ScaleFigure(std::vector<std::vector<float>>& matrix, float scale);

  /**
   * @brief Rotates the model by the specified angles (in degrees)
   * @param matrix - reference to the current representation matrix
   * @param angle - current rotation angles along the x, y, z axes
   * @warning matrix must be 4x4!
   */
  void RotateFigure(std::vector<std::vector<float>>& matrix, Vertex&& angle);

  /**
   * @brief Resets all transformations to initial state
   */
  void ResetSettings();

 private:
  std::vector<Vertex> vertices_;  ///< Transformed vertices
  std::unordered_set<std::pair<unsigned, unsigned>, PairHash>
      edges_;  ///< Edges

  std::unique_ptr<Context> strategy_;  ///< Transformation strategy context

  Vertex shift_;     ///< Shift along three axes
  Vertex rotation_;  ///< Rotation along three axes (Euler)
  float scale_;      ///< Current scale
};

}  // namespace s21

#endif