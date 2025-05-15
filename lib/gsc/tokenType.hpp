#pragma once

#include <string>

/** @enum TokenType
 * @brief Enum representing the different types of tokens in the GSC language.
 *
 * This enum is used to categorize tokens during lexical analysis.
 */
enum TokenType {
  // Single-character tokens.
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,

  // One or two character tokens.
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

  // Literals.
  IDENTIFIER,
  STRING,
  NUMBER,

  // Keywords.
  AND,
  OR,
  IF,
  ELSE,
  TRUE,
  FALSE,
  FOR,
  WHILE,
  NIL,
  PRINT,
  VAR,

  END_OF_FILE
};

static const std::string tokenStrings[] = {
    "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE",  "RIGHT_BRACE", "MINUS",
    "PLUS",       "SEMICOLON",   "SLASH",       "STAR",        "BANG",
    "BANG_EQUAL", "EQUAL",       "EQUAL_EQUAL", "GREATER",     "GREATER_EQUAL",
    "LESS",       "LESS_EQUAL",  "IDENTIFIER",  "STRING",      "NUMBER",
    "AND",        "OR",          "IF",          "ELSE",        "TRUE",
    "FALSE",      "FOR",         "WHILE",       "NIL",         "PRINT",
    "VAR",        "END_OF_FILE"};

std::string toString(TokenType type);
