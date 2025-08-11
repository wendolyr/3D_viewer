#include "common_objects.h"

namespace s21 {

Vertex& Vertex::operator+=(const Vertex& other) {
  x += other.x;
  y += other.y;
  z += other.z;

  return *this;
}

Vertex& Vertex::operator-=(const Vertex& other) {
  y -= other.y;
  x -= other.x;
  z -= other.z;

  return *this;
}

Vertex& Vertex::operator+(const Vertex& other) {
  *this += other;

  return *this;
}

Vertex& Vertex::operator-(const Vertex& other) {
  *this -= other;

  return *this;
}

}  // namespace s21