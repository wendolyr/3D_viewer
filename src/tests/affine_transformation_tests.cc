#include "tests_helpers.h"

TEST(AffineTransformationTester, ScaleTest) {
  s21::Facade controller;
  CreateCorrectFile();
  EXPECT_EQ(controller.ParseFile("test.obj"), s21::FileError::kOk);

  std::vector<std::vector<float>> t;
  controller.ScaleFigure(t, 2.0);

  for (size_t i = 0; i < t.size(); ++i) {
    for (size_t j = 0; j < t[i].size(); ++j) {
      if (i == j && i != 3) {
        EXPECT_TRUE(fabs(t[i][j] - 2.0) < 1e-6);
      } else if (i == j && i == 3) {
        EXPECT_TRUE(fabs(t[i][j] - 1.0) < 1e-6);
      } else {
        EXPECT_TRUE(fabs(t[i][j]) < 1e-6);
      }
    }
  }

  controller.ScaleFigure(t, 4.0);

  for (size_t i = 0; i < t.size(); ++i) {
    for (size_t j = 0; j < t[i].size(); ++j) {
      if (i == j && i != 3) {
        EXPECT_TRUE(fabs(t[i][j] - 2.0) < 1e-6);
      } else if (i == j && i == 3) {
        EXPECT_TRUE(fabs(t[i][j] - 1.0) < 1e-6);
      } else {
        EXPECT_TRUE(fabs(t[i][j]) < 1e-6);
      }
    }
  }

  controller.ScaleFigure(t, 0.5);

  for (size_t i = 0; i < t.size(); ++i) {
    for (size_t j = 0; j < t[i].size(); ++j) {
      if (i == j && i != 3) {
        EXPECT_TRUE(fabs(t[i][j] - 0.125) < 1e-6);
      } else if (i == j && i == 3) {
        EXPECT_TRUE(fabs(t[i][j] - 1.0) < 1e-6);
      } else {
        EXPECT_TRUE(fabs(t[i][j]) < 1e-6);
      }
    }
  }
}

TEST(AffineTransformationTester, MoveTest) {
  s21::Facade controller;
  CreateCorrectFile();
  EXPECT_EQ(controller.ParseFile("test.obj"), s21::FileError::kOk);

  std::vector<std::vector<float>> t;
  controller.MoveFigure(t, {5.0, 0.0, 0.0});

  for (size_t i = 0; i < t.size(); ++i) {
    for (size_t j = 0; j < t[i].size(); ++j) {
      if (i == j) {
        EXPECT_TRUE(fabs(t[i][j] - 1.0) < 1e-6);
      } else if (j == 3 && i == 0) {
        EXPECT_TRUE(fabs(t[i][j] - 5.0) < 1e-6);
      } else {
        EXPECT_TRUE(fabs(t[i][j]) < 1e-6);
      }
    }
  }

  controller.MoveFigure(t, {5.0, 5.0, 0.0});

  for (size_t i = 0; i < t.size(); ++i) {
    for (size_t j = 0; j < t[i].size(); ++j) {
      if (i == j) {
        EXPECT_TRUE(fabs(t[i][j] - 1.0) < 1e-6);
      } else if (j == 3 && i == 1) {
        EXPECT_TRUE(fabs(t[i][j] - 5.0) < 1e-6);
      } else {
        EXPECT_TRUE(fabs(t[i][j]) < 1e-6);
      }
    }
  }

  controller.MoveFigure(t, {5.0, 5.0, -5.0});

  for (size_t i = 0; i < t.size(); ++i) {
    for (size_t j = 0; j < t[i].size(); ++j) {
      if (i == j) {
        EXPECT_TRUE(fabs(t[i][j] - 1.0) < 1e-6);
      } else if (j == 3 && i == 2) {
        EXPECT_TRUE(fabs(t[i][j] + 5.0) < 1e-6);
      } else {
        EXPECT_TRUE(fabs(t[i][j]) < 1e-6);
      }
    }
  }
}

TEST(AffineTransformationTester, RotateTest) {
  s21::Facade controller;
  CreateCorrectFile();
  EXPECT_EQ(controller.ParseFile("test.obj"), s21::FileError::kOk);

  std::vector<std::vector<float>> t;
  controller.RotateFigure(t, {90.0, 0.0, 0.0});
  for (size_t i = 0; i < t.size(); ++i) {
    for (size_t j = 0; j < t[i].size(); ++j) {
      if (i == j && (i == 0 || i == 3)) {
        EXPECT_TRUE(fabs(t[i][j] - 1.0) < 1e-6);
      } else if (i == 1 && j == 2) {
        EXPECT_TRUE(fabs(t[i][j] + sin(M_PI / 2)) < 1e-6);
      } else if (i == 2 && j == 1) {
        EXPECT_TRUE(fabs(t[i][j] - sin(M_PI / 2)) < 1e-6);
      } else {
        EXPECT_TRUE(fabs(t[i][j]) < 1e-6);
      }
    }
  }

  controller.RotateFigure(t, {90.0, 90.0, 0.0});
  for (size_t i = 0; i < t.size(); ++i) {
    for (size_t j = 0; j < t[i].size(); ++j) {
      if (i == j && (i == 1 || i == 3)) {
        EXPECT_TRUE(fabs(t[i][j] - 1.0) < 1e-6);
      } else if (i == 0 && j == 2) {
        EXPECT_TRUE(fabs(t[i][j] - sin(M_PI / 2)) < 1e-6);
      } else if (i == 2 && j == 0) {
        EXPECT_TRUE(fabs(t[i][j] + sin(M_PI / 2)) < 1e-6);
      } else {
        EXPECT_TRUE(fabs(t[i][j]) < 1e-6);
      }
    }
  }

  controller.RotateFigure(t, {90.0, 90.0, 90.0});
  for (size_t i = 0; i < t.size(); ++i) {
    for (size_t j = 0; j < t[i].size(); ++j) {
      if (i == j && (i == 2 || i == 3)) {
        EXPECT_TRUE(fabs(t[i][j] - 1.0) < 1e-6);
      } else if (i == 0 && j == 1) {
        EXPECT_TRUE(fabs(t[i][j] + sin(M_PI / 2)) < 1e-6);
      } else if (i == 1 && j == 0) {
        EXPECT_TRUE(fabs(t[i][j] - sin(M_PI / 2)) < 1e-6);
      } else {
        EXPECT_TRUE(fabs(t[i][j]) < 1e-6);
      }
    }
  }

  // controller.RotateFigure(t, {0.0, 0.0, -90.0});
  // controller.RotateFigure(t, {0.0, -90.0, 0.0});
  // controller.RotateFigure(t, {-90.0, 0.0, 0.0});
}