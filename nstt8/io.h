#ifndef IO_H
#define IO_H

#include <stdexcept>
#include <string>
#include <string_view>

class Io {
 public:
  virtual bool good() const = 0;
  inline operator bool() const { return good(); }
  inline bool operator!() const { return !good(); };

  virtual void nop() = 0;

  virtual ~Io() = default;
};

class Reader : public virtual Io {
 public:
  virtual Reader& operator>>(char&) = 0;
  virtual bool eof() const = 0;
};

inline Reader& operator>>(Reader& r, std::string& line) {
  line.erase();
  char c;
  while (r >> c && c != '\n')
    line.push_back(c);
  return r;
}

template <typename T>
  requires std::unsigned_integral<T>
inline Reader& operator>>(Reader& r, T& value) {
  value = 0;
  char digit;
  while (r >> digit && std::isdigit(digit)) {
    if (std::numeric_limits<T>::max() / 10 < value)
      throw std::range_error("too large");
    value *= 10;
    if (std::numeric_limits<T>::max() - digit < value)
      throw std::range_error("too large");
    value += digit - '0';
  }
  r.nop();
  return r;
}

class Writer : public virtual Io {
 public:
  virtual Writer& operator<<(std::string_view) = 0;
};

inline Writer& operator<<(Writer& writer, const std::string& string) {
  return writer << (std::string_view)string;
};

template <typename T>
  requires requires(T t) { std::to_string(t); }
Writer& operator<<(Writer& writer, int value) {
  return writer << std::to_string(value);
}

class ReaderWriter : public virtual Reader, public virtual Writer {};

#endif
