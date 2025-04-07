#include "file_io.h"

#include <cstdio>

FileIo::FileIo(FILE* file) : file(file, &fclose) {}

FileIo FileIo::open(const char* path) { return FileIo{fopen(path, "a+")}; }
FileIo FileIo::tmp() { return tmpfile(); };

bool FileIo::good() const { return recent_good && !ferror(file.get()); };
void FileIo::nop() { recent_good = true; }

Reader& FileIo::operator>>(char& c) {
  int c_ = fgetc(file.get());
  if (c_ == EOF)
    recent_good = false;
  else
    c = c_;
  return *this;
}

bool FileIo::eof() const { return feof(file.get()); };

Writer& FileIo::operator<<(std::string_view data) {
  fwrite(data.begin(), 1, data.size(), file.get());
  return *this;
}
