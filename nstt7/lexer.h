#ifndef LEXER_H
#define LEXER_H

#include <lang.h>

#include <iterator>
#include <optional>
#include <string_view>

struct Token {
  enum class Kind {
    number,
    word,
    equal_sign,
    left_parenthesis,
    right_parenthesis,
    bad,
    eof,
  };

  Kind kind;
  std::string_view span;

  operator std::string_view() const { return span; };
  operator integer() const;

  bool operator==(const Token&) const = default;
};

class Lexer {
  std::string_view file;
  std::optional<Token> token;
  size_t pos = 0;

 public:
  Lexer() : Lexer(std::string_view()) {};
  explicit Lexer(std::string_view file) : file(file) { (*this)++; }
  explicit Lexer(const char* file) : Lexer(std::string_view(file)) {}

  using difference_type = std::ptrdiff_t;
  using value_type = Token;

  const Token& operator*() const { return *token; }
  const Token* operator->() const { return token.operator->(); }

  Lexer& operator++();
  Lexer operator++(int) {
    Lexer copy = *this;
    ++*this;
    return copy;
  };

 private:
  void init();
};

static_assert(std::input_iterator<Lexer>);

#endif
