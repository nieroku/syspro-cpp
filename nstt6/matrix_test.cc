#include "matrix.h"

#include "gtest/gtest.h"

TEST(MatrixTest, TestConstructors) {
  EXPECT_EQ(Matrix::zero(2), Matrix(std::vector<double>{0, 0}));
  EXPECT_EQ(Matrix::unit(2), Matrix(std::vector<double>{1, 1}));
}

TEST(MatrixTest, TestRuleOfFive) {
  Matrix source = Matrix::unit(2);
  Matrix copied = source;
  ASSERT_EQ(source, copied);
  Matrix moved = std::move(source);
  ASSERT_EQ(copied, moved);
  ASSERT_EQ(source, Matrix::zero(0));
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
  copied = copied;
  ASSERT_EQ(copied, moved);
  copied = std::move(copied);
  ASSERT_EQ(copied, moved);
#pragma clang diagnostic pop
}

TEST(MatrixTest, TestCheckedIndexing) {
  Matrix matrix(std::vector<double>{1, 2});
  EXPECT_EQ(1, matrix[0][0]);
  EXPECT_EQ(0, matrix[0][1]);
  EXPECT_EQ(0, matrix[1][0]);
  EXPECT_EQ(2, matrix[1][1]);
  EXPECT_EQ(1, matrix.at(0).at(0));
  EXPECT_EQ(0, matrix.at(0).at(1));
  EXPECT_EQ(0, matrix.at(1).at(0));
  EXPECT_EQ(2, matrix.at(1).at(1));
  EXPECT_THROW(matrix.at(3), std::out_of_range);
  EXPECT_THROW(matrix.at(2).at(3), std::out_of_range);
}

TEST(MatrixTest, TestComparison) {
  EXPECT_NE(Matrix::zero(2), Matrix::unit(2));
  EXPECT_EQ(Matrix::unit(2), Matrix::unit(2));
}

TEST(MatrixTest, TestAddition) {
  EXPECT_EQ(Matrix::unit(2), Matrix::zero(2) + Matrix::unit(2));
  Matrix matrix(std::vector<double>{1, 2});
  matrix += Matrix{std::vector<double>{2, 1}};
  Matrix unit = Matrix::unit(2);
  EXPECT_EQ(matrix, unit + unit + unit);

  EXPECT_THROW(Matrix::unit(2) + Matrix::unit(3), Matrix::size_mismatch);
  EXPECT_THROW(matrix += Matrix::unit(3), Matrix::size_mismatch);
}

TEST(MatrixTest, TestScalarMul) {
  EXPECT_EQ(Matrix(std::vector<double>{2, 3}) * -1,
            Matrix(std::vector<double>{-2, -3}));
  Matrix matrix = Matrix::unit(2);
  matrix *= 0;
  EXPECT_EQ(matrix, Matrix::zero(2));
}

TEST(MatrixTest, TestMatMul) {
  EXPECT_EQ(Matrix::unit(2) * Matrix::zero(2), Matrix::zero(2));
  Matrix matrix(std::vector<double>{2, 3});
  matrix *= Matrix(std::vector<double>{3, 2});
  EXPECT_EQ(matrix, Matrix::unit(2) * 6);

  EXPECT_THROW(Matrix::unit(2) * Matrix::unit(3), Matrix::size_mismatch);
  EXPECT_THROW(matrix *= Matrix::unit(3), Matrix::size_mismatch);
}

TEST(MatrixTest, TestTrace) {
  EXPECT_EQ(0, (double)Matrix::zero(0));
  EXPECT_EQ(1, (double)Matrix::unit(1));
  EXPECT_EQ(3, (double)Matrix(std::vector<double>{1, 2}));
}
