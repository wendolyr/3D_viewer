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

TEST(FileManagerTester, SaveLoadSettingsTest1) {
  s21::Facade controller;
  s21::ViewParams p;
  EXPECT_FALSE(controller.LoadLastState(p));
}

TEST(FileManagerTester, SaveLoadSettingsTest2) {
  s21::Facade controller;
  CreateCorrectFile();
  controller.ParseFile("cube.obj");
  controller.RotateFigure({90.0, 0, 0});
  controller.MoveFigure({0.0, 24.0, 0});
  controller.ScaleFigure(3.0);
  auto before = controller.GetVertices();
  s21::ViewParams p;
  controller.SaveModel(p);

  s21::Facade controller2;
  controller2.LoadLastState(p);
  auto after = controller2.GetVertices();
  // std::cout << after.size() << '\n';
  for (size_t i = 0; i < before.size(); ++i) {
    EXPECT_TRUE(fabs(before[i].x - after[i].x) < 1e-6);
    EXPECT_TRUE(fabs(before[i].y - after[i].y) < 1e-6);
    EXPECT_TRUE(fabs(before[i].z - after[i].z) < 1e-6);
  }
}

TEST(FileManagerTester, SaveLoadSettingsTest3) {
  s21::Facade controller;
  CreateIncorrectLastSettingsFile();

  s21::ViewParams p;
  EXPECT_FALSE(controller.LoadLastState(p));
}