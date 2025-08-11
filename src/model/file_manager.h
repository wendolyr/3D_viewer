/**
 * @file file_manager.h
 * @brief Header file for FileManager description
 */

#ifndef CPP4_3DVIEWER_V2_0_MODEL_PARSER_H
#define CPP4_3DVIEWER_V2_0_MODEL_PARSER_H

#include <charconv>
#include <fstream>
#include <sstream>
#include <string>

#include "figure.h"

namespace s21 {

/**
 * @class FileManager
 * @brief Handles file I/O operations for 3D models and settings.
 *
 * Responsible for:
 *   - Parsing .obj files into model data
 *   - Saving/Loading model states and view configurations
 */
class FileManager {
 public:
  /**
   * @brief Parses a .obj file into FigureModel
   * @returns FileError status.
   */
  FileError ParseFile(const std::string &file_name, FigureModel &model);

  /// Saves model and view parameters to disk.
  void SaveModel(FigureModel &model, ViewParams &view_params);

  /// Restores the last saved model state and view settings
  bool LoadLastState(FigureModel &model, ViewParams &view_params);

 private:
  // Helper methods for parsing OBJ file components
  FileError ParseVertices(const char *ptr, std::vector<Vertex> &vertices);
  FileError ParsePolygons(
      const char *ptr, std::vector<Vertex> &vertices,
      std::unordered_set<std::pair<unsigned, unsigned>, PairHash> &polygons);

  // Settings persistence
  void SaveSettings(FigureModel &model, ViewParams &view_params);
  bool LoadSettings(FigureModel &model, ViewParams &view_params);
  bool LoadTripleSetting(Vertex &p, std::ifstream &file);
  template <typename T>
  bool LoadSingleSetting(T &p, std::ifstream &file);
};

}  // namespace s21

#endif