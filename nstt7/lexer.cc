#include "lexer.h"

#include <charconv>

using enum Token::Kind;

Token::operator integer() const {
  integer tmp;
  auto [_, ec] = std::from_chars(span.data(), span.data() + span.size(), tmp);
  if (ec == std::errc::result_out_of_range)
    throw "out of range";
  return tmp;
}

static bool isSpace(char c) { return c == ' ' || c == '\t' || c == '\n'; }

static bool isNumber(char c) { return '0' <= c && c <= '9'; }

static bool isWordFirstChar(char c) {
  return ('a' <= (c | 0x20) && (c | 0x20) <= 'z') || c == '_';
}

static bool isWordChar(char c) { return isWordFirstChar(c) || isNumber(c); }

Lexer& Lexer::operator++() {
  char c;

  token = Token{eof, std::string_view{}};
  while (pos < file.size() && isSpace(c = file[pos]))
    pos++;
  if (pos >= file.size())
    return *this;

  const auto begin = pos;
  if (isWordFirstChar(c)) {
    token->kind = word;
    while (++pos < file.size() && isWordChar(c = file[pos]))
      ;
  } else if (isNumber(c) || c == '-') {
    token->kind = number;
    while (++pos < file.size() && isNumber(c = file[pos]))
      ;
  } else {
    switch (c) {
      case '=':
        token->kind = equal_sign;
        break;
      case '(':
        token->kind = left_parenthesis;
        break;
      case ')':
        token->kind = right_parenthesis;
        break;
      default:
        token->kind = bad;
    }
    pos++;
  }
  token->span = file.substr(begin, pos - begin);
  return *this;
}
