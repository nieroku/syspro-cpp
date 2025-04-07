#include "buf_io.h"

BufferedReader::BufferedReader(Reader& r, size_t buf_size)
    : underlying(r), buffer(new char[buf_size]), buf_size(buf_size) {}

bool BufferedReader::good() const { return underlying.good(); }

void BufferedReader::nop() { underlying.nop(); }

Reader& BufferedReader::operator>>(char& c) {
  if (span.empty()) {
    size_t len = 0;
    while (len < buf_size && underlying >> buffer[len++])
      ;
    if (!len)
      return *this;
    nop();
    span = std::string_view(&buffer[0], &buffer[len]);
  }
  c = span[0];
  span = span.substr(1);
  return *this;
}

bool BufferedReader::eof() const { return !span.length() && underlying.eof(); }

BufferedWriter::BufferedWriter(Writer& w, size_t buf_size) : underlying(w) {
  buffer.reserve(buf_size);
}

bool BufferedWriter::good() const { return underlying; }

void BufferedWriter::nop() { underlying.nop(); }

Writer& BufferedWriter::operator<<(std::string_view data) {
  if (data.size() > buffer.capacity() - buffer.size()) {
    if (!flush())
      return *this;
    if (data.size() > buffer.capacity())
      return underlying << data;
  }
  buffer.insert(buffer.end(), data.begin(), data.end());
  underlying.nop();
  return *this;
}

BufferedWriter& BufferedWriter::flush() {
  if (underlying << std::string_view(buffer.begin(), buffer.end()))
    buffer.clear();
  return *this;
}

BufferedIo::BufferedIo(ReaderWriter& rw, size_t buf_size)
    : BufferedReader(rw, buf_size), BufferedWriter(rw, buf_size) {}

bool BufferedIo::good() const {
  switch (latest_op) {
    case read:
      return BufferedReader::good();
    case write:
      return BufferedWriter::good();
  }
}

void BufferedIo::nop() {
  switch (latest_op) {
    case read:
      BufferedReader::nop();
      break;
    case write:
      BufferedWriter::nop();
      break;
  }
}

Reader& BufferedIo::operator>>(char& c) {
  flush();
  if (!*this)
    return (BufferedReader&)*this;
  return BufferedReader::operator>>(c);
}

Writer& BufferedIo::operator<<(std::string_view data) {
  latest_op = write;
  return BufferedWriter::operator<<(data);
}

BufferedWriter& BufferedIo::flush() {
  latest_op = write;
  return BufferedWriter::flush();
}
