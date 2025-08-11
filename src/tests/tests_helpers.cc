#include "tests_helpers.h"

void CreateCorrectFile() {
  std::ofstream file("cube.obj");
  file << "v -1.0 -1.0 -1.0\n";
  file << "v  1.0 -1.0 -1.0\n";
  file << "v  1.0  1.0 -1.0\n";
  file << "v -1.0  1.0 -1.0\n";
  file << "v -1.0 -1.0  1.0\n";
  file << "v  1.0 -1.0  1.0\n";
  file << "v  1.0  1.0  1.0\n";
  file << "v -1.0  1.0  1.0\n";
  file << "vn -1.0  2.2  3.3\n";
  file << "f 1 2 3 4\n";
  file << "f 5 6 7 8\n";
  file << "f 1//23 2 6 5\n";
  file << "f 2 3 7 6\n";
  file << "f 3 4 8 -1\n";
  file << "f 4 1 5 8\n";
  file << "fp 3 2 3 1\n";
}

void CreateInvalidVerticesOneFile() {
  std::ofstream file("cube.obj");
  file << "v -1.0u-1.0 -1.0\n";
}

void CreateInvalidVerticesTwoFile() {
  std::ofstream file("cube.obj");
  file << "v -1.0 -1.0 -1.0i\n";
}

void CreateInvalidVerticesThreeFile() {
  std::ofstream file("cube.obj");
  file << "v -1.0 -1.0 -1.0 \n";
}

void CreateInvalidPolygonsOneFile() {
  std::ofstream file("cube.obj");
  file << "v -1.0 -1.0 -1.0\n";
  file << "v  1.0 -1.0 -1.0\n";
  file << "v  1.0  1.0 -1.0\n";
  file << "v -1.0  1.0 -1.0\n";

  file << "f a1 2 3 4\n";
}

void CreateInvalidPolygonsTwoFile() {
  std::ofstream file("cube.obj");
  file << "v -1.0 -1.0 -1.0\n";
  file << "v  1.0 -1.0 -1.0\n";
  file << "v  1.0  1.0 -1.0\n";
  file << "v -1.0  1.0 -1.0\n";

  file << "f 1_2 3 4\n";
}

void CreateInvalidPolygonsThreeFile() {
  std::ofstream file("cube.obj");
  file << "v -1.0 -1.0 -1.0\n";
  file << "v  1.0 -1.0 -1.0\n";
  file << "v  1.0  1.0 -1.0\n";
  file << "v -1.0  1.0 -1.0\n";

  file << "f 1 2 3 4_\n";
}

void CreateInvalidPolygonsFourFile() {
  std::ofstream file("cube.obj");
  file << "v -1.0 -1.0 -1.0\n";
  file << "v  1.0 -1.0 -1.0\n";
  file << "v  1.0  1.0 -1.0\n";
  file << "v -1.0  1.0 -1.0\n";

  file << "f 1 2 3 5\n";
}