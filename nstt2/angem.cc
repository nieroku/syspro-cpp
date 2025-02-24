#include "angem.h"

#include <cmath>
#include <limits>
#include <optional>

static bool almostEquals(double a, double b) {
  auto abs_max = fabs(fmax(a, b));
  auto eps = std::numeric_limits<double>::epsilon();
  if (abs_max != 0) eps *= abs_max;
  return std::fabs(a - b) < eps;
}

static double lineNorm(double x, double y) {
  return std::sqrt(x * x + y * y) * (x < 0 ? -1 : 1);
}

Point::Point(double x, double y) : x(x), y(y) {}

bool operator==(Point const& p, Point const& q) {
  return almostEquals(p.x, q.x) && almostEquals(p.y, q.y);
}

std::optional<Line> Line::fromPoints(Point const& p, Point const& q) {
  if (p == q) return std::nullopt;
  return Line(p, q);
}

Line::Line(Point const& p, Point const& q)
    : a(-(q.y - p.y)), b(q.x - p.x), c(-(a * p.x + b * p.y)) {}

Line::Line(double a, double b, double c) : a(a), b(b), c(c) {}

Line Line::perpendicular(Point const& p) const {
  return Line(-b, a, b * p.x - a * p.y);
}

std::optional<Point> operator&(Line const& l1, Line const& l2) {
  double det = l1.a * l2.b - l1.b * l2.a;
  if (almostEquals(det, 0)) return std::nullopt;
  return Point((l1.b * l2.c - l1.c * l2.b) / det,
               (l1.c * l2.a - l1.a * l2.c) / det);
}

bool operator==(Line const& l1, Line const& l2) {
  double norm1 = lineNorm(l1.a, l1.b);
  double norm2 = lineNorm(l2.a, l2.b);
  return almostEquals(l1.b / norm1, l2.b / norm2) &&
         almostEquals(l1.c / norm1, l2.c / norm2);
}
