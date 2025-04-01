#include "gtest/gtest.h"
#include "lexer.h"

using namespace std::string_view_literals;

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

TEST(LexerTest, TestEmpty) {
  Lexer lex("");
  for (int i = 0; i < 5; i++)
    EXPECT_EQ((lex++)->kind, eof);
  lex++;
}

TEST(LexerTest, TestSingleToken) {
  std::array<std::pair<std::string_view, Token::Kind>, 5> samples{
      std::pair{"word"sv, word},
      std::pair{"126"sv, number},
      std::pair{"("sv, left_parenthesis},
      std::pair{")"sv, right_parenthesis},
      std::pair{"="sv, equal_sign},
  };
  for (const auto [file, kind] : samples)
    EXPECT_EQ(*Lexer(file), Token(kind, file));
}

TEST(LexerTest, TestExample) {
  const auto file =
      ""
      "(let K = (val 10) in\n"
      "  (add (val 5) (var K))"
      ")\n"sv;
  std::array expecteds{
      Token(left_parenthesis, file.substr(0, 1)),
      Token(word, file.substr(1, 3)),
      Token(word, file.substr(5, 1)),
      Token(equal_sign, file.substr(7, 1)),
      Token(left_parenthesis, file.substr(9, 1)),
      Token(word, file.substr(10, 3)),
      Token(number, file.substr(14, 2)),
      Token(right_parenthesis, file.substr(16, 1)),
      Token(word, file.substr(18, 2)),
      Token(left_parenthesis, file.substr(23, 1)),
      Token(word, file.substr(24, 3)),
      Token(left_parenthesis, file.substr(28, 1)),
      Token(word, file.substr(29, 3)),
      Token(number, file.substr(33, 1)),
      Token(right_parenthesis, file.substr(34, 1)),
      Token(left_parenthesis, file.substr(36, 1)),
      Token(word, file.substr(37, 3)),
      Token(word, file.substr(41, 1)),
      Token(right_parenthesis, file.substr(42, 1)),
      Token(right_parenthesis, file.substr(43, 1)),
      Token(right_parenthesis, file.substr(44, 1)),
      Token(eof, std::string_view{}),
  };
  Lexer lex(file);
  for (const auto expected : expecteds)
    EXPECT_EQ(*(lex++), expected);
}

TEST(LexerTest, TokenView) {
  std::string_view file =
      "(let F ="
      "  (function arg (add (var arg) (val 1))) in"
      "    (let V = (val -1) in"
      "      (call (var F) (var V))))"sv;
  std::array expecteds{
      "(",   "let", "F",   "=",    "(", "function", "arg", "(",   "add",
      "(",   "var", "arg", ")",    "(", "val",      "1",   ")",   ")",
      ")",   "in",  "(",   "let",  "V", "=",        "(",   "val", "-1",
      ")",   "in",  "(",   "call", "(", "var",      "F",   ")",   "(",
      "var", "V",   ")",   ")",    ")", ")",
  };

  Lexer lex(file);
  for (const auto expected : expecteds)
    EXPECT_EQ((std::string_view)*lex++, expected);
}
