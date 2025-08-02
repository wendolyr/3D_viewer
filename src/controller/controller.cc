#ifndef CPP4_3DVIEWER_V2_0_CONTROLLER_CONTROLLER_H
#define CPP4_3DVIEWER_V2_0_CONTROLLER_CONTROLLER_H

#include <set>
#include <vector>

#include "../model/common_objects.h"

namespace s21 {

/**
 * to do
 * Complete object_ -> New class (Facade probably ?)
 * Methods getters and setters
 * where should place parser?
 */

class Controller {
 public:
  std::vector<Vertex> GetVertices() { return object_.GetVertices(); }

  std::set<std::pair<unsigned, unsigned>> GetPolygons() {
    return object_.GetPolygons();
  }

  void Transform(std::vector<Vertex>& vertces) { object_.Transform(vertices); }

 private:
  CHANGE_ME object_;

  Controller() = default;
  ~Controller() = default;
}

}  // namespace s21

#endif