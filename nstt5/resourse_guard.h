#include <utility>

template <typename T>
class ResourceGuard {
  T *ptr;

 public:
  ResourceGuard(T *ptr) : ptr(ptr) {}
  ResourceGuard(ResourceGuard const &other)
      : ptr(other.ptr ? new T(*other.ptr) : nullptr) {}
  ResourceGuard(ResourceGuard &&other) : ptr(other.ptr) { other.ptr = nullptr; }
  ResourceGuard &operator=(ResourceGuard other) {
    std::swap(ptr, other.ptr);
    return *this;
  }
  ~ResourceGuard() { delete ptr; };

  T &operator*() { return *ptr; };
  T const &operator*() const { return *ptr; };
  T *operator->() { return ptr; }
};
