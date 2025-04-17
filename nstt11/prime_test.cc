#include "prime.h"

#include "gtest/gtest.h"

TEST(NthPrimeTest, FindsNthPrime) {
  EXPECT_EQ(nth_prime<1>(), 2);
  EXPECT_EQ(nth_prime<2>(), 3);
  EXPECT_EQ(nth_prime<3>(), 5);
  EXPECT_EQ(nth_prime<126>(), 701);
  EXPECT_EQ(nth_prime<256>(), 1619);
  EXPECT_EQ(nth_prime<512>(), 3671);
}
