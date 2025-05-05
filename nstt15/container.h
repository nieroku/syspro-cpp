#ifndef CONTAINER_H
#define CONTAINER_H

#include <cstddef>
#include <utility>

template <typename... Types>
class Container;

template <typename Head, typename... Tail>
class Container<Head, Tail...> {
  Head head;
  Container<Tail...> tail;

 public:
  Container(Head head, Tail... tail) : head(head), tail(tail...) {}

  template <typename V>
  V getElement(size_t idx) {
    if (idx == 0)
      std::unreachable();
    return tail.template getElement<V>(idx - 1);
  }

  template <>
  Head getElement<Head>(size_t idx) {
    if (idx == 0)
      return head;
    return tail.template getElement<Head>(idx - 1);
  }
};

template <>
class Container<> {
 public:
  template <typename T>
  T getElement(size_t) {
    std::unreachable();
  }
};

#endif
