/**
 * @file file_manager.h
 * @brief Header file for FileManager description
 */

#ifndef CPP4_3DVIEWER_V2_0_MODEL_PARSER_H
#define CPP4_3DVIEWER_V2_0_MODEL_PARSER_H

#include <charconv>
#include <chrono>
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

  /**
   * @brief Saves model and view parameters to disk.
   */
  void SaveModel(FigureModel &model, ViewParams &view_params);

  /**
   * @brief Restores the last saved model state and view settings
   * @returns True if state load successfully and false otherwise
   */
  bool LoadLastState(FigureModel &model, ViewParams &view_params);

 private:
  /**
   * @brief Helper for parsing line with vertices
   * @returns FileError status.
   */
  FileError ParseVertices(const char *ptr, std::vector<Vertex> &vertices);

  /**
   * @brief Helper for parsing line with edges
   * @returns FileError status.
   */
  FileError ParseEdges(
      const char *ptr, std::vector<Vertex> &vertices,
      std::unordered_set<std::pair<unsigned, unsigned>, PairHash> &edges);

  /**
   * @brief Save current settings to file
   */
  void SaveSettings(FigureModel &model, ViewParams &view_params);

  /**
   * @brief Load settings from file
   * @returns True if loading completed successfully and false otherwise
   */
  bool LoadSettings(FigureModel &model, ViewParams &view_params);

  /**
   * @brief Parse line with three parameters
   * @returns True if parsed successfully and false otherwise
   */
  bool LoadTripleSetting(Vertex &p, std::ifstream &file);

  /**
   * @brief Parse line with four parameters
   * @returns True if parsed successfully and false otherwise
   */
  bool LoadQuadraSetting(Quaternion &p, std::ifstream &file);

  /**
   * @brief Parse line with one parameter
   * @returns True if parsed successfully and false otherwise
   */
  template <typename T>
  bool LoadSingleSetting(T &p, std::ifstream &file);

  /**
   * @brief Parse line with one parameter (specifically for strings)
   * @returns True if parsed successfully and false otherwise
   */
  bool LoadSingleSetting(std::string &p, std::ifstream &file);
};

}  // namespace s21

#endif