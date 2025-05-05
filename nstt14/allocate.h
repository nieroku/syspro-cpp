#ifndef ALLOCATE_H
#define ALLOCATE_H

#include <concepts>
#include <cstddef>
#include <new>

namespace internal {

template <typename T, typename... U>
void allocate(char* memory, const T& head, const U&... tail) {
  new (memory) T(head);
  if constexpr (sizeof...(U) > 0)
    allocate(memory + sizeof(T), tail...);
}

}  // namespace internal

template <size_t SIZE, typename... Types>
  requires(std::copy_constructible<Types> && ...)
          && (SIZE >= (0 + ... + sizeof(Types)))
void allocate(void* memory, const Types&... args) {
  if constexpr (sizeof...(Types) > 0)
    internal::allocate((char*)memory, args...);
}

#endif
