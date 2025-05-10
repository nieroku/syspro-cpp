#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <concepts>
#include <memory>
#include <string>
#include <utility>

namespace internal {

template <typename T>
struct defaults {
  using Identity = const T&;
  constexpr static auto IdentityOf = std::identity();
};

template <class CharT, class Traits, class Allocator>
struct defaults<std::basic_string<CharT, Traits, Allocator>> {
  using Identity = std::basic_string_view<CharT, Traits>;
  constexpr static auto IdentityOf = std::basic_string<
      CharT,
      Traits,
      Allocator>::operator std::basic_string_view<CharT, Traits>;
};

}  // namespace internal

template <
    typename T,
    typename Identity = internal::defaults<T>::Identity,
    auto IdentityOf = internal::defaults<T>::IdentityOf>
requires requires(const T& value) {
  { IdentityOf(value) } -> std::same_as<Identity>;
} class AvlTree {
  template <bool nonconst>
  class Iterator;
  class Node;

  std::unique_ptr<Node> node_;

 public:
  using iterator = Iterator<true>;
  using const_iterator = Iterator<false>;

  static_assert(std::bidirectional_iterator<iterator>);
  static_assert(std::bidirectional_iterator<const_iterator>);

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  AvlTree() = default;
  AvlTree(const AvlTree& other) {
    if (other)
      node_ = std::make_unique<Node>(*other.node_);
  }
  AvlTree(AvlTree&&) = default;
  AvlTree& operator=(AvlTree other) {
    node_ = std::move(other.node_);
    return *this;
  }
  ~AvlTree() = default;

  bool empty() const { return !(bool)node_; }

  iterator begin() {
    return empty() ? end() : iterator(this, node_.get()).descendLeft();
  }
  iterator end() { return iterator(this); }

  const_iterator cbegin() const {
    return empty() ? cend() : const_iterator(this, node_.get()).descendLeft();
  }
  const_iterator cend() const { return const_iterator(this); }

  reverse_iterator rbegin() { return reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }

  const_reverse_iterator crbegin() const {
    return const_reverse_iterator(cend());
  }
  const_reverse_iterator crend() const {
    return const_reverse_iterator(cbegin());
  }

  iterator find(Identity value) {
    return iterator(this, findTree(value).node_.get());
  }
  const_iterator find(Identity value) const {
    return const_iterator(this, findTree(value).node_.get());
  }

  iterator insert(const T& value) requires std::copy_constructible<T> {
    auto& tree = findTree(IdentityOf(value));
    if (!tree)
      tree = AvlTree(std::make_unique<Node>(value));
    return iterator(this, tree.node_.get());
  }

 private:
  AvlTree(std::unique_ptr<Node>&& node) : node_(std::move(node)) {}

  operator bool() const { return !empty(); }

  static auto& findTree_(auto tree, Identity id) {
    while (*tree && id != IdentityOf(tree->node_->value_))
      tree = id < IdentityOf(tree->node_->value_) ? &tree->node_->left_
                                                  : &tree->node_->right_;
    return *tree;
  }

  AvlTree& findTree(Identity id) { return findTree_(this, id); }
  const AvlTree& findTree(Identity id) const { return findTree_(this, id); }
};

template <typename T, typename Identity, auto IdentityOf>
requires requires(const T& value) {
  { IdentityOf(value) } -> std::same_as<Identity>;
} class AvlTree<T, Identity, IdentityOf>::Node {
  friend class AvlTree;

  AvlTree left_;
  AvlTree right_;
  size_t height = 0;
  T value_;

 public:
  Node(const T& value) : value_(value) {};
  Node(const Node&) = default;
  Node(Node&&) = default;
  ~Node() = default;
};

template <typename T, typename Identity, auto IdentityOf>
requires requires(const T& value) {
  { IdentityOf(value) } -> std::same_as<Identity>;
} template <bool nonconst>
class AvlTree<T, Identity, IdentityOf>::Iterator {
  friend class AvlTree;

  std::conditional_t<nonconst, AvlTree, const AvlTree>* root_{};
  std::conditional_t<nonconst, AvlTree::Node, const AvlTree::Node>* node_{};

  Iterator(decltype(root_) root) : root_(root) {}
  Iterator(decltype(root_) root, decltype(node_) node)
      : root_(root), node_(node) {}

 public:
  using difference_type = std::ptrdiff_t;
  using value_type = std::conditional_t<nonconst, T, const T>;

  Iterator() = default;
  Iterator(const Iterator&) = default;
  Iterator(Iterator&&) = default;
  Iterator& operator=(const Iterator&) = default;
  Iterator& operator=(Iterator&&) = default;
  operator const_iterator() { return const_iterator(root_, node_); }

  value_type& operator*() const { return node_->value_; }
  value_type* operator->() const { return &node_->value_; }

  Iterator& operator++() {
    if (node_->right_) {
      node_ = node_->right_.node_.get();
      descendLeft();
    } else {
      Node* pred;
      do {
        pred = up();
        if (!node_)
          break;
      } while (pred == node_->right_.node_.get());
    }
    return *this;
  }

  Iterator operator++(int) {
    auto tmp = *this;
    ++*this;
    return tmp;
  }

  Iterator& operator--() {
    if (!node_) {
      node_ = root_->node_.get();
      descendRight();
    } else if (node_->left_) {
      node_ = node_->left_.node_.get();
      descendRight();
    } else {
      Node* pred;
      do {
        pred = up();
        if (!node_)
          break;
      } while (pred == node_->left_.node_.get());
    }
    return *this;
  }

  Iterator operator--(int) {
    auto tmp = *this;
    --*this;
    return tmp;
  }

  bool operator==(const Iterator& other) const { return node_ == other.node_; }

 private:
  Iterator& descendLeft() {
    while (node_->left_)
      node_ = node_->left_.node_.get();
    return *this;
  }

  Iterator& descendRight() {
    while (node_->right_)
      node_ = node_->right_.node_.get();
    return *this;
  }

  Node* up() { return const_cast<Node*>(std::exchange(node_, node_->parent_)); }
};

#endif
