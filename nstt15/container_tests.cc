#include "container.h"
#include "gtest/gtest.h"

struct Point {
  int x, y;

  Point(int x, int y) : x(x), y(y) {}

  bool operator==(const Point&) const = default;
};

TEST(ContainerTests, Example) {
  Container<int, char, Point> c(12, 'c', Point{2, 3});
  EXPECT_EQ(c.getElement<int>(0), 12);
  EXPECT_EQ(c.getElement<char>(1), 'c');
  EXPECT_EQ(c.getElement<Point>(2), (Point{2, 3}));
}
