#include "tests_helpers.h"

TEST(CommonTester, VertexOperatorTest) {
  s21::Vertex a{1.0, 2.0, 3.0};
  s21::Vertex b{2.0, 3.0, 4.0};

  a += b;

  EXPECT_TRUE(fabs(a.x - 3.0) < 1e-6);
  EXPECT_TRUE(fabs(a.y - 5.0) < 1e-6);
  EXPECT_TRUE(fabs(a.z - 7.0) < 1e-6);

  a -= b;

  EXPECT_TRUE(fabs(a.x - 1.0) < 1e-6);
  EXPECT_TRUE(fabs(a.y - 2.0) < 1e-6);
  EXPECT_TRUE(fabs(a.z - 3.0) < 1e-6);

  s21::Vertex c = a + b;

  EXPECT_TRUE(fabs(c.x - 3.0) < 1e-6);
  EXPECT_TRUE(fabs(c.y - 5.0) < 1e-6);
  EXPECT_TRUE(fabs(c.z - 7.0) < 1e-6);

  c = c - b;

  EXPECT_TRUE(fabs(c.x - 1.0) < 1e-6);
  EXPECT_TRUE(fabs(c.y - 2.0) < 1e-6);
  EXPECT_TRUE(fabs(c.z - 3.0) < 1e-6);
}