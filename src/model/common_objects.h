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
 * @struct Quaternion
 * @brief Represents a quaternion for 3D rotation operations. Allows to avoid
 * gimbal lock.
 */
struct Quaternion {
  float x, y, z, w;  ///< Components of the quaternion (xi + yj + zk + w)

  /**
   * @brief Default constructor. Initializes to identity quaternion (no
   * rotation).
   */
  Quaternion();

  /**
   * @brief Parameterized constructor.
   * @param a X component (imaginary part i)
   * @param b Y component (imaginary part j)
   * @param c Z component (imaginary part k)
   * @param d W component (real part)
   */
  Quaternion(float a, float b, float c, float d);

  /**
   * @brief Creates a quaternion from Euler angles
   * @param pitch Rotation around X-axis in radians
   * @param yaw Rotation around Y-axis in radians
   * @param roll Rotation around Z-axis in radians
   * @return Quaternion representing the combined rotation
   */
  static Quaternion FromEuler(float pitch, float yaw, float roll);

  /**
   * @brief Converts the quaternion to a 4x4 rotation matrix.
   * @return 4x4 rotation matrix
   */
  std::vector<std::vector<float>> ToMatrix() const;

  /**
   * @brief Multiplies two quaternions
   * @param other The quaternion to multiply with
   * @return Resulting quaternion representing combined rotation
   */
  Quaternion operator*(const Quaternion& other) const;
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
 * @struct PairHash
 * @brief Custom hash function for edge pairs.
 */
struct PairHash {
  std::size_t operator()(const std::pair<unsigned, unsigned>& p) const {
    return p.first * 2654435761 + p.second;
  }
};

/**
 * @struct Params
 * @brief Aggregates transformation state parameters.
 */
struct Params {
  Vertex shift;           ///< Translation offsets
  Vertex rotation;        ///< Rotation angles (Euler degrees)
  float scale = 0.0;      ///< Current scaling factor
  Quaternion quaternion;  ///< Quaternion rotation
};

}  // namespace s21

#endif