#include "angem.h"

#include <cmath>

#include "gtest/gtest.h"

bool almostEquals(double a, double b) {
  return std::fabs(a - b) < std::numeric_limits<double>::epsilon();
}

bool operator==(Point const& a, Point const& b) {
  return almostEquals(a.x, b.x) && almostEquals(a.y, b.y);
}

double lineNorm(double x, double y) {
  return std::sqrt(x * x + y * y) * (x < 0 ? -1 : 1);
}

bool operator==(Line const& l1, Line const& l2) {
  double norm1 = lineNorm(l1.a, l1.b);
  double norm2 = lineNorm(l2.a, l2.b);
  return almostEquals(l1.b / norm1, l2.b / norm2) &&
         almostEquals(l1.c / norm1, l2.c / norm2);
}

TEST(AngemTest, TestLine) {
  EXPECT_EQ(Line(Point(0, 0), Point(1, 1)), Line(-1, 1, 0));
}

TEST(AngemTest, TestIntersection) {
  Point common(1, 1);
  EXPECT_EQ(Line(Point(0, 0), common) & Line(Point(0, 1), common), common);
  EXPECT_EQ(Line(1, 0, 0) & Line(0, 1, 0), Point(0, 0));
  EXPECT_EQ(Line(42, 42, 0) & Line(42, 42, 42), std::nullopt);
}

TEST(AngemTest, TestPerpendicular) {
  EXPECT_EQ(Line(-1, 1, 0).perpendicular(Point(0, 0)), Line(1, 1, 0));
  EXPECT_EQ(Line(1, 0, -1).perpendicular(Point(1, 1)), Line(0, 1, -1))
      << Line(1, 0, -1).perpendicular(Point(1, 1)).a
      << Line(1, 0, -1).perpendicular(Point(1, 1)).b
      << Line(1, 0, -1).perpendicular(Point(1, 1)).c << std::endl;
}
