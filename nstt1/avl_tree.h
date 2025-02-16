#include <cstddef>

class AvlTreeNode;

class AvlTree {
  friend AvlTreeNode;
  friend void swap(AvlTree& a, AvlTree& b);

 public:
  AvlTree();
  ~AvlTree();
  bool contains(int) const;
  void insert(int);
  void remove(int);

 private:
  AvlTreeNode* node_ = nullptr;

  ptrdiff_t balance() const;
  size_t height() const;
};
