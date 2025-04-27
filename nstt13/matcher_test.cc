#include "matcher.h"

#include "gtest/gtest.h"

TEST(GetIndexOfFirstMatchTest, Test) {
  auto bool_check = [](bool b) { return b; };
  double false_ref = 0.0;
  EXPECT_EQ(getIndexOfFirstMatch(bool_check, false, 0, false_ref, 123), 3);
}
