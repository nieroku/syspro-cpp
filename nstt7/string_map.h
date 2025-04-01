#ifndef STRING_MAP_H
#define STRING_MAP_H

#include <string>
#include <string_view>
#include <unordered_map>

namespace {
struct StringMapHash {
  using is_transparent = void;
  size_t operator()(std::string_view str) const {
    return std::hash<std::string_view>{}(str);
  }
  size_t operator()(const std::string& str) const {
    return std::hash<std::string>{}(str);
  }
};
}  // namespace

template <class T>
using StringMap =
    std::unordered_map<std::string, T, StringMapHash, std::equal_to<>>;

#endif
