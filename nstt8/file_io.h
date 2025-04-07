#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>

#include <memory>

#include "io.h"

class FileIo : public ReaderWriter {
  std::unique_ptr<FILE, decltype(&fclose)> file;
  bool recent_good;

 public:
  static FileIo open(const char* path);
  static FileIo tmp();

  bool good() const;
  void nop();

  Reader& operator>>(char&);
  bool eof() const;

  Writer& operator<<(std::string_view);

 private:
  FileIo(FILE* file);
};

#endif
