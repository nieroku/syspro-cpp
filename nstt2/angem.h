#include <optional>

struct Point {
  double x;
  double y;

  Point(double x, double y);

  friend bool operator==(Point const&, Point const&);
};

class Line {
  double a, b, c;

 public:
  Line(double a, double b, double c);

  static std::optional<Line> fromPoints(Point const&, Point const&);

  Line perpendicular(Point const& p) const;

  friend std::optional<Point> operator&(Line const&, Line const&);
  friend bool operator==(Line const&, Line const&);

 private:
  Line(Point const& p, Point const& q);
};
