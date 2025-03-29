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
  while (pos < file.size() && isSpace(c = file[pos])) pos++;
  if (pos >= file.size()) return *this;
  token->span = Token::Span{pos, 1};

  if (isWordFirstChar(c)) {
    token->kind = word;
    while (++pos < file.size() && isWordChar(c = file[pos])) token->span.len++;
  } else if (isNumber(c) || c == '-') {
    token->kind = number;
    while (++pos < file.size() && isNumber(c = file[pos])) token->span.len++;
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
