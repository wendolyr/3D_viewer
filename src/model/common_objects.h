#ifndef CPP4_3DVIEWER_V2_0_MODEL_COMMON_OBJECTS_H
#define CPP4_3DVIEWER_V2_0_MODEL_COMMON_OBJECTS_H

#include <functional>

namespace s21 {

struct Vertex {
  float x, y, z;
};

struct Params {
  Vertex shift;
  Vertex rotation;
  float scale;
};

struct PairHash {
  std::size_t operator()(const std::pair<unsigned, unsigned>& p) const {
    return p.first * 2654435761 + p.second;
  }
};

}  // namespace s21

#endif