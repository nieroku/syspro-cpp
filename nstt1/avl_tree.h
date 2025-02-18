#include <cstddef>

class AvlTree {
  friend void swap(AvlTree& a, AvlTree& b);

 public:
  AvlTree();
  AvlTree(AvlTree const&);
  AvlTree& operator=(AvlTree const&);
  ~AvlTree();

  bool contains(int) const;
  void insert(int);
  void remove(int);

 private:
  class Node;
  Node* node_ = nullptr;

  ptrdiff_t balance() const;
  size_t height() const;
};
