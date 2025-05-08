#ifndef MATCHER_H
#define MATCHER_H

#include <utility>

template <typename Checker, typename... Args>
int getIndexOfFirstMatch(Checker&& check, Args&&... args) {
  int idx = -1;
  (... || (++idx, check(std::forward<Args>(args))));
  return idx;
}

#endif
