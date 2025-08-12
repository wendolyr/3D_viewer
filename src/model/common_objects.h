#ifndef CPP4_3DVIEWER_V2_0_MODEL_COMMON_OBJECTS_H
#define CPP4_3DVIEWER_V2_0_MODEL_COMMON_OBJECTS_H

#include <functional>
#include <string>

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
  Vertex& operator+(const Vertex& other);

  /// Overloaded operator += for Vertex
  Vertex& operator-(const Vertex& other);
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

}  // namespace s21

#endif