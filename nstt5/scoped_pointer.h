#include <utility>

template <typename T>
class ScopedPointer {
  T *ptr;

 public:
  ScopedPointer(T *ptr) : ptr(ptr) {}
  ScopedPointer(ScopedPointer const &other) = delete;
  ScopedPointer(ScopedPointer &&other) : ptr(other.ptr) { other.ptr = nullptr; }
  ScopedPointer &operator=(ScopedPointer other) {
    std::swap(ptr, other.ptr);
    return *this;
  }
  ~ScopedPointer() { delete ptr; };

  T &operator*() { return *ptr; };
  T const &operator*() const { return *ptr; };
  T *operator->() { return ptr; }
};
