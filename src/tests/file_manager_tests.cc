#include "tests_helpers.h"

TEST(FileManagerTester, ParseFileTest1) {
  s21::Facade controller;
  EXPECT_EQ(controller.ParseFile("1234.obj"), s21::FileError::kNotExist);
}

TEST(FileManagerTester, ParseFileTest2) {
  s21::Facade controller;
  CreateCorrectFile();
  EXPECT_EQ(controller.ParseFile("cube.obj"), s21::FileError::kOk);
}

TEST(FileManagerTester, ParseFileTest3) {
  s21::Facade controller;
  CreateInvalidVerticesOneFile();
  EXPECT_EQ(controller.ParseFile("cube.obj"), s21::FileError::kInvalidFile);

  CreateInvalidVerticesTwoFile();
  EXPECT_EQ(controller.ParseFile("cube.obj"), s21::FileError::kInvalidFile);

  CreateInvalidVerticesThreeFile();
  EXPECT_EQ(controller.ParseFile("cube.obj"), s21::FileError::kInvalidFile);
}

TEST(FileManagerTester, ParseFileTest4) {
  s21::Facade controller;
  CreateInvalidPolygonsOneFile();
  EXPECT_EQ(controller.ParseFile("cube.obj"), s21::FileError::kInvalidFile);

  CreateInvalidPolygonsTwoFile();
  EXPECT_EQ(controller.ParseFile("cube.obj"), s21::FileError::kInvalidFile);

  CreateInvalidPolygonsThreeFile();
  EXPECT_EQ(controller.ParseFile("cube.obj"), s21::FileError::kInvalidFile);

  CreateInvalidPolygonsFourFile();
  EXPECT_EQ(controller.ParseFile("cube.obj"), s21::FileError::kInvalidFile);
}

TEST(FileManagerTester, SaveLoadSettings) {
  s21::Facade controller;
  CreateCorrectFile();
  controller.RotateFigure({90.0, 0, 0});
  controller.MoveFigure({0.0, 24.0, 0});
}
