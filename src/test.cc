#include <chrono>
#include <iostream>

#include "controller/facade.h"

int main() {
  s21::Facade facade;
  std::string name = "bugatti.obj";
  // std::string name = "cube.obj";

  facade.ParseFile(name);

  // for (int i = 2; i < 120; ++i) {
  // auto s = facade.GetVertices();
  // for (auto i : s) {
  //   std::cout << i.x << ' ' << i.y << ' ' << i.z << std::endl;
  // }

  s21::Vertex a = {1, 1, 1};
  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < 120; ++i) {
    // facade.MoveFigure(a);
    // facade.RotateFigure(a);
    facade.ScaleFigure(2);
  }

  auto end = std::chrono::steady_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "Time " << duration.count() << " ms" << std::endl;

  // s = facade.GetVertices();

  // for (auto i : s) {
  //   std::cout << i.x << ' ' << i.y << ' ' << i.z << std::endl;
  // }

  // std::cout << facade.GetVertices().size() << std::endl;
  // std::cout << facade.GetPolygons().size() << std::endl;

  return 0;
}