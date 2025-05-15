#include "gsc/token.hpp"
#include <utility>

Token::Token(TokenType type, std::string lexeme, std::any literal, int line)
    : type{type}, lexeme{std::move(lexeme)}, literal{std::move(literal)},
      line{line} {}

TokenType Token::getType() const { return type; }

std::string Token::getLexeme() const { return lexeme; }

std::any Token::getLiteral() const { return literal; }

int Token::getLine() const { return line; }

std::string Token::toString() const {
  std::string literal_str;

  switch (type) {
  case (TRUE):
    literal_str = "true";
    break;
  case (FALSE):
    literal_str = "false";
    break;
  case (NUMBER):
    literal_str = std::to_string(std::any_cast<int>(literal));
    break;
  case (STRING):
    try {
      literal_str = std::any_cast<std::string>(literal);
    } catch (const std::bad_any_cast &e) {
      literal_str =
          static_cast<std::string>(std::any_cast<const char *>(literal));
    }
    break;
  case (IDENTIFIER):
    literal_str = lexeme;
    break;
  default:
    literal_str = "nil";
    break;
  }

  return ::toString(type) + " " + lexeme + " " + literal_str;
}
