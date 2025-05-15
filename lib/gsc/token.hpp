#pragma once

#include "tokenType.hpp"
#include <any>
#include <string>

/** @class Token
 * @brief Represents a token in the source code.
 *
 * The Token class encapsulates the type, lexeme, literal value, and line number
 * of a token. It provides a method to convert the token to a string
 * representation.
 */
class Token {
private:
  const TokenType type;
  const std::string lexeme;
  const std::any literal;
  const int line;

public:
  /** @brief Constructs a Token object.
   *
   * @param type The type of the token.
   * @param lexeme The lexeme of the token.
   * @param literal The literal value of the token (if any).
   * @param line The line number where the token was found.
   *
   * @note The literal value can be of any type, so it is stored as std::any.
   * @note The lexeme and literal values are moved into the object to avoid
   * copying.
   */
  Token(TokenType type, std::string lexeme, std::any literal, int line);

  TokenType getType() const;

  std::string getLexeme() const;

  std::any getLiteral() const;

  int getLine() const;

  std::string toString() const;
};
