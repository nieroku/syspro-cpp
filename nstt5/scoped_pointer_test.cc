#include "scoped_pointer.h"

#include "counter.h"
#include "gtest/gtest.h"

TEST(ScopedPointerTest, TestPointerSemantics) {
  ScopedPointer counter{new Counter{}};
  ASSERT_EQ(counter->get(), 0);
  ASSERT_EQ((*counter).get(), 0);
  (*counter)++;
  ASSERT_EQ(counter->get(), 1);
  ASSERT_EQ((*counter).get(), 1);
}

TEST(ScopedPointerTest, TestRuleOfFive) {
  ScopedPointer guarded{new int{42}};
  ScopedPointer copied{new int(*guarded)};
  ASSERT_EQ(*guarded, *copied);
  ASSERT_NE(guarded.operator->(), copied.operator->());
  ScopedPointer moved = std::move(guarded);
  ASSERT_EQ(guarded.operator->(), nullptr);
  ASSERT_EQ(*moved, *copied);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
  copied = std::move(copied);
  ASSERT_EQ(*copied, *moved);
#pragma clang diagnostic pop
}
