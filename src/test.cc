#include <chrono>
#include <iostream>

#include "controller/facade.h"

int main() {
  s21::Facade facade;
  // std::string name = "bugatti.obj";
  std::string name = "cube.obj";

  auto start = std::chrono::steady_clock::now();
  facade.ParseFile(name);
  auto end = std::chrono::steady_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout << "Time " << duration.count() << " ms" << std::endl;

  auto v = facade.GetVertices();
  // auto p = facade.GetPolygons();

  for (auto i : v) {
    // std::cout << i.first + 1 << ' ' << i.second + 1 << '\n';
    std::cout << i.x << ' ' << i.y << ' ' << i.z << '\n';
  }

  s21::Vertex a = {90, 0, 0};
  // facade.MoveFigure(a);
  facade.RotateFigure(a);

  a = {0, 90, 0};
  facade.RotateFigure(a);

  a = {0, -90, 0};
  facade.RotateFigure(a);
  a = {-90, 0, 0};
  facade.RotateFigure(a);

  // facade.ScaleFigure(2);

  // std::cout << facade.GetVertices().size() << std::endl;
  // std::cout << facade.GetPolygons().size() << std::endl;

  v = facade.GetVertices();
  // auto p = facade.GetPolygons();

  for (auto i : v) {
    // std::cout << i.first + 1 << ' ' << i.second + 1 << '\n';
    std::cout << i.x << ' ' << i.y << ' ' << i.z << '\n';
  }

  return 0;
}