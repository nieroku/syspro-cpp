#include <cstddef>
#include <vector>

class Matrix {
  size_t size;
  double *data;

  Matrix(size_t, double *);

 public:
  static Matrix undefined(size_t n);
  static Matrix zero(size_t n);
  static Matrix unit(size_t n);
  explicit Matrix(std::vector<double> const &);

  Matrix(Matrix const &);
  Matrix(Matrix &&);
  Matrix &operator=(Matrix);
  ~Matrix();

  class Row {
    friend class Matrix;

    double *data;

    Row(double *data);

   public:
    double &operator[](size_t column);
    double const &operator[](size_t column) const;
  };

  Row operator[](size_t row);
  const Row operator[](size_t row) const;

  bool operator==(Matrix const &) const;
  bool operator!=(Matrix const &) const;

  Matrix operator+(Matrix const &);
  Matrix &operator+=(Matrix const &);
  Matrix operator*(double);
  Matrix &operator*=(double);
  Matrix operator*(Matrix const &);
  Matrix &operator*=(Matrix const &);

  double trace() const;
  explicit operator double() const;
};
