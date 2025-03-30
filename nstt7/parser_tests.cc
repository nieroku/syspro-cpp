#include "gtest/gtest.h"
#include "parser.h"

TEST(ParserTest, TestSingle) {
  std::array exprs{
      "(val 10)",
      "(var x)",
      "(add (val 1) (val -1))",
      "(let x = (val 10) in (var x))",
  };
  for (const auto expr : exprs)
    EXPECT_EQ(std::string(expr), (std::string)*parse(expr));
}
