#ifndef CPP4_3DVIEWER_V2_0_MODEL_COMMON_OBJECTS_H
#define CPP4_3DVIEWER_V2_0_MODEL_COMMON_OBJECTS_H

namespace s21 {

struct Vertex {
  double x, y, z;
};

struct Params {
  Vertex shift;
  Vertex rotation;
  double scale;
};

}  // namespace s21

#endif