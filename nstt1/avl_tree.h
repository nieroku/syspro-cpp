#include <algorithm>
#include <utility>

template <std::totally_ordered T>
class AvlTree {
  class Node;
  Node* node_ = nullptr;

 public:
  AvlTree() = default;

  AvlTree(AvlTree const& other)
      : node_(other.node_ ? new Node(*other.node_) : nullptr) {}

  AvlTree(AvlTree&& other) : node_(other.node_) { other.node_ = nullptr; }

  AvlTree& operator=(AvlTree other) {
    std::swap(node_, other.node_);
    return *this;
  }

  ~AvlTree() { delete node_; };

  bool contains(T value) const {
    return node_ ? node_->contains(value) : false;
  }

  void insert(T value) {
    if (node_)
      node_->insert(value);
    else
      node_ = new Node(value);
  }

  void remove(T value) {
    if (node_) node_ = node_->remove(value);
  }

 private:
  ptrdiff_t balance() const { return node_ ? node_->balance() : 0; }

  T extractMin() {
    if (node_->left_.height()) {
      T min = node_->left_.extractMin();
      node_->fix();
      return min;
    }
    T min = node_->value_;
    *this = std::move(node_->right_);
    return min;
  }

  size_t height() const { return node_ ? node_->height() : 0; }
};

template <std::totally_ordered T>
class AvlTree<T>::Node {
  friend AvlTree;

  T value_;
  size_t height_ = 1;
  AvlTree left_;
  AvlTree right_;

 public:
  Node(T value) : value_(value) {};

  Node(Node const& other)
      : value_(other.value_),
        height_(other.height_),
        left_(other.left_),
        right_(other.right_) {}

  Node(Node&& other)
      : value_(other.value_),
        height_(other.height_),
        left_(std::move(other.left_)),
        right_(std::move(other.right_)) {}

  Node& operator=(Node other) {
    std::swap(value_, other.value_);
    std::swap(height_, other.height_);
    std::swap(left_, other.left_);
    std::swap(right_, other.right_);
    return *this;
  }

  ~Node() = default;

  bool contains(T value) {
    if (value == value_) return true;
    return (value < value_ ? left_ : right_).contains(value);
  }

  void insert(T value) {
    if (value != value_) (value < value_ ? left_ : right_).insert(value);
    fix();
  }

  Node* remove(T value) {
    if (value != value_) {
      (value < value_ ? left_ : right_).remove(value);
      fix();
      return this;
    }
    if (left_.height() && right_.height()) {
      value_ = right_.extractMin();
      fix();
      return this;
    }
    Node* heir = nullptr;
    std::swap(heir, (left_.height() ? left_ : right_).node_);
    delete this;
    return heir;
  }

  ptrdiff_t balance() const { return right_.height() - left_.height(); }
  size_t height() const { return height_; }

 private:
  void fix() {
    switch (balance()) {
      case 2:
        if (right_.balance() < 0) right_.node_->rotateRight();
        rotateLeft();
        break;
      case -2:
        if (right_.balance() > 0) right_.node_->rotateLeft();
        rotateRight();
        break;
      default:
        update();
        break;
    }
  }

  void rotateLeft() {
    std::swap(value_, right_.node_->value_);
    std::swap(right_.node_->left_, right_.node_->right_);
    std::swap(left_, right_.node_->left_);
    std::swap(left_, right_);
    left_.node_->update();
    update();
  }

  void rotateRight() {
    std::swap(value_, left_.node_->value_);
    std::swap(left_.node_->left_, left_.node_->right_);
    std::swap(left_.node_->right_, right_);
    std::swap(left_, right_);
    right_.node_->update();
    update();
  }

  void update() { height_ = std::max(left_.height(), right_.height()) + 1; }
};
