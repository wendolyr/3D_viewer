#ifndef CPP4_3DVIEWER_V2_0_MODEL_FACADE_H
#define CPP4_3DVIEWER_V2_0_MODEL_FACADE_H

#include "file_manager.h"
#include "model.h"

namespace s21 {

class Facade {
 public:
  void ParseFile(std::string &file);

 private:
  FigureModel model_;
  FileManager parser_data_;
};

}  // namespace s21

#endif