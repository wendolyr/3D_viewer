#include "tests_helpers.h"

TEST(FileManagerTester, ParseFileTest1) {
  s21::Facade controller;
  EXPECT_EQ(controller.ParseFile("1234.obj"), s21::FileError::kNotExist);
}

TEST(FileManagerTester, ParseFileTest2) {
  s21::Facade controller;
  CreateCorrectFile();
  EXPECT_EQ(controller.ParseFile("test.obj"), s21::FileError::kOk);

  CreateFileWithoutVerticesEdges();
  EXPECT_EQ(controller.ParseFile("test.obj"), s21::FileError::kInvalidFile);
}

TEST(FileManagerTester, ParseFileTest3) {
  s21::Facade controller;
  CreateInvalidVerticesOneFile();
  EXPECT_EQ(controller.ParseFile("test.obj"), s21::FileError::kInvalidFile);

  CreateInvalidVerticesTwoFile();
  EXPECT_EQ(controller.ParseFile("test.obj"), s21::FileError::kInvalidFile);

  CreateInvalidVerticesThreeFile();
  EXPECT_EQ(controller.ParseFile("test.obj"), s21::FileError::kInvalidFile);
}

TEST(FileManagerTester, ParseFileTest4) {
  s21::Facade controller;
  CreateInvalidEdgesOneFile();
  EXPECT_EQ(controller.ParseFile("test.obj"), s21::FileError::kInvalidFile);

  CreateInvalidEdgesTwoFile();
  EXPECT_EQ(controller.ParseFile("test.obj"), s21::FileError::kInvalidFile);

  CreateInvalidEdgesThreeFile();
  EXPECT_EQ(controller.ParseFile("test.obj"), s21::FileError::kInvalidFile);

  CreateInvalidEdgesFourFile();
  EXPECT_EQ(controller.ParseFile("test.obj"), s21::FileError::kInvalidFile);
}

TEST(FileManagerTester, SaveLoadSettingsTest1) {
  s21::Facade controller;
  s21::ViewParams p;
  EXPECT_FALSE(controller.LoadLastState(p));
}

TEST(FileManagerTester, SaveLoadSettingsTest2) {
  s21::Facade controller;
  CreateCorrectFile();
  controller.ParseFile("test.obj");
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
  CreateIncorrectLastSettingsFileOne();

  s21::ViewParams p;
  EXPECT_FALSE(controller.LoadLastState(p));

  CreateIncorrectLastSettingsFileTwo();
  EXPECT_FALSE(controller.LoadLastState(p));

  CreateIncorrectLastSettingsFileThree();
  EXPECT_FALSE(controller.LoadLastState(p));

  CreateEmptyLastSettingsFile();
  EXPECT_FALSE(controller.LoadLastState(p));

  controller.ResetSettings();
  controller.SaveModel(p);
  CreateEmptyLastObjectFile();
  EXPECT_TRUE(controller.LoadLastState(p));
}