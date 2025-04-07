#ifndef BUF_IO_H
#define BUF_IO_H

#include <memory>
#include <vector>

#include "io.h"

class BufferedReader : public Reader {
  Reader& underlying;
  std::unique_ptr<char[]> buffer;
  size_t buf_size;
  std::string_view span;

 public:
  BufferedReader(Reader&, size_t buf_size = 4096);

  virtual bool good() const;
  virtual void nop();

  virtual Reader& operator>>(char&);
  virtual bool eof() const;
};

class BufferedWriter : public Writer {
  Writer& underlying;
  std::vector<char> buffer;

 public:
  BufferedWriter(Writer&, size_t buf_size = 4096);

  virtual bool good() const;
  virtual void nop();

  virtual Writer& operator<<(std::string_view);
  virtual BufferedWriter& flush();
};

class BufferedIo : public BufferedReader, public BufferedWriter {
  enum {
    read,
    write,
  } latest_op = read;

 public:
  BufferedIo(ReaderWriter&, size_t buf_size = 4096);

  virtual bool good() const;
  virtual void nop();

  virtual Reader& operator>>(char&);

  virtual Writer& operator<<(std::string_view);
  BufferedWriter& flush();
};

#endif
