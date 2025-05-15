#pragma once

#include "token.hpp"
#include <map>
#include <string_view>
#include <vector>

static const std::map<std::string_view, TokenType> keywords = {
    {"and", TokenType::AND},   {"or", TokenType::OR},
    {"if", TokenType::IF},     {"else", TokenType::ELSE},
    {"true", TokenType::TRUE}, {"false", TokenType::FALSE},
    {"for", TokenType::FOR},   {"while", TokenType::WHILE},
    {"nil", TokenType::NIL},   {"print", TokenType::PRINT},
    {"var", TokenType::VAR}};

/** @class Scanner
 * @brief Lexical analyzer for the GSC programming language.
 *
 * The Scanner class is responsible for tokenizing the input source code.
 * It reads the source code character by character and generates a list of
 * tokens that represent the structure of the code.
 */
class Scanner {
private:
  std::string_view program;
  std::vector<Token> tokens;
  int start = 0;
  int current = 0;
  int line = 1;

  /** @internal
   *
   * @brief Adds a token without literal value to the token list.
   *
   * @param type The type of the token to add.
   * @see addToken(TokenType type, std::any literal)
   */
  void addToken(TokenType type);

  /** @internal
   *
   * @brief Adds a token with a literal value to the token list.
   *
   * @param type The type of the token to add.
   * @param literal The literal value of the token (can be null).
   * @see addToken(TokenType type)
   */
  void addToken(TokenType type, std::any literal);

  /** @internal
   *
   * @brief Scans the next token from the source code.
   *
   * @note This function is responsible for identifying the type of token and
   * if isn't a valid token, it will report an error.
   * @see error(int line, const std::string &message)
   **/
  void scanToken();

  void identifier();
  void number();
  void string();

  bool match(const char &expected);
  char peek() const;
  char peekNext() const;

  bool isAtEnd() const;
  char advance();

public:
  /** @brief Constructs a Scanner object.
   *
   * @param program The source code to be scanned.
   *
   * @note The program is stored as a string_view to avoid unnecessary copies.
   */
  Scanner(std::string_view program);

  /** @brief Scans the entire source code and generates a list of tokens. */
  void scanTokens();

  std::vector<Token> getTokens() const;
};
