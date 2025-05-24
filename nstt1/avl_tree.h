#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <algorithm>
#include <concepts>
#include <memory>
#include <string>
#include <utility>

namespace internal {

template <typename T>
struct defaults {
  using Identity = const T&;
  constexpr static auto IdentityOf = [](const T& value) -> const T& {
    return value;
  };
};

template <class CharT, class Traits, class Allocator>
struct defaults<std::basic_string<CharT, Traits, Allocator>> {
  using Identity = std::basic_string_view<CharT, Traits>;
  constexpr static auto IdentityOf = [](const auto& value) -> Identity {
    return value;
  };
};

}  // namespace internal

template <typename T,
          std::totally_ordered Identity = internal::defaults<T>::Identity,
          auto IdentityOf = internal::defaults<T>::IdentityOf>
  requires requires(const T& value) {
    { IdentityOf(value) } -> std::same_as<Identity>;
  }
class AvlTree {
  template <typename TreePtr, typename NodePtr>
  class Iterator;
  struct Node;

  std::unique_ptr<Node> node;

 public:
  using iterator = Iterator<AvlTree*, Node*>;
  using const_iterator = Iterator<const AvlTree*, const Node*>;

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  AvlTree() = default;
  AvlTree(const AvlTree& other) : AvlTree(other, (Node*)nullptr) {}
  AvlTree(AvlTree&&) = default;
  AvlTree& operator=(AvlTree other) {
    node = std::move(other.node);
    return *this;
  }
  ~AvlTree() = default;

  AvlTree(std::initializer_list<T> il) {
    for (auto&& e : il) insert(e);
  }

  bool empty() const { return !(bool)node; }

  iterator begin() {
    return empty() ? end() : iterator(this, node.get()).descendLeft();
  }
  iterator end() { return iterator(this); }

  const_iterator cbegin() const {
    return empty() ? cend() : const_iterator(this, node.get()).descendLeft();
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
    return iterator(this, findPos(value).node.get());
  }
  const_iterator find(Identity value) const {
    return const_iterator(this, findPos(value).node.get());
  }

  bool contains(Identity value) const { return find(value) != cend(); }

  iterator insert(const T& value)
    requires std::copy_constructible<T>
  {
    return insert(IdentityOf(value), [&](AvlTree& pos) {
      pos = AvlTree(std::forward<const T&>(value));
    });
  }

  iterator insert(T&& value)
    requires std::move_constructible<T>
  {
    return insert(IdentityOf(value),
                  [&](AvlTree& pos) { pos = AvlTree(std::forward<T>(value)); });
  }

  template <typename... Args>
  iterator emplace(Args&&... args)
    requires std::constructible_from<T, Args...>
  {
    AvlTree tree(args...);
    return insert(tree.identity(),
                  [&](AvlTree& pos) { pos = std::move(tree); });
  }

  iterator remove(iterator pos) {
    auto& root = *this;
    auto next = std::next(pos);
    auto& tree = pos.node->enclosingTree(*this);

    auto parent = pos.node->parent;
    auto left = std::move(tree.left());
    auto right = std::move(tree.right());
    if (left && right) {
      right.setParent(nullptr);
      tree = std::move(next.node->enclosingTree(right).extractAsMin(right));
      tree.setParent(parent);
      if (!left) std::unreachable();
      tree.attachLeft(std::move(left));
      tree.attachRight(std::move(right));
    } else if (left || right) {
      auto& succ = left ? left : right;
      succ.setParent(parent);
      tree = std::move(succ);
    } else {
      tree = AvlTree();
    }

    if (parent) parent->fix(root);
    return next;
  }

  iterator remove(const_iterator pos) { return remove((iterator)pos); }

 private:
  template <typename... Args>
  AvlTree(Args... args)
      : node(std::make_unique<Node>(std::forward<Args>(args)...)) {}

  AvlTree(const AvlTree& other, Node* parent)
      : node(other ? std::make_unique<Node>(*other.node, parent) : nullptr) {}

  AvlTree(AvlTree&& other, Node* parent) : AvlTree(std::move(other)) {
    setParent(parent);
  }

  operator bool() const { return !empty(); }

  AvlTree& left() { return node->left; }
  AvlTree& right() { return node->right; }
  const AvlTree& left() const { return node->left; }
  const AvlTree& right() const { return node->right; }

  void attachLeft(AvlTree&& tree) {
    left() = AvlTree(std::move(tree), asParent());
    updateHeight();
  }

  void attachRight(AvlTree&& tree) {
    right() = AvlTree(std::move(tree), asParent());
    updateHeight();
  }

  AvlTree::Node* parent() const { return node->parent; }
  AvlTree::Node* asParent() const { return node.get(); }
  void setParent(Node* parent) {
    if (node) node->parent = parent;
  }

  size_t height() const { return empty() ? 0 : node->height; };
  ptrdiff_t balance() const { return right().height() - left().height(); };

  Identity identity() const { return IdentityOf(node->value); }

  static auto& findPos(auto tree, Identity id, auto*&... parent) {
    if constexpr (sizeof...(parent)) std::forward<Node*&>(parent...) = nullptr;
    while (*tree && id != tree->identity()) {
      if constexpr (sizeof...(parent))
        std::forward<Node*&>(parent...) = tree->asParent();
      bool less = id < tree->identity();
      tree = &(less ? tree->left() : tree->right());
    }
    return *tree;
  }

  AvlTree& findPos(Identity id, auto*&... parent) {
    return findPos(this, id, parent...);
  }
  const AvlTree& findPos(Identity id, auto*&... parent) const {
    return findPos(this, id, parent...);
  }

  iterator insert(Identity id, auto insert) {
    auto& root = *this;
    Node* parent;
    auto& pos = findPos(id, parent);
    if (!pos) {
      insert(pos);
      pos.setParent(parent);
      if (parent) parent->fix(root);
    }
    return iterator(this, pos.node.get());
  }

  void fix(AvlTree& root) {
    auto old_height = height();
    switch (balance()) {
      case -2:
        if (left().balance() > 0) left().rotateLeft();
        rotateRight();
        break;
      case 2:
        if (right().balance() < 0) right().rotateRight();
        rotateLeft();
        break;
    }
    if (old_height != updateHeight() && parent()) parent()->fix(root);
  }

  void rotateLeft() {
    auto r = std::move(right());
    r.setParent(parent());
    attachRight(std::move(r.left()));
    r.attachLeft(std::move(*this));
    *this = std::move(r);
  }

  void rotateRight() {
    auto l = std::move(left());
    l.setParent(parent());
    attachLeft(std::move(l.right()));
    l.attachRight(std::move(*this));
    *this = std::move(l);
  }

  size_t updateHeight() {
    return node->height = std::max(left().height(), right().height()) + 1;
  }

  AvlTree extractAsMin(AvlTree& root) {
    AvlTree extracted = std::move(*this);
    *this = std::move(extracted.right());
    setParent(extracted.parent());
    if (extracted.parent()) extracted.parent()->fix(root);
    return extracted;
  }
};

template <typename T, std::totally_ordered Identity, auto IdentityOf>
  requires requires(const T& value) {
    { IdentityOf(value) } -> std::same_as<Identity>;
  }
struct AvlTree<T, Identity, IdentityOf>::Node {
  friend class AvlTree;

  Node* parent{};
  size_t height = 1;
  AvlTree left;
  AvlTree right;
  T value;

  template <typename... Args>
  Node(Args&&... args)
    requires std::constructible_from<T, Args...>
      : value(std::forward<Args>(args)...){};
  Node(const Node&) = delete;
  Node(Node&&) = delete;
  Node& operator=(const Node&) = delete;
  Node& operator=(Node&&) = delete;
  Node(const Node& other, Node* parent)
      : parent(parent),
        height(other.height),
        left(other.left, this),
        right(other.right, this),
        value(other.value) {}

  AvlTree& enclosingTree(AvlTree& root) {
    AvlTree* tree;
    if (parent) {
      bool is_left_child = this == parent->left.node.get();
      tree = &(is_left_child ? parent->left : parent->right);
    } else
      tree = &root;
    if (tree->node.get() != this) std::unreachable();
    return *tree;
  }

  void fix(AvlTree& root) { return enclosingTree(root).fix(root); }
};

template <typename T, std::totally_ordered Identity, auto IdentityOf>
  requires requires(const T& value) {
    { IdentityOf(value) } -> std::same_as<Identity>;
  }
template <typename TreePtr, typename NodePtr>
class AvlTree<T, Identity, IdentityOf>::Iterator {
  friend class AvlTree;

  TreePtr root{};
  NodePtr node{};

  Iterator(TreePtr root) : root(root) {}
  Iterator(TreePtr root, NodePtr node) : root(root), node(node) {}

 public:
  using difference_type = std::ptrdiff_t;
  using value_type = std::remove_reference_t<decltype((node->value))>;

  Iterator() = default;
  Iterator(const Iterator&) = default;
  Iterator(Iterator&&) = default;
  Iterator& operator=(const Iterator&) = default;
  Iterator& operator=(Iterator&&) = default;
  operator const_iterator() { return const_iterator(root, node); }

  value_type& operator*() const { return node->value; }
  value_type* operator->() const { return std::addressof(node->value); }

  Iterator& operator++() {
    if (node->right) {
      node = node->right.node.get();
      descendLeft();
    } else {
      NodePtr pred;
      do {
        pred = up();
        if (!node) break;
      } while (pred == node->right.asParent());
    }
    return *this;
  }

  Iterator operator++(int) {
    auto tmp = *this;
    ++*this;
    return tmp;
  }

  Iterator& operator--() {
    if (!node) {
      node = root->node.get();
      descendRight();
    } else if (node->left) {
      node = node->left.node.get();
      descendRight();
    } else {
      while (up() == node->left.asParent());
    }
    return *this;
  }

  Iterator operator--(int) {
    auto tmp = *this;
    --*this;
    return tmp;
  }

  bool operator==(const Iterator& other) const { return node == other.node; }

 private:
  operator iterator() {
    return iterator(const_cast<AvlTree*>(root), const_cast<Node&>(node));
  }

  Iterator& descendLeft() {
    while (node->left) node = node->left.node.get();
    return *this;
  }

  Iterator& descendRight() {
    while (node->right) node = node->right.node.get();
    return *this;
  }

  auto up() { return std::exchange(node, node->parent); }
};

#endif
