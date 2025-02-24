#include "angem.h"

#include <optional>

#include "gtest/gtest.h"

TEST(AngemTest, TestLine) {
  EXPECT_EQ(Line::fromPoints(Point(0, 0), Point(1, 1)).value(), Line(-1, 1, 0));
  EXPECT_EQ(Line::fromPoints(Point(1, 1), Point(1, 1)), std::nullopt);
}

TEST(AngemTest, TestIntersection) {
  Point common(1, 1);
  Line line1 = Line::fromPoints(Point(0, 0), common).value();
  Line line2 = Line::fromPoints(Point(0, 1), common).value();
  EXPECT_EQ(line1 & line2, common);
  EXPECT_EQ(Line(1, 0, 0) & Line(0, 1, 0), Point(0, 0));
  EXPECT_EQ(Line(42, 42, 0) & Line(42, 42, 42), std::nullopt);
}

TEST(AngemTest, TestPerpendicular) {
  EXPECT_EQ(Line(-1, 1, 0).perpendicular(Point(0, 0)), Line(1, 1, 0));
  EXPECT_EQ(Line(1, 0, -1).perpendicular(Point(1, 1)), Line(0, 1, -1));
}
