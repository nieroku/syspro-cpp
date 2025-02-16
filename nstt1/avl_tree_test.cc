#include "avl_tree.h"

#include "gtest/gtest.h"

using std::array;

array<int, 12> nums = {5, 1, -1, 2, 6, 3, 11, 4, 7, 0, -3, 12};

TEST(AvlTreeTest, TestFindDelete) {
  AvlTree tree;
  for (auto i = nums.begin(); i != nums.end(); i++) {
    for (auto j = i; j != nums.end(); j++) EXPECT_FALSE(tree.contains(*j));
    tree.insert(*i);
    for (auto j = nums.begin(); j != i; j++) EXPECT_TRUE(tree.contains(*j));
  }
  for (auto i = nums.begin(); i != nums.end(); i++) {
    for (auto j = i; j != nums.end(); j++)
      EXPECT_TRUE(tree.contains(*j)) << *i << std::endl;
    tree.remove(*i);
    for (auto j = nums.begin(); j != i; j++) EXPECT_FALSE(tree.contains(*j));
  }
}

TEST(AvlTreeTest, TestInsertTwice) {
  AvlTree tree;
  ASSERT_FALSE(tree.contains(0));
  tree.insert(0);
  ASSERT_TRUE(tree.contains(0));
  tree.insert(0);
  ASSERT_TRUE(tree.contains(0));
  tree.remove(0);
  ASSERT_FALSE(tree.contains(0));
}
