#include <optional>

struct Point {
  double x;
  double y;

  Point(double x, double y);
};

struct Line {
  double a, b, c;

  Line(double a, double b, double c);
  Line(Point const& p, Point const& q);

  Line perpendicular(Point const& p) const;

  friend std::optional<Point> operator&(Line const& l1, Line const& l2);
};
