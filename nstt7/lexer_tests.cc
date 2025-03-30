#include <sstream>

#include "gtest/gtest.h"
#include "lexer.h"

using enum Token::Kind;

template <>
std::string testing::PrintToString(const Token::Kind& kind) {
  switch (kind) {
    case Token::Kind::number:
      return "<number>";
    case Token::Kind::word:
      return "<word>";
    case Token::Kind::equal_sign:
      return "'='";
    case Token::Kind::left_parenthesis:
      return "'('";
    case Token::Kind::right_parenthesis:
      return "')'";
    case Token::Kind::bad:
      return "<bad>";
    case Token::Kind::eof:
      return "<eof>";
  }
}

template <>
std::string testing::PrintToString(const Token& token) {
  std::stringstream out;
  out << "token " << ::testing::PrintToString(token.kind) << " at "
      << "[" << token.span.pos << ", " << token.span.len << ")";
  return out.str();
}

TEST(LexerTest, TestEmpty) {
  Lexer lex("");
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(lex->kind, eof);
    lex++;
  }
}

TEST(LexerTest, TestSingleToken) {
  Lexer lex;

  lex = Lexer("word");
  EXPECT_EQ(*(lex++), Token(word, Token::Span{0, 4}));
  lex = Lexer("126");
  EXPECT_EQ(*(lex++), Token(number, Token::Span{0, 3}));
  lex = Lexer("(");
  EXPECT_EQ(*(lex++), Token(left_parenthesis, Token::Span{0, 1}));
  lex = Lexer(")");
  EXPECT_EQ(*(lex++), Token(right_parenthesis, Token::Span{0, 1}));
  lex = Lexer("=");
  EXPECT_EQ(*(lex++), Token(equal_sign, Token::Span{0, 1}));
}

TEST(LexerTest, TestExample) {
  Lexer lex(
      ""
      "(let K = (val 10) in\n"
      "  (add (val 5) (var K))"
      ")\n"
  );
  std::array expecteds{
      Token(left_parenthesis, Token::Span{0, 1}),
      Token(word, Token::Span{1, 3}),
      Token(word, Token::Span{5, 1}),
      Token(equal_sign, Token::Span{7, 1}),
      Token(left_parenthesis, Token::Span{9, 1}),
      Token(word, Token::Span{10, 3}),
      Token(number, Token::Span{14, 2}),
      Token(right_parenthesis, Token::Span{16, 1}),
      Token(word, Token::Span{18, 2}),
      Token(left_parenthesis, Token::Span{23, 1}),
      Token(word, Token::Span{24, 3}),
      Token(left_parenthesis, Token::Span{28, 1}),
      Token(word, Token::Span{29, 3}),
      Token(number, Token::Span{33, 1}),
      Token(right_parenthesis, Token::Span{34, 1}),
      Token(left_parenthesis, Token::Span{36, 1}),
      Token(word, Token::Span{37, 3}),
      Token(word, Token::Span{41, 1}),
      Token(right_parenthesis, Token::Span{42, 1}),
      Token(right_parenthesis, Token::Span{43, 1}),
      Token(right_parenthesis, Token::Span{44, 1}),
      Token(eof, Token::Span::empty),
  };
  for (const auto expected : expecteds)
    EXPECT_EQ(*(lex++), expected);
}

TEST(LexerTest, TokenView) {
  std::string_view file =
      "(let F ="
      "(function arg (add (var arg) (val 1))) in"
      "(let V = (val -1) in"
      "(call (var F) (var V))))";
  Lexer lex(file);
  std::array expecteds{
      "(",   "let", "F",   "=",    "(", "function", "arg", "(",   "add",
      "(",   "var", "arg", ")",    "(", "val",      "1",   ")",   ")",
      ")",   "in",  "(",   "let",  "V", "=",        "(",   "val", "-1",
      ")",   "in",  "(",   "call", "(", "var",      "F",   ")",   "(",
      "var", "V",   ")",   ")",    ")", ")",
  };
  std::cout << lex->view(file) << std::endl;
  for (const auto expected : expecteds)
    EXPECT_EQ((lex++)->view(file), expected);
}
