#ifndef CPP4_3DVIEWER_V2_0_MODEL_COMMON_OBJECTS_H
#define CPP4_3DVIEWER_V2_0_MODEL_COMMON_OBJECTS_H

#include <functional>
#include <string>

namespace s21 {

enum class FileError { kOk, kNotExist, kInvalidFile };

struct Vertex {
  float x = 0.0, y = 0.0, z = 0.0;

  Vertex& operator+=(const Vertex& other) {
    x += other.x;
    y += other.y;
    z += other.z;

    return *this;
  }

  Vertex& operator-=(const Vertex& other) {
    y -= other.y;
    x -= other.x;
    z -= other.z;

    return *this;
  }

  Vertex& operator+(const Vertex& other) {
    *this += other;

    return *this;
  }

  Vertex& operator-(const Vertex& other) {
    *this -= other;

    return *this;
  }
};

struct ViewParams {
  int projection_type;

  int edge_type;
  Vertex edge_color;
  float edge_thickness;
  // float dash_size;
  // float gap_size;

  int vertex_display;
  Vertex vertex_color;
  float vertex_size;

  Vertex background_color;
  std::string file_name;
};

struct Params {
  Vertex shift;
  Vertex rotation;
  float scale = 0.0;
};

struct PairHash {
  std::size_t operator()(const std::pair<unsigned, unsigned>& p) const {
    return p.first * 2654435761 + p.second;
  }
};

}  // namespace s21

#endif