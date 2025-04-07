#ifndef STRING_IO_H
#define STRING_IO_H

#include "io.h"

class StringIo : public virtual ReaderWriter {
  std::string string;
  size_t pos = 0;
  bool good_ = true;

 public:
  StringIo() {}
  StringIo(std::string string) : string(string) {}

  bool good() const { return good_; }
  void nop();

  Reader& operator>>(char&);
  bool eof() const;

  Writer& operator<<(std::string_view);
};

#endif
