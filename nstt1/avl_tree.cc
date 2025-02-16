#include "avl_tree.h"

#include <algorithm>
#include <utility>

using std::swap;

class AvlTreeNode {
 public:
  AvlTreeNode(int value) {
    value_ = value;
    height_ = 1;
  };

  ~AvlTreeNode() = default;

  bool contains(int value) {
    if (value == value_) return true;
    return (value < value_ ? left_ : right_).contains(value);
  }

  void insert(int value) {
    if (value != value_) (value < value_ ? left_ : right_).insert(value);
    fix();
  }

  AvlTreeNode* remove(int value) {
    if (value != value_) {
      if (value < value_)
        left_.remove(value);
      else
        right_.remove(value);
      fix();
      return this;
    }
    if (left_.height() && right_.height()) {
      right_.node_ = right_.node_->extractMin(value_);
      fix();
      return this;
    }
    AvlTreeNode* heir = nullptr;
    swap(heir, (left_.height() ? left_ : right_).node_);
    delete this;
    return heir;
  }

  ptrdiff_t balance() const { return right_.height() - left_.height(); }
  size_t height() const { return height_; }

 private:
  int value_;
  size_t height_;
  AvlTree left_;
  AvlTree right_;

  AvlTreeNode* extractMin(int& to) {
    if (left_.height()) {
      left_.node_ = left_.node_->extractMin(to);
      fix();
      return this;
    }
    to = value_;
    AvlTreeNode* heir = nullptr;
    swap(right_.node_, heir);
    delete this;
    return heir;
  }

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
    swap(value_, right_.node_->value_);
    swap(right_.node_->left_, right_.node_->right_);
    swap(left_, right_.node_->left_);
    swap(left_, right_);
    left_.node_->update();
    update();
  }

  void rotateRight() {
    swap(value_, left_.node_->value_);
    swap(left_.node_->left_, left_.node_->right_);
    swap(left_.node_->right_, right_);
    swap(left_, right_);
    right_.node_->update();
    update();
  }

  void update() { height_ = std::max(left_.height(), right_.height()) + 1; }
};

AvlTree::AvlTree() = default;
AvlTree::~AvlTree() { delete node_; };

bool AvlTree::contains(int value) const {
  return node_ ? node_->contains(value) : false;
}

void AvlTree::insert(int value) {
  if (node_)
    node_->insert(value);
  else
    node_ = new AvlTreeNode(value);
}

void AvlTree::remove(int value) {
  if (node_) node_ = node_->remove(value);
}

ptrdiff_t AvlTree::balance() const { return node_ ? node_->balance() : 0; }
size_t AvlTree::height() const { return node_ ? node_->height() : 0; }

void swap(AvlTree& a, AvlTree& b) { swap(a.node_, b.node_); }
