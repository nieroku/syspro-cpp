#include "resourse_guard.h"

#include "gtest/gtest.h"
#include "counter.h"

TEST(ResourceGuardTest, TestPointerSemantics) {
  ResourceGuard counter{new Counter{}};
  ASSERT_EQ(counter->get(), 0);
  ASSERT_EQ((*counter).get(), 0);
  (*counter)++;
  ASSERT_EQ(counter->get(), 1);
  ASSERT_EQ((*counter).get(), 1);
}

TEST(ResourceGuardTest, TestRuleOfFive) {
  ResourceGuard guarded{new int{42}};
  ResourceGuard copied = guarded;
  ASSERT_EQ(*guarded, *copied);
  ASSERT_NE(guarded.operator->(), copied.operator->());
  ResourceGuard moved = std::move(guarded);
  ASSERT_EQ(guarded.operator->(), nullptr);
  ASSERT_EQ(*moved, *copied);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
  copied = copied;
  ASSERT_EQ(*copied, *moved);
  copied = std::move(copied);
  ASSERT_EQ(*copied, *moved);
#pragma clang diagnostic pop
}
