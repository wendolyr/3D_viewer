#include "tests_helpers.h"

TEST(AffineTransformationTester, ScaleTest) {
  s21::Facade controller;
  CreateCorrectFile();
  EXPECT_EQ(controller.ParseFile("test.obj"), s21::FileError::kOk);

  auto before = controller.GetVertices();
  controller.ScaleFigure(2.0);

  auto after = controller.GetVertices();

  for (size_t i = 0; i < before.size(); ++i) {
    EXPECT_TRUE(fabs(after[i].x / before[i].x) - 2.0 < 1e-6);
    EXPECT_TRUE(fabs(after[i].y / before[i].y) - 2.0 < 1e-6);
    EXPECT_TRUE(fabs(after[i].z / before[i].z) - 2.0 < 1e-6);
  }

  controller.ScaleFigure(4.0);

  after = controller.GetVertices();

  for (size_t i = 0; i < before.size(); ++i) {
    EXPECT_TRUE(fabs(after[i].x / before[i].x) - 4.0 < 1e-6);
    EXPECT_TRUE(fabs(after[i].y / before[i].y) - 4.0 < 1e-6);
    EXPECT_TRUE(fabs(after[i].z / before[i].z) - 4.0 < 1e-6);
  }

  controller.ScaleFigure(0.5);

  after = controller.GetVertices();

  for (size_t i = 0; i < before.size(); ++i) {
    EXPECT_TRUE(fabs(after[i].x / before[i].x) - 0.5 < 1e-6);
    EXPECT_TRUE(fabs(after[i].y / before[i].y) - 0.5 < 1e-6);
    EXPECT_TRUE(fabs(after[i].z / before[i].z) - 0.5 < 1e-6);
  }
}

TEST(AffineTransformationTester, MoveTest) {
  s21::Facade controller;
  CreateCorrectFile();
  EXPECT_EQ(controller.ParseFile("test.obj"), s21::FileError::kOk);

  auto before = controller.GetVertices();
  controller.MoveFigure({1.0, 0.0, 0.0});

  auto after = controller.GetVertices();

  for (size_t i = 0; i < before.size(); ++i) {
    EXPECT_TRUE(after[i].x - before[i].x - 1.0 < 1e-6);
    EXPECT_TRUE(after[i].y - before[i].y < 1e-6);
    EXPECT_TRUE(after[i].z - before[i].z < 1e-6);
  }

  controller.MoveFigure({0.0, 1.0, 0.0});
  after = controller.GetVertices();
  for (size_t i = 0; i < before.size(); ++i) {
    EXPECT_TRUE(after[i].x - before[i].x - 1.0 < 1e-6);
    EXPECT_TRUE(after[i].y - before[i].y - 1.0 < 1e-6);
    EXPECT_TRUE(after[i].z - before[i].z < 1e-6);
  }

  controller.MoveFigure({0.0, 0.0, -1.0});
  after = controller.GetVertices();
  for (size_t i = 0; i < before.size(); ++i) {
    EXPECT_TRUE(after[i].x - before[i].x - 1.0 < 1e-6);
    EXPECT_TRUE(after[i].y - before[i].y - 1.0 < 1e-6);
    EXPECT_TRUE(after[i].z - before[i].z + 1.0 < 1e-6);
  }
}

TEST(AffineTransformationTester, RotateTest) {
  s21::Facade controller;
  CreateCorrectFile();
  EXPECT_EQ(controller.ParseFile("test.obj"), s21::FileError::kOk);

  auto before = controller.GetVertices();

  controller.RotateFigure({90.0, 0.0, 0.0});
  controller.RotateFigure({0.0, 90.0, 0.0});
  controller.RotateFigure({0.0, 0.0, 90.0});

  controller.RotateFigure({0.0, 0.0, -90.0});
  controller.RotateFigure({0.0, -90.0, 0.0});
  controller.RotateFigure({-90.0, 0.0, 0.0});

  auto after = controller.GetVertices();

  for (size_t i = 0; i < before.size(); ++i) {
    EXPECT_TRUE(fabs(after[i].x - before[i].x) < 1e-6);
    EXPECT_TRUE(fabs(after[i].y - before[i].y) < 1e-6);
    EXPECT_TRUE(fabs(after[i].z - before[i].z) < 1e-6);
  }
}