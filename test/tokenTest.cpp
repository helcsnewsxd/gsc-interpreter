#include "gsc/token.hpp"
#include "catch2/catch_amalgamated.hpp"

TEST_CASE("Token construction and access", "[token][constructor][access]") {
  SECTION("NUMBER type") {
    Token token(TokenType::NUMBER, "42", 42, 1);
    CHECK(token.getType() == TokenType::NUMBER);
    CHECK(token.getLexeme() == "42");
    CHECK(std::any_cast<int>(token.getLiteral()) == 42);
    CHECK(token.getLine() == 1);
  }

  SECTION("STRING type") {
    Token token(TokenType::STRING, "hello", nullptr, 2);
    CHECK(token.getType() == TokenType::STRING);
    CHECK(token.getLexeme() == "hello");
    CHECK(token.getLiteral().type() == typeid(std::nullptr_t));
    CHECK(token.getLine() == 2);
  }

  SECTION("PRINT type with char* literal construction") {
    Token token(TokenType::PRINT, "print", "hi", 2);
    CHECK(token.getType() == TokenType::PRINT);
    CHECK(token.getLexeme() == "print");
    CHECK(std::string(std::any_cast<const char *>(token.getLiteral())) == "hi");
    CHECK(token.getLine() == 2);
  }

  SECTION("PRINT type with string literal construction") {
    Token token(TokenType::PRINT, "print", std::string("hi"), 2);
    CHECK(token.getType() == TokenType::PRINT);
    CHECK(token.getLexeme() == "print");
    CHECK(std::any_cast<std::string>(token.getLiteral()) == "hi");
    CHECK(token.getLine() == 2);
  }
}

TEST_CASE("Token toString method", "[token][toString]") {
  SECTION("TRUE type") {
    Token token(TokenType::TRUE, "true", nullptr, 3);
    CHECK(token.toString() == "TRUE true true");
  }

  SECTION("FALSE type") {
    Token token(TokenType::FALSE, "false", nullptr, 4);
    CHECK(token.toString() == "FALSE false false");
  }

  SECTION("NUMBER type") {
    Token token(TokenType::NUMBER, "42", 42, 5);
    CHECK(token.toString() == "NUMBER 42 42");
  }

  SECTION("STRING type with char* literal") {
    Token token(TokenType::STRING, "hello", "world", 6);
    CHECK(token.toString() == "STRING hello world");
  }

  SECTION("STRING type with string literal") {
    Token token(TokenType::STRING, "hello", std::string("world"), 6);
    CHECK(token.toString() == "STRING hello world");
  }

  SECTION("IDENTIFIER type") {
    Token token(TokenType::IDENTIFIER, "var", "x", 7);
    CHECK(token.toString() == "IDENTIFIER var var");
  }

  SECTION("ANOTHER type") {
    Token token(TokenType::AND, "&&", nullptr, 8);
    CHECK(token.toString() == "AND && nil");
  }
}
