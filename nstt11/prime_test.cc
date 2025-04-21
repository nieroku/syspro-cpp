#include "prime.h"

#include "gtest/gtest.h"

TEST(NthPrimeTest, FindsNthPrime) {
  EXPECT_EQ(nth_prime_v<1>, 2);
  EXPECT_EQ(nth_prime_v<2>, 3);
  EXPECT_EQ(nth_prime_v<3>, 5);
  EXPECT_EQ(nth_prime_v<126>, 701);
  EXPECT_EQ(nth_prime_v<256>, 1619);
  EXPECT_EQ(nth_prime_v<512>, 3671);
}
