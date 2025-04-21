#include <algorithm>
#include <memory>
#include <string_view>
#include <utility>

template <typename T, std::totally_ordered I = const T&>
  requires requires(T t) { static_cast<I>(t); }
class AvlTree {
  class Node;
  class Iterator;
  std::unique_ptr<Node> node_;

 public:
  AvlTree() = default;

  AvlTree(const AvlTree& other) {
    if (!other.empty())
      node_ = std::make_unique<Node>(*other.node_);
  }

  AvlTree(AvlTree&& other) = default;

  AvlTree& operator=(AvlTree other) {
    node_ = std::move(other.node_);
    return *this;
  }

  ~AvlTree() = default;

  using iterator = Iterator;
  static_assert(std::forward_iterator<iterator>);

  iterator begin() const {
    return empty() ? end() : iterator(node_.get()).descend();
  }

  iterator end() const { return iterator(); }

  iterator find(I value) const { return find(value, nullptr); }

  iterator insert(T value) { return insert(value, nullptr); }

  // void remove(I value) {
  //   if (node_)
  //     node_ = node_->remove(value);
  // }

  bool empty() const { return !node_; }

 private:
  ptrdiff_t balance() const { return node_ ? node_->balance() : 0; }
  size_t height() const { return node_ ? node_->height() : 0; }

  iterator find(I value, Node* parent) const {
    return node_.get() ? node_->find(value, parent) : end();
  }

  iterator insert(T value, Node* parent) {
    if (node_)
      return node_->insert(value, parent);
    else {
      node_ = std::make_unique<Node>(value, parent);
      return iterator(node_.get());
    }
  }

  // T extractMin() {
  //   if (node_->left_.height()) {
  //     T min = node_->left_.extractMin();
  //     node_->fix();
  //     return min;
  //   }
  //   T min = node_->value_;
  //   *this = std::move(node_->right_);
  //   return min;
  // }
};

template <typename T, std::totally_ordered I>
  requires requires(T t) { static_cast<I>(t); }
class AvlTree<T, I>::Node {
  friend AvlTree;

  T value_;
  size_t height_ = 1;
  AvlTree left_;
  AvlTree right_;
  Node* parent_;

 public:
  Node(T value, Node* parent) : value_(value), parent_(parent) {};

  iterator find(I value, Node* parent) {
    parent_ = parent;
    auto id = static_cast<I>(value_);
    if (value == id)
      return Iterator(this);
    return (value < id ? left_ : right_).find(value, this);
  }

  iterator insert(T value, Node* parent) {
    parent_ = parent;
    auto id = static_cast<I>(value_);
    if (value != id) {
      auto it = (value < id ? left_ : right_).insert(value, this);
      fix();
      return it;
    }
    return Iterator(this);
  }

  // void remove(I value) {
  //   auto id = static_cast<I>(value_);
  //   if (value != id) {
  //     (value < id ? left_ : right_).remove(value);
  //     fix();
  //     return this;
  //   }
  //   if (left_.height() && right_.height()) {
  //     value_ = right_.extractMin();
  //     fix();
  //     return this;
  //   }
  //   Node* heir = nullptr;
  //   std::swap(heir, (left_.height() ? left_ : right_).node_);
  //   delete this;
  //   return heir;
  // }

  ptrdiff_t balance() const { return right_.height() - left_.height(); }
  size_t height() const { return height_; }

 private:
  void fix() {
    switch (balance()) {
      case 2:
        if (right_.balance() < 0)
          right_.node_->rotateRight();
        rotateLeft();
        break;
      case -2:
        if (right_.balance() > 0)
          right_.node_->rotateLeft();
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

template <typename T, std::totally_ordered I>
  requires requires(T t) { static_cast<I>(t); }
class AvlTree<T, I>::Iterator {
  friend class AvlTree;

  Node* node_ = nullptr;
  bool ascending = false;

 public:
  using difference_type = std::ptrdiff_t;
  using value_type = T;

  Iterator() = default;

  const T& operator*() const { return node_->value_; }
  const T* operator->() const { return &node_->value_; }

  Iterator& operator++() {
    if (!ascending && !node_->right_.empty()) {
      node_ = node_->right_.node_.get();
      descend();
    } else
      ascend();
    return *this;
  }

  Iterator operator++(int) {
    auto tmp = *this;
    ++*this;
    return tmp;
  }

  bool operator==(const Iterator& other) const { return node_ == other.node_; }

 private:
  Iterator& ascend() {
    Node* prev;
    do {
      prev = node_;
      node_ = node_->parent_;
    } while (node_ && (ascending = prev == node_->right_.node_.get()));
    return *this;
  }

  Iterator& descend() {
    while (!node_->left_.empty()) {
      auto prev = node_;
      node_ = node_->left_.node_.get();
      node_->parent_ = prev;
    }
    ascending = false;
    return *this;
  }

  Iterator(Node* node) : node_(node) {}
};
