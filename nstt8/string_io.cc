#include "string_io.h"

void StringIo::nop() { good_ = true; }

Reader& StringIo::operator>>(char& c) {
  good_ = pos < string.size();
  if (good_)
    c = string[pos++];
  return *this;
}
bool StringIo::eof() const { return pos == string.size(); }

Writer& StringIo::operator<<(std::string_view data) {
  string.append(data);
  nop();
  return *this;
}
