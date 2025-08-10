#ifndef CPP4_3DVIEWER_V2_0_MODEL_COMMON_OBJECTS_H
#define CPP4_3DVIEWER_V2_0_MODEL_COMMON_OBJECTS_H

#include <functional>

namespace s21 {

enum class FileError { kOk, kNotExist, kInvalidFile };

struct Vertex {
  float x, y, z;

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

  // Vertex(float a, float b, float c) : x(a), y(b), z(c) {}
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