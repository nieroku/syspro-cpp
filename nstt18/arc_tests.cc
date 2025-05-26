#include "arc.h"
#include "gtest/gtest.h"

TEST(ArcTests, Lifetime) {
  Arc arc(new int);
  Arc copied(arc);
  Arc moved(std::move(copied));
  ASSERT_EQ(copied.get(), nullptr);
  Arc<int> assigned;
  assigned = moved;
}

TEST(ArcTests, UpgradeSuccess) {
  Arc arc(new int);
  Arc once_weakened = arc.downgrade().upgrade();
  ASSERT_EQ(once_weakened.get(), arc.get());
}

TEST(ArcTests, UpgradeFailure) {
  auto weak = Arc(new int).downgrade();
  auto upgraded = weak.upgrade();
  ASSERT_EQ(upgraded.get(), nullptr);
}
