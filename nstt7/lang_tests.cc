#include "gtest/gtest.h"
#include "lang.h"
#include "parser.h"

using namespace std::literals;

TEST(LangTest, ExampleOne) {
  auto raw_expr =
      "(let K = (val 10) in"
      "(add (val 5) (var K))"
      ")";
  auto evaluated = parse(raw_expr)->eval();
  EXPECT_EQ(15, *evaluated);
  EXPECT_EQ("(val 15)"s, (std::string)*evaluated);
}
