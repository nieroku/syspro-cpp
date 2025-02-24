#include "angem.h"

#include <optional>

using std::nullopt;
using std::optional;

Point::Point(double x, double y) : x(x), y(y) {}

Line::Line(double a, double b, double c) : a(a), b(b), c(c) {}

Line::Line(Point const& p, Point const& q)
    : a(-(q.y - p.y)), b(q.x - p.x), c(-(a * p.x + b * p.y)) {}

std::array<double, 3> Line::coefficients() const { return {a, b, c}; }

Line Line::perpendicular(Point const& p) const {
  return Line(-b, a, b * p.x - a * p.y);
}

optional<Point> operator&(Line const& l1, Line const& l2) {
  double det = l1.a * l2.b - l1.b * l2.a;
  if (det == 0) return nullopt;
  return Point((l1.b * l2.c - l1.c * l2.b) / det,
               (l1.c * l2.a - l1.a * l2.c) / det);
}
