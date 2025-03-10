#include "matrix.h"

#include <algorithm>
#include <cstdio>
#include <utility>

Matrix::Matrix(size_t n, double* data) : size(n), data(data) {}

Matrix Matrix::undefined(size_t n) { return Matrix(n, new double[n * n]); }

Matrix Matrix::zero(size_t n) { return Matrix(n, new double[n * n]{}); }

Matrix Matrix::unit(size_t n) {
  Matrix matrix(n, new double[n * n]{});
  for (size_t i = 0; i < n; i++) matrix[i][i] = 1;
  return matrix;
}

Matrix::Matrix(std::vector<double> const& diagonal)
    : Matrix(Matrix::zero(diagonal.size())) {
  for (size_t i = 0; i < size; i++) {
    (*this)[i][i] = diagonal[i];
  }
}

Matrix::Matrix(Matrix const& other) : Matrix(Matrix::undefined(other.size)) {
  std::copy(other.data, other.data + size * size, data);
}

Matrix::Matrix(Matrix&& other) : Matrix(other.size, other.data) {
  other.size = 0;
  other.data = 0;
}

Matrix& Matrix::operator=(Matrix other) {
  std::swap(size, other.size);
  std::swap(data, other.data);
  return *this;
}

Matrix::~Matrix() { delete[] data; }

Matrix::Row Matrix::operator[](size_t row) {
  return Row(data + row * size, size);
}
const Matrix::Row Matrix::operator[](size_t row) const {
  return Row(data + row * size, size);
}
Matrix::Row Matrix::at(size_t row) {
  if (row >= size) throw std::out_of_range("row out of range");
  return (*this)[row];
}
const Matrix::Row Matrix::at(size_t row) const {
  if (row >= size) throw std::out_of_range("row out of range");
  return (*this)[row];
}

Matrix::Row::Row(double* data, size_t length) : data(data), length(length) {}

double& Matrix::Row::operator[](size_t column) { return data[column]; }
double const& Matrix::Row::operator[](size_t column) const {
  return data[column];
}
double& Matrix::Row::at(size_t column) {
  if (column >= length) throw std::out_of_range("column out of range");
  return (*this)[column];
}
double const& Matrix::Row::at(size_t column) const {
  if (column >= length) throw std::out_of_range("column out of range");
  return (*this)[column];
}

bool Matrix::operator==(Matrix const& other) const {
  if (size != other.size) return false;
  return std::equal(data, data + size * size, other.data);
}
bool Matrix::operator!=(Matrix const& other) const { return !(*this == other); }

Matrix Matrix::operator+(Matrix const& other) {
  if (other.size != size) throw size_mismatch();
  Matrix result = Matrix::undefined(size);
  std::transform(data, data + size * size, other.data, result.data,
                 std::plus<>());
  return result;
}

Matrix& Matrix::operator+=(Matrix const& other) {
  if (other.size != size) throw size_mismatch();
  std::transform(data, data + size * size, other.data, data, std::plus<>());
  return *this;
}

Matrix Matrix::operator*(double multiplier) {
  Matrix result = Matrix::undefined(size);
  std::transform(data, data + size * size, result.data,
                 [=](double x) { return x * multiplier; });
  return result;
};

Matrix& Matrix::operator*=(double multiplier) {
  std::transform(data, data + size * size, data,
                 [=](double x) { return x * multiplier; });
  return *this;
}

Matrix Matrix::operator*(Matrix const& other) {
  if (other.size != size) throw size_mismatch();
  Matrix result = Matrix::zero(size);
  for (size_t i = 0; i < size; i++)
    for (size_t k = 0; k < size; k++)
      for (size_t j = 0; j < size; j++)
        result[i][j] += (*this)[i][k] * other[k][j];
  return result;
}

Matrix& Matrix::operator*=(Matrix const& other) {
  if (other.size != size) throw size_mismatch();
  *this = (*this) * other;
  return *this;
}

double Matrix::trace() const {
  double trace = 0.0;
  for (size_t i = 0; i < size; i++) trace += (*this)[i][i];
  return trace;
}

Matrix::operator double() const { return trace(); }
