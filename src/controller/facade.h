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

  /// Parses a 3D object file and initializes the model.
  FileError ParseFile(const std::string& file);

  /// Saves the current model state and view settings to files.
  void SaveModel(ViewParams& view_params);

  /// Loads the last saved model state and view settings. Returns true on
  /// success.
  bool LoadLastState(ViewParams& view_params);

  /// Returns a reference to the model's vertex data.
  const std::vector<Vertex>& GetVertices() const;

  /// Returns a reference to the model's polygon edge indices.
  const std::unordered_set<std::pair<unsigned, unsigned>, PairHash>& GetEdges()
      const;

  /// Returns current transformation parameters (shift, rotation, scale).
  const Params GetCurrentSettings() const;

  /// Translates the model by the specified offset.
  void MoveFigure(std::vector<std::vector<float>>& matrix, Vertex&& shift);

  /// Scales the model by the given factor.
  void ScaleFigure(std::vector<std::vector<float>>& matrix, float scale);

  /// Rotates the model by the specified angles (in degrees).
  void RotateFigure(std::vector<std::vector<float>>& matrix, Vertex&& angle);

  /// Resets all transformations to initial state.
  void ResetSettings();

 private:
  FigureModel model_;  ///< Handles model data and transformations
  FileManager data_;   ///< Manages file I/O operations
};

}  // namespace s21

#endif