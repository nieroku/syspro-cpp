#include "parser.h"

#include <memory>

#include "lexer.h"

namespace {
class Keyword {
 public:
  enum Value {
    kw_val,
    kw_var,
    kw_add,
    kw_if,
    kw_then,
    kw_else,
    kw_let,
    kw_in,
    kw_function,
    kw_call,
  };

  Keyword(const Keyword &) = default;
  Keyword(Keyword &&) = default;
  Keyword &operator=(const Keyword &) = default;
  Keyword &operator=(Keyword &&) = default;
  Keyword(Value value) : value(value) {}
  operator std::string_view() const {
    using namespace std::literals;
    switch (value) {
      case kw_val:
        return "val"sv;
      case kw_var:
        return "var"sv;
      case kw_add:
        return "add"sv;
      case kw_if:
        return "if"sv;
      case kw_then:
        return "then"sv;
      case kw_else:
        return "else"sv;
      case kw_let:
        return "let"sv;
      case kw_in:
        return "in"sv;
      case kw_function:
        return "function"sv;
      case kw_call:
        return "call"sv;
    }
  }

 private:
  Value value;
};

}  // namespace

using enum Token::Kind;
using enum Keyword::Value;

static Token expect(Lexer &lex, Token::Kind kind) {
  if (lex->kind != kind)
    throw "unexpected token";
  return *(lex++);
}

static Token expect(Lexer &lex, Keyword kw) {
  if ((std::string_view)*lex != kw)
    throw "unexpected keyword";
  return *(lex++);
}

Expr parse(Lexer &lex) {
  Expr expr;
  expect(lex, left_parenthesis);
  std::string_view kw = expect(lex, word);
  if (kw == Keyword(kw_val)) {
    expr = std::make_shared<ValExpr>((integer)expect(lex, number));
  } else if (kw == Keyword(kw_var)) {
    expr = std::make_shared<VarExpr>(std::string(expect(lex, word)));
  } else if (kw == Keyword(kw_add)) {
    expr = std::make_shared<AddExpr>(parse(lex), parse(lex));
  } else if (kw == Keyword(kw_if)) {
    const auto e1 = parse(lex);
    const auto e2 = parse(lex);
    expect(lex, kw_then);
    const auto e_then = parse(lex);
    expect(lex, kw_else);
    const auto e_else = parse(lex);
    expr = std::make_shared<IfExpr>(e1, e2, e_then, e_else);
  } else if (kw == Keyword(kw_let)) {
    std::string_view id = expect(lex, word);
    expect(lex, equal_sign);
    const auto e_value = parse(lex);
    expect(lex, kw_in);
    const auto e_body = parse(lex);
    expr = std::make_shared<LetExpr>(std::string(id), e_value, e_body);
  } else if (kw == Keyword(kw_function)) {
    expr = std::make_shared<FuncExpr>(
        std::string(expect(lex, word)), parse(lex)
    );
  } else if (kw == Keyword(kw_call)) {
    expr = std::make_shared<CallExpr>(parse(lex), parse(lex));
  } else
    throw "invalid expression";
  expect(lex, right_parenthesis);
  return expr;
}

Expr parse(std::string_view file) {
  Lexer lex(file);
  return parse(lex);
}
