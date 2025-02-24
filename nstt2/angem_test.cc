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
  const auto [a1, b1, c1] = l1.coefficients();
  const auto [a2, b2, c2] = l2.coefficients();
  double norm1 = lineNorm(a1, b1);
  double norm2 = lineNorm(a2, b2);
  return almostEquals(b1 / norm1, b2 / norm2) &&
         almostEquals(c1 / norm1, c2 / norm2);
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
  EXPECT_EQ(Line(1, 0, -1).perpendicular(Point(1, 1)), Line(0, 1, -1));
}
