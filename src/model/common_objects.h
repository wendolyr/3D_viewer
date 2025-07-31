#ifndef CPP4_3DVIEWER_V2_0_MODEL_COMMON_OBJECTS_H
#define CPP4_3DVIEWER_V2_0_MODEL_COMMON_OBJECTS_H

namespace s21 {

struct Vertex {
  double x, y, z;
};

enum class Parameter { kScale, kX, kY, kZ };

}  // namespace s21

#endif