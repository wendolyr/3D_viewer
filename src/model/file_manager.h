#ifndef CPP4_3DVIEWER_V2_0_MODEL_PARSER_H
#define CPP4_3DVIEWER_V2_0_MODEL_PARSER_H

#include <charconv>
#include <fstream>
#include <sstream>
#include <string>

#include "model.h"

namespace s21 {

class FileManager {
 public:
  void LoadLastState(FigureModel &model);
  int ParseFile(std::string &file_name, FigureModel &model);
  void SaveModel(std::string &file_name, FigureModel &model);

 private:
  int ParseVertices();
  int ParsePolygons();
  const char *SkipToTheNextLine(const char *ptr, const char *end);

  void SaveSettings(std::string &file_name, FigureModel &model);
  bool LoadSettings(FigureModel &model);
};

}  // namespace s21

#endif