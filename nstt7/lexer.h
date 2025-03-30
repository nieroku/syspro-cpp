#ifndef LEXER_H
#define LEXER_H

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

  struct Span {
    size_t pos;
    size_t len;

    static const Span empty;
    bool operator==(Span const&) const = default;

    std::string_view view(std::string_view file) const {
      return file.substr(pos, len);
    }
  };

  Kind kind;
  Span span;

  std::string_view view(std::string_view file) const {
    return span.view(file);
  };
  bool operator==(Token const&) const = default;
};

inline const Token::Span Token::Span::empty{0, 0};

class Lexer {
  std::string_view file_;
  std::optional<Token> token;
  size_t pos = 0;

 public:
  Lexer() : Lexer(std::string_view()) {};
  explicit Lexer(std::string_view file) : file_(file) { (*this)++; }
  explicit Lexer(const char* file) : Lexer(std::string_view(file)) {}

  using difference_type = std::ptrdiff_t;
  using value_type = Token;

  std::string_view file() const { return file_; }

  Token const& operator*() const { return *token; }
  Token const* operator->() const { return token.operator->(); }

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
