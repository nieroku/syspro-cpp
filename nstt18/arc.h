#ifndef ARC_H
#define ARC_H

#include <atomic>
#include <cstddef>
#include <utility>

template <typename T>
class Arc {
  struct Counters {
    std::atomic<size_t> strong = 1;
    std::atomic<size_t> weak = 1;
  };

  T* element{};
  Counters* counters{};

 public:
  class Weak;

  Arc() = default;
  Arc(T* element) : element(element) {
    if (!*this)
      return;
    try {
      counters = new Counters;
    } catch (...) {
      delete element;
      throw;
    }
  }

  Arc(const Arc& other) : Arc(other.element, other.counters) {
    if (*this) {
      counters->strong.fetch_add(1, std::memory_order::relaxed);
      counters->weak.fetch_add(1, std::memory_order::relaxed);
    }
  };

  Arc(Arc&& other)
      : Arc(std::exchange(other.element, nullptr), other.counters) {}

  Arc& operator=(Arc other) {
    std::swap(element, other.element);
    std::swap(counters, other.counters);
    return *this;
  }

  Weak downgrade() const;

  T* get() const { return element; }
  T* operator->() const { return element; }

  T& operator*() const { return *element; }

  explicit operator bool() { return element; }

  ~Arc() {
    if (!*this)
      return;
    if (counters->strong.fetch_sub(1, std::memory_order::acq_rel) - 1 == 0)
      delete element;
    if (counters->weak.fetch_sub(1, std::memory_order::acq_rel) - 1 == 0)
      delete counters;
  }

 private:
  Arc(T* element, Counters* counters) : element(element), counters(counters) {}
};

template <typename T>
class Arc<T>::Weak {
  T* element{};
  Counters* counters{};

 public:
  Weak() = default;
  Weak(const Arc& arc) : element(arc.element), counters(arc.counters) {
    if (element)
      counters->weak.fetch_add(1, std::memory_order::relaxed);
  }

  Weak(const Weak& other) : element(other.element), counters(other.counters) {
    if (element)
      counters->weak.fetch_add(1, std::memory_order::relaxed);
  };

  Weak(Weak&& other)
      : element(std::exchange(other.element, nullptr)),
        counters(other.counters) {}

  Weak& operator=(Weak other) {
    std::swap(element, other.element);
    std::swap(counters, other.counters);
    return *this;
  }

  Arc<T> upgrade() const;

  ~Weak() {
    if (!element)
      return;
    if (counters->weak.fetch_sub(1, std::memory_order::acq_rel) - 1 == 0)
      delete counters;
  }
};

template <typename T>
Arc<T> Arc<T>::Weak::upgrade() const {
  if (!element)
    return Arc();
  size_t expected = 0;
  while (!counters->strong.compare_exchange_weak(
      expected,
      expected ? expected + 1 : 0,
      std::memory_order::release,
      std::memory_order::relaxed
  ))
    continue;
  if (!expected)
    return Arc();
  counters->weak.fetch_add(1, std::memory_order::relaxed);
  return Arc(element, counters);
}

template <typename T>
typename Arc<T>::Weak Arc<T>::downgrade() const {
  return Weak(*this);
}

#endif
