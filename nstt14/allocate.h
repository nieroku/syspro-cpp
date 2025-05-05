#ifndef ALLOCATE_H
#define ALLOCATE_H

#include <concepts>
#include <cstddef>

template <size_t SIZE, typename _ = void>
void allocate(void* memory) {}

template <size_t SIZE, typename T, typename... U>
  requires(std::copy_constructible<T> && ... && std::copy_constructible<U>)
          && (SIZE >= (sizeof(T) + ... + sizeof(U)))
void allocate(void* memory, const T& head, const U&... tail) {
  new (memory) T(head);
  allocate<SIZE>((char*)memory + sizeof(T), tail...);
}

#endif
