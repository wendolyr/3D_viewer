#include <ctime>
#include <iostream>

#include "controller/facade.h"

int main() {
  s21::Facade facade;
  std::string name = "cube.obj";

  facade.ParseFile(name);

  std::cout << facade.GetVertices().size() << std::endl;
  std::cout << facade.GetPolygons().size() << std::endl;

  return 0;
}