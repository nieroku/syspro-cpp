#ifndef INSTANCE_LIMIT_H
#define INSTANCE_LIMIT_H

#include <cstddef>
#include <stdexcept>

class InstanceLimitExceeded : public std::logic_error {
 public:
  InstanceLimitExceeded() : std::logic_error("instance limit exceeded") {}
};

template <typename T, size_t L>
class InstanceLimit {
  static size_t counter;

 public:
  InstanceLimit() {
    if (counter == L)
      throw InstanceLimitExceeded();
    counter++;
  }

 protected:
  ~InstanceLimit() noexcept { counter--; }
};

template <typename T, size_t L>
size_t InstanceLimit<T, L>::counter = 0;

#endif
