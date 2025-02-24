#include <cstddef>

class AvlTree {
 public:
  AvlTree();
  AvlTree(AvlTree const&);
  AvlTree(AvlTree&&);
  AvlTree& operator=(AvlTree);
  ~AvlTree();

  bool contains(int) const;
  void insert(int);
  void remove(int);

 private:
  class Node;
  Node* node_ = nullptr;

  ptrdiff_t balance() const;
  int extractMin();
  size_t height() const;
};
