#ifndef CPP4_3DVIEWER_V2_0_MODEL_COMMON_OBJECTS_H
#define CPP4_3DVIEWER_V2_0_MODEL_COMMON_OBJECTS_H

#include <cmath>
#include <functional>
#include <string>
#include <vector>

namespace s21 {

/**
 * @enum FileError
 * @brief File operation status codes
 */
enum class FileError {
  kOk,          ///< Correct file
  kNotExist,    ///< File doesn't exist
  kInvalidFile  ///< Incorrect file content
};

/**
 * @struct Vertex
 * @brief Represents a 3D point in space.
 */
struct Vertex {
  float x = 0.0, y = 0.0, z = 0.0;

  /// Overloaded operator += for Vertex
  Vertex& operator+=(const Vertex& other);

  /// Overloaded operator -= for Vertex
  Vertex& operator-=(const Vertex& other);

  /// Overloaded operator + for Vertex
  Vertex operator+(const Vertex& other);

  /// Overloaded operator += for Vertex
  Vertex operator-(const Vertex& other);
};

/**
 * @struct ViewParams
 * @brief Stores visualization settings for rendering.
 */
struct ViewParams {
  int projection_type = 0;  ///< Projection mode (orthogonal/perspective)

  int edge_type = 0;           ///< Edge rendering style (solid/dashed)
  Vertex edge_color;           ///< RGB color for edges
  float edge_thickness = 0.0;  ///< Edge line width

  int vertex_display = 0.0;  ///< Vertex rendering mode (none/circle/square)
  Vertex vertex_color;       ///< RGB color for vertices
  float vertex_size = 0.0;   ///< Vertex point size

  Vertex background_color;         ///< RGB background color
  std::string file_name = ".obj";  ///< Default file name
};

/**
 * @struct Params
 * @brief Aggregates transformation state parameters.
 */
struct Params {
  Vertex shift;       ///< Translation offsets
  Vertex rotation;    ///< Rotation angles (degrees)
  float scale = 0.0;  ///< Current scaling factor
};

/**
 * @struct PairHash
 * @brief Custom hash function for edge pairs.
 */
struct PairHash {
  std::size_t operator()(const std::pair<unsigned, unsigned>& p) const {
    return p.first * 2654435761 + p.second;
  }
};

class Quaternion {
 public:
  Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
  Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

  static Quaternion FromEuler(float pitch, float yaw, float roll) {
    float cy = cos(yaw * 0.5f);
    float sy = sin(yaw * 0.5f);
    float cp = cos(pitch * 0.5f);
    float sp = sin(pitch * 0.5f);
    float cr = cos(roll * 0.5f);
    float sr = sin(roll * 0.5f);

    return Quaternion(cy * cp * sr - sy * sp * cr, sy * cp * sr + cy * sp * cr,
                      sy * cp * cr - cy * sp * sr, cy * cp * cr + sy * sp * sr);
  }

  std::vector<std::vector<float>> ToMatrix() const {
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

  Quaternion operator*(const Quaternion& other) const {
    return Quaternion(w * other.x + x * other.w + y * other.z - z * other.y,
                      w * other.y - x * other.z + y * other.w + z * other.x,
                      w * other.z + x * other.y - y * other.x + z * other.w,
                      w * other.w - x * other.x - y * other.y - z * other.z);
  }

  float x, y, z, w;
};

}  // namespace s21

#endif