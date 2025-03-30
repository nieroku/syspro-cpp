#include "lexer.h"

using enum Token::Kind;

static bool isSpace(char8_t c) { return c == ' ' || c == '\t' || c == '\n'; }

static bool isNumber(char8_t c) { return '0' <= c && c <= '9'; }

static bool isWordFirstChar(char8_t c) {
  return ('a' <= (c | 0x20) && (c | 0x20) <= 'z') || c == '_';
}

static bool isWordChar(char8_t c) { return isWordFirstChar(c) || isNumber(c); }

Lexer& Lexer::operator++() {
  char8_t c;

  token = Token{eof, Token::Span::empty};
  while (pos < file_.size() && isSpace(c = file_[pos]))
    pos++;
  if (pos >= file_.size())
    return *this;
  token->span = Token::Span{pos, 1};

  if (isWordFirstChar(c)) {
    token->kind = word;
    while (++pos < file_.size() && isWordChar(c = file_[pos]))
      token->span.len++;
  } else if (isNumber(c) || c == '-') {
    token->kind = number;
    while (++pos < file_.size() && isNumber(c = file_[pos]))
      token->span.len++;
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
  return *this;
}
