#include "avl_tree.h"

#include <algorithm>

#include "gtest/gtest.h"

using std::array;

array<int, 12> nums = {5, 1, -1, 2, 6, 3, 11, 42, 7, 0, -3, 12};

TEST(AvlTreeTest, TestFindDelete) {
  AvlTree<int> tree;
  for (auto i = nums.begin(); i != nums.end(); i++) {
    for (auto j = i; j != nums.end(); j++)
      EXPECT_EQ(tree.find(*j), tree.end()) << j << '\n';
    tree.insert(*i);
    for (auto j = nums.begin(); j != i; j++)
      EXPECT_NE(tree.find(*j), tree.end());
  }
  //   for (auto i = nums.begin(); i != nums.end(); i++) {
  // for (auto j = i; j != nums.end(); j++)
  //   EXPECT_EQ(tree.find(*j), tree.end());
  // tree.remove(*i);
  // for (auto j = nums.begin(); j != i; j++)
  //   EXPECT_FALSE(tree.contains(*j));
  //   }
}

TEST(AvlTreeTest, TestInsertTwice) {
  AvlTree<int> tree;
  ASSERT_EQ(tree.find(0), tree.end());
  auto it = tree.insert(0);
  ASSERT_EQ(it, tree.find(0));
  ASSERT_EQ(it, tree.insert(0));
  ASSERT_NE(it, tree.end());
  // tree.remove(0);
  // ASSERT_FALSE(tree.contains(0));
}

TEST(AvlTreeTest, TestCopyConstructor) {
  AvlTree<int> original;
  original.insert(0);
  original.insert(42);
  AvlTree copy = original;
  EXPECT_NE(copy.find(0), copy.end());
  EXPECT_NE(copy.find(42), copy.end());
  copy.insert(-2);
  EXPECT_EQ(original.find(-2), original.end());
  // copy.remove(42);
  // EXPECT_TRUE(original.contains(42));
  // EXPECT_FALSE(copy.contains(42));
}

TEST(AvlTreeTest, TestAssignmentOperator) {
  AvlTree<int> first;
  for (const auto num : nums)
    first.insert(num);
  AvlTree<int> second;
  second.insert(-42);
  second = first;
  EXPECT_EQ(second.find(-42), second.end());
  for (const auto num : nums)
    EXPECT_NE(second.find(num), second.end());
// second.remove(42);
// EXPECT_TRUE(first.contains(42));
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
  first = first;
#pragma clang diagnostic pop
  for (const auto num : nums)
    EXPECT_NE(first.find(num), first.end());
}

TEST(AvlTreeTest, TestMoveSemantics) {
  AvlTree<int> original;
  for (const auto num : nums)
    original.insert(num);
  AvlTree moved = std::move(original);
  for (const auto num : nums)
    EXPECT_EQ(original.find(num), original.end());
  for (const auto num : nums)
    EXPECT_NE(moved.find(num), original.end());
  moved = std::move(moved);
  for (const auto num : nums)
    EXPECT_NE(moved.find(num), original.end());
}

TEST(AvlTreeTest, RangeBasedFor) {
  AvlTree<int> tree;
  for (const auto& _ : tree)
    FAIL();
  for (const auto num : nums)
    tree.insert(num);

  array<int, 12> sorted = nums;
  std::sort(sorted.begin(), sorted.end());
  {
    auto expected = sorted.begin();
    for (const auto& num : tree)
      EXPECT_EQ(num, *(expected++));
  }
  {
    auto expected = sorted.begin();
    for (auto it = tree.cbegin(); it != tree.cend(); it++)
      EXPECT_EQ(*it, *(expected++));
  }
  {
    auto expected = sorted.rbegin();
    for (auto it = tree.rbegin(); it != tree.rend(); it++)
      EXPECT_EQ(*it, *(expected++));
  }
  {
    auto expected = sorted.rbegin();
    for (auto it = tree.crbegin(); it != tree.crend(); it++)
      EXPECT_EQ(*it, *(expected++));
  }
}
