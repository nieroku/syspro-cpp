#include "gtest/gtest.h"
#include "instance_limit.h"

namespace {
class Sample : public InstanceLimit<Sample, 5> {};
}  // namespace

TEST(InstanceLimitTest, MainTest) {
  Sample sample;
  Sample three[3] = {};
  Sample* on_heap = new Sample();
  ASSERT_THROW(Sample(), InstanceLimitExceeded);
  delete on_heap;
  ASSERT_NO_THROW(Sample());
}
