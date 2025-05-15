#include "gsc/tokenType.hpp"
#include "catch2/catch_amalgamated.hpp"

TEST_CASE("toString for TokenType works", "[TokenType][toString]") {
  std::vector<std::pair<TokenType, std::string>> tokenCases = {
      {LEFT_PAREN, "LEFT_PAREN"},
      {RIGHT_PAREN, "RIGHT_PAREN"},
      {LEFT_BRACE, "LEFT_BRACE"},
      {RIGHT_BRACE, "RIGHT_BRACE"},
      {MINUS, "MINUS"},
      {PLUS, "PLUS"},
      {SEMICOLON, "SEMICOLON"},
      {SLASH, "SLASH"},
      {STAR, "STAR"},
      {BANG, "BANG"},
      {BANG_EQUAL, "BANG_EQUAL"},
      {EQUAL, "EQUAL"},
      {EQUAL_EQUAL, "EQUAL_EQUAL"},
      {GREATER, "GREATER"},
      {GREATER_EQUAL, "GREATER_EQUAL"},
      {LESS, "LESS"},
      {LESS_EQUAL, "LESS_EQUAL"},
      {IDENTIFIER, "IDENTIFIER"},
      {STRING, "STRING"},
      {NUMBER, "NUMBER"},
      {AND, "AND"},
      {OR, "OR"},
      {IF, "IF"},
      {ELSE, "ELSE"},
      {TRUE, "TRUE"},
      {FALSE, "FALSE"},
      {FOR, "FOR"},
      {WHILE, "WHILE"},
      {NIL, "NIL"},
      {PRINT, "PRINT"},
      {VAR, "VAR"},
      {END_OF_FILE, "END_OF_FILE"}};

  for (const auto &[tokenType, expectedString] : tokenCases) {
    REQUIRE(::toString(tokenType) == expectedString);
  }
}
