/**
 * @file facade.h
 * @brief Header file for facade strategy
 */

#ifndef CPP4_3DVIEWER_V2_0_MODEL_FACADE_H
#define CPP4_3DVIEWER_V2_0_MODEL_FACADE_H

#include "../model/figure.h"
#include "../model/file_manager.h"

namespace s21 {

/**
 * @class Facade
 * @brief Provides a simplified interface to the 3D model manipulation system.
 *
 * The Facade class encapsulates complex interactions with the FigureModel and
 * FileManager, offering a unified API for parsing files, saving/loading models,
 * transforming figures, and accessing model data.
 */
class Facade {
 public:
  Facade() = default;
  ~Facade() = default;

  /**
   * @brief Parses a 3D object file and initializes the model
   * @param file - name of the object file
   * @returns FileError status
   */
  FileError ParseFile(const std::string& file);

  /**
   * @brief Saves the current model state and view settings to files
   * @param view_param - current visual settings (colors, thickness etc)
   */
  void SaveModel(ViewParams& view_params);

  /**
   * @brief Loads the last saved model state and view settings
   * @param view_param - current visual settings (colors, thickness etc)
   * @returns True if loaded successfully and false otherwise
   */
  bool LoadLastState(ViewParams& view_params);

  /**
   * @brief Getter for vertices
   * @returns Constant reference to the model vertices
   */
  const std::vector<Vertex>& GetVertices() const;

  /**
   * @brief Getter for edges
   * @returns Constant reference to the model edges
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
  FigureModel model_;  ///< Handles model data and transformations
  FileManager data_;   ///< Manages file I/O operations
};

}  // namespace s21

#endif