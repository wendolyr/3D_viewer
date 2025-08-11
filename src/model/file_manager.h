#ifndef CPP4_3DVIEWER_V2_0_MODEL_PARSER_H
#define CPP4_3DVIEWER_V2_0_MODEL_PARSER_H

#include <charconv>
#include <fstream>
#include <sstream>
#include <string>

#include "figure.h"

namespace s21 {

class FileManager {
 public:
  FileError ParseFile(const std::string &file_name, FigureModel &model);
  void SaveModel(FigureModel &model, ViewParams &view_params);
  bool LoadLastState(FigureModel &model, ViewParams &view_params);

 private:
  FileError ParseVertices(const char *ptr, std::vector<Vertex> &vertices);
  FileError ParsePolygons(
      const char *ptr, std::vector<Vertex> &vertices,
      std::unordered_set<std::pair<unsigned, unsigned>, PairHash> &polygons);

  void SaveSettings(FigureModel &model, ViewParams &view_params);
  bool LoadSettings(FigureModel &model, ViewParams &view_params);
  bool LoadTripleSetting(Vertex &p, std::ifstream &file);

  template <typename T>
  bool LoadSingleSetting(T &p, std::ifstream &file);
};

}  // namespace s21

#endif