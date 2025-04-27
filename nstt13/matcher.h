#ifndef MATCHER_H
#define MATCHER_H

#include <utility>

namespace detail {

template <typename Checker>
struct Helper {
  Checker& check;
  bool found = false;
  int idx = 0;

  Helper(Checker& check) : check(check) {}

  template <typename T>
  Helper& operator&&(T&& arg) {
    found = found || check(std::forward<T>(arg));
    if (!found) idx++;
    return *this;
  }

  operator int() { return idx; }
};

}  // namespace detail

template <typename Checker, typename... Args>
int getIndexOfFirstMatch(Checker&& check, Args&&... args) {
  return (detail::Helper{check} && ... && std::forward<Args>(args));
}

#endif
