#include "gtest/gtest.h"
#include "string_io.h"

TEST(StringIOTest, BaseTest) {
  StringIo io("Hello");
  io << ", world!";
  char c;
  std::string str;
  while (io >> c)
    str.push_back(c);
  ASSERT_EQ(str, "Hello, world!");

  io << "123";
  unsigned x;
  ASSERT_TRUE(io >> x);
  EXPECT_EQ(x, 123);
  ASSERT_TRUE(io.eof());
}
