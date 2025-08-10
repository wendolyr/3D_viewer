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
  void LoadLastState(FigureModel &model);
  FileError ParseFile(const std::string &file_name, FigureModel &model);
  void SaveModel(const std::string &file_name, FigureModel &model);

 private:
  FileError ParseVertices(const char *ptr, std::vector<Vertex> &vertices);
  FileError ParsePolygons(
      const char *ptr, std::vector<Vertex> &vertices,
      std::unordered_set<std::pair<unsigned, unsigned>, PairHash> &polygons);

  void SaveSettings(const std::string &file_name, FigureModel &model);
  bool LoadSettings(FigureModel &model);
};

}  // namespace s21

#endif