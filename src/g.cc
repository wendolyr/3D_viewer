#include <iostream>

#include "controller/facade.h"

int main() {
  s21::Facade controller;
  controller.ParseFile("LP_Sneaker3.obj");
  std::cout << "Vertices: " << controller.GetVertices().size() << '\n';
  std::cout << "Polygons: " << controller.GetPolygons().size() << '\n';
  return 0;
}