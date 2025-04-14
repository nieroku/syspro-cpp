#include "gtest/gtest.h"
#include "parser.h"

using namespace std::literals;

TEST(LangTest, FirstExample) {
  auto raw_expr =
      "(let K = (val 10) in"
      "  (add (val 5) (var K))"
      ")";
  auto evaluated = parse(raw_expr)->eval();
  EXPECT_EQ(15, *evaluated);
  EXPECT_EQ("(val 15)"s, (std::string)*evaluated);
}

TEST(LangTest, SecondExample) {
  auto raw_expr =
      "(let A = (val 20) in"
      "  (let B = (val 30) in"
      "    (if (var A) (add (var B) (val 3))"
      "      then (val 10)"
      "      else (add (var B) (val 1)))))";
  auto evaluated = parse(raw_expr)->eval();
  EXPECT_EQ(31, *evaluated);
  EXPECT_EQ("(val 31)"s, (std::string)*evaluated);
}

TEST(LangTest, ThirdExample) {
  auto raw_expr =
      "(let F ="
      " (function arg (add (var arg) (val 1))) in"
      "   (let V = (val -1) in"
      "     (call (var F) (var V))))";
  auto evaluated = parse(raw_expr)->eval();
  EXPECT_EQ(0, *evaluated);
  EXPECT_EQ("(val 0)"s, (std::string)*evaluated);
}

TEST(LangTest, FourthExample) {
  auto raw_expr = "(add (var A) (var B))";
  EXPECT_ANY_THROW(parse(raw_expr)->eval());
}

TEST(LangTest, Fibonacci) {
  auto raw_expr =
      "(let fibonacci ="
      "  (function n"
      "    (if (val 2) (var n)"
      "      then (var n)"
      "      else"
      "        (add"
      "          (call (var fibonacci) (add (var n) (val -1)))"
      "          (call (var fibonacci) (add (var n) (val -2))))))"
      "  in (call (var fibonacci) (val 6)))";

  auto evaluated = parse(raw_expr)->eval();
  EXPECT_EQ(8, *evaluated);
  EXPECT_EQ("(val 8)"s, (std::string)*evaluated);
}

TEST(LangTest, EnableSharedFromThisForExprs) {
  const ValExpr val_expr(10);
  EXPECT_EQ((std::string)val_expr, (std::string)*val_expr.BaseExpr::eval());
  const FuncExpr func_expr("id", val_expr.BaseExpr::eval());
  EXPECT_EQ((std::string)func_expr, (std::string)*func_expr.BaseExpr::eval());
}
