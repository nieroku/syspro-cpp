#include "avl_tree.h"

#include "gtest/gtest.h"

using std::array;

array<int, 12> nums = {5, 1, -1, 2, 6, 3, 11, 42, 7, 0, -3, 12};

TEST(AvlTreeTest, TestFindDelete) {
  AvlTree<int> tree;
  for (auto i = nums.begin(); i != nums.end(); i++) {
    for (auto j = i; j != nums.end(); j++) EXPECT_FALSE(tree.contains(*j));
    tree.insert(*i);
    for (auto j = nums.begin(); j != i; j++) EXPECT_TRUE(tree.contains(*j));
  }
  for (auto i = nums.begin(); i != nums.end(); i++) {
    for (auto j = i; j != nums.end(); j++) EXPECT_TRUE(tree.contains(*j));
    tree.remove(*i);
    for (auto j = nums.begin(); j != i; j++) EXPECT_FALSE(tree.contains(*j));
  }
}

TEST(AvlTreeTest, TestInsertTwice) {
  AvlTree<int> tree;
  ASSERT_FALSE(tree.contains(0));
  tree.insert(0);
  ASSERT_TRUE(tree.contains(0));
  tree.insert(0);
  ASSERT_TRUE(tree.contains(0));
  tree.remove(0);
  ASSERT_FALSE(tree.contains(0));
}

TEST(AvlTreeTest, TestDestructor) {
  AvlTree<int> empty_tree;
  AvlTree<int> singleton_tree;
  singleton_tree.insert(0);
  AvlTree<int> big_tree;
  for (const auto num : nums) big_tree.insert(num);
}

TEST(AvlTreeTest, TestCopyConstructor) {
  AvlTree<int> original;
  original.insert(0);
  original.insert(42);
  AvlTree copy = original;
  EXPECT_TRUE(copy.contains(0));
  EXPECT_TRUE(copy.contains(42));
  copy.insert(-2);
  EXPECT_FALSE(original.contains(-2));
  copy.remove(42);
  EXPECT_TRUE(original.contains(42));
  EXPECT_FALSE(copy.contains(42));
}

TEST(AvlTreeTest, TestAssignmentOperator) {
  AvlTree<int> first;
  for (const auto num : nums) first.insert(num);
  AvlTree<int> second;
  second.insert(-42);
  second = first;
  EXPECT_FALSE(second.contains(-42));
  for (const auto num : nums) EXPECT_TRUE(second.contains(num));
  second.remove(42);
  EXPECT_TRUE(first.contains(42));
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
  first = first;
#pragma clang diagnostic pop
  for (const auto num : nums) EXPECT_TRUE(first.contains(num));
}

TEST(AvlTreeTest, TestMoveSemantics) {
  AvlTree<int> original;
  for (const auto num : nums) original.insert(num);
  AvlTree moved = std::move(original);
  for (const auto num : nums) EXPECT_FALSE(original.contains(num));
  for (const auto num : nums) EXPECT_TRUE(moved.contains(num));
  moved = std::move(moved);
  for (const auto num : nums) EXPECT_TRUE(moved.contains(num));
}
