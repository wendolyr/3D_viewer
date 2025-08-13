#include "tests_helpers.h"

void CreateCorrectFile() {
  std::ofstream file("test.obj");
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

void CreateFileWithoutVerticesEdges() {
  std::ofstream file("test.obj");
  file << "vn 12 22 33";
}

void CreateInvalidVerticesOneFile() {
  std::ofstream file("test.obj");
  file << "v -1.0u-1.0 -1.0\n";
}

void CreateInvalidVerticesTwoFile() {
  std::ofstream file("test.obj");
  file << "v -1.0 -1.0 -1.0i\n";
}

void CreateInvalidVerticesThreeFile() {
  std::ofstream file("test.obj");
  file << "v -1.0 -1.0 -1.0 \n";
}

void CreateInvalidEdgesOneFile() {
  std::ofstream file("test.obj");
  file << "v -1.0 -1.0 -1.0\n";
  file << "v  1.0 -1.0 -1.0\n";
  file << "v  1.0  1.0 -1.0\n";
  file << "v -1.0  1.0 -1.0\n";

  file << "f a1 2 3 4\n";
}

void CreateInvalidEdgesTwoFile() {
  std::ofstream file("test.obj");
  file << "v -1.0 -1.0 -1.0\n";
  file << "v  1.0 -1.0 -1.0\n";
  file << "v  1.0  1.0 -1.0\n";
  file << "v -1.0  1.0 -1.0\n";

  file << "f 1_2 3 4\n";
}

void CreateInvalidEdgesThreeFile() {
  std::ofstream file("test.obj");
  file << "v -1.0 -1.0 -1.0\n";
  file << "v  1.0 -1.0 -1.0\n";
  file << "v  1.0  1.0 -1.0\n";
  file << "v -1.0  1.0 -1.0\n";

  file << "f 1 2 3 4_\n";
}

void CreateInvalidEdgesFourFile() {
  std::ofstream file("test.obj");
  file << "v -1.0 -1.0 -1.0\n";
  file << "v  1.0 -1.0 -1.0\n";
  file << "v  1.0  1.0 -1.0\n";
  file << "v -1.0  1.0 -1.0\n";

  file << "f 1 2 3 5\n";
}

void CreateIncorrectLastSettingsFileOne() {
  std::ofstream file(".last_settings.txt");
  file << "12\n";
}

void CreateIncorrectLastSettingsFileTwo() {
  std::ofstream file(".last_settings.txt");
  file << "0.0 0.0 0.0\n";
  file << "0.0 0.0 0.0\n";
}

void CreateIncorrectLastSettingsFileThree() {
  std::ofstream file(".last_settings.txt");
  file << "0.0 0.0 0.0\n";
  file << "0.0 0.0 0.0\n";
  file << "0.0 g\n";
}

void CreateIncorrectLastObjectFile() {
  std::ofstream file(".last_object.obj");
  file << "v g";
}

void CreateEmptyLastSettingsFile() { std::ofstream file(".last_settings.txt"); }

void CreateEmptyLastObjectFile() { std::ofstream file(".last_object.obj"); }