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

Vertex Vertex::operator+(const Vertex& other) {
  Vertex temp = {x + other.x, y + other.y, z + other.z};

  return temp;
}

Vertex Vertex::operator-(const Vertex& other) {
  Vertex temp = {x - other.x, y - other.y, z - other.z};

  return temp;
}

Quaternion::Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}

Quaternion::Quaternion(float a, float b, float c, float d)
    : x(a), y(b), z(c), w(d) {}

Quaternion Quaternion::FromEuler(float pitch, float yaw, float roll) {
  float cy = cos(yaw * 0.5f);
  float sy = sin(yaw * 0.5f);
  float cp = cos(pitch * 0.5f);
  float sp = sin(pitch * 0.5f);
  float cr = cos(roll * 0.5f);
  float sr = sin(roll * 0.5f);

  return Quaternion(cy * cp * sr - sy * sp * cr, sy * cp * sr + cy * sp * cr,
                    sy * cp * cr - cy * sp * sr, cy * cp * cr + sy * sp * sr);
}

std::vector<std::vector<float>> Quaternion::ToMatrix() const {
  std::vector<std::vector<float>> matrix(4, std::vector<float>(4, 0.0f));

  float xx = x * x;
  float xy = x * y;
  float xz = x * z;
  float xw = x * w;
  float yy = y * y;
  float yz = y * z;
  float yw = y * w;
  float zz = z * z;
  float zw = z * w;

  matrix[0][0] = 1.0f - 2.0f * (yy + zz);
  matrix[0][1] = 2.0f * (xy - zw);
  matrix[0][2] = 2.0f * (xz + yw);
  matrix[0][3] = 0.0f;

  matrix[1][0] = 2.0f * (xy + zw);
  matrix[1][1] = 1.0f - 2.0f * (xx + zz);
  matrix[1][2] = 2.0f * (yz - xw);
  matrix[1][3] = 0.0f;

  matrix[2][0] = 2.0f * (xz - yw);
  matrix[2][1] = 2.0f * (yz + xw);
  matrix[2][2] = 1.0f - 2.0f * (xx + yy);
  matrix[2][3] = 0.0f;

  matrix[3][0] = 0.0f;
  matrix[3][1] = 0.0f;
  matrix[3][2] = 0.0f;
  matrix[3][3] = 1.0f;

  return matrix;
}

Quaternion Quaternion::operator*(const Quaternion& other) const {
  return Quaternion(w * other.x + x * other.w + y * other.z - z * other.y,
                    w * other.y - x * other.z + y * other.w + z * other.x,
                    w * other.z + x * other.y - y * other.x + z * other.w,
                    w * other.w - x * other.x - y * other.y - z * other.z);
}

}  // namespace s21