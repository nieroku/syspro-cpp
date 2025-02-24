#include <array>
#include <optional>

struct Point {
  double x;
  double y;

  Point(double x, double y);
};

class Line {
  double a, b, c;

 public:
  Line(double a, double b, double c);
  Line(Point const& p, Point const& q);

  std::array<double, 3> coefficients() const;
  Line perpendicular(Point const& p) const;

  friend std::optional<Point> operator&(Line const& l1, Line const& l2);
};
