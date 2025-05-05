#include "allocate.h"
#include "gtest/gtest.h"

static_assert(requires(void* memory) { allocate<0>(memory); });

TEST(AllocateTests, Array) {
  int array[4] = {0};
  allocate<sizeof(array)>(array, 0, 1, 2, 3);
  for (int i = 0; i < 4; i++)
    EXPECT_EQ(array[i], i);
}
