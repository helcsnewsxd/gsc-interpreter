#include "gsc/scanner.hpp"
#include "catch2/catch_amalgamated.hpp"
#include "gsc/error.hpp"
#include <iostream>
#include <sstream>

void checkTokenSize(const std::vector<Token> &tokens, int expectedSize) {
  REQUIRE(static_cast<int>(tokens.size()) == expectedSize);
}

void checkEOFToken(const Token &token, int line = 1) {
  CHECK(token.getType() == TokenType::END_OF_FILE);
  CHECK(token.getLexeme() == "");
  CHECK(token.getLiteral().type() == typeid(std::nullptr_t));
  CHECK(token.getLine() == line);
}

void checkSimpleTokens(const Token &token, const TokenType &tokenType,
                       std::string_view lexeme, int line = 1) {
  CHECK(token.getType() == tokenType);
  CHECK(token.getLexeme() == lexeme);
  CHECK(token.getLiteral().type() == typeid(std::nullptr_t));
  CHECK(token.getLine() == line);
}

void checkStringToken(const Token &token, std::string word, int line = 1) {
  CHECK(token.getType() == TokenType::STRING);
  CHECK(token.getLexeme() == "\"" + word + "\"");
  CHECK(token.getLiteral().type() == typeid(std::string));
  CHECK(std::any_cast<std::string>(token.getLiteral()) == word);
  CHECK(token.getLine() == line);
}

void checkNumberToken(const Token &token, int number, int line = 1) {
  CHECK(token.getType() == TokenType::NUMBER);
  CHECK(token.getLexeme() == std::to_string(number));
  CHECK(token.getLiteral().type() == typeid(int));
  CHECK(std::any_cast<int>(token.getLiteral()) == number);
  CHECK(token.getLine() == line);
}

TEST_CASE("Scanner test for null program", "[scanner][null_token]") {
  SECTION("Empty program") {
    Scanner scanner{""};
    scanner.scanTokens();

    std::vector<Token> tokens = scanner.getTokens();
    checkTokenSize(tokens, 1);
    checkEOFToken(tokens[0], 1);
  }

  SECTION("With spaces") {
    Scanner scanner{" \t\r"};
    scanner.scanTokens();

    std::vector<Token> tokens = scanner.getTokens();
    checkTokenSize(tokens, 1);
    checkEOFToken(tokens[0], 1);
  }

  SECTION("With newlines") {
    Scanner scanner{"\n\n\n"};
    scanner.scanTokens();

    std::vector<Token> tokens = scanner.getTokens();
    checkTokenSize(tokens, 1);
    checkEOFToken(tokens[0], 4);
  }

  SECTION("With comments") {
    Scanner scanner{"// This is a comment\n"};
    scanner.scanTokens();

    std::vector<Token> tokens = scanner.getTokens();
    checkTokenSize(tokens, 1);
    checkEOFToken(tokens[0], 2);
  }
}

TEST_CASE("Scanner test for single tokens", "[scanner][single_token]") {
  SECTION("Left parenthesis") {
    Scanner scanner{"("};
    scanner.scanTokens();

    std::vector<Token> tokens = scanner.getTokens();
    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::LEFT_PAREN, "(");
    checkEOFToken(tokens[1], 1);
  }

  SECTION("Right parenthesis") {
    Scanner scanner{")"};
    scanner.scanTokens();

    std::vector<Token> tokens = scanner.getTokens();
    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::RIGHT_PAREN, ")");
    checkEOFToken(tokens[1], 1);
  }

  SECTION("Left brace") {
    Scanner scanner{"{"};
    scanner.scanTokens();

    std::vector<Token> tokens = scanner.getTokens();
    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::LEFT_BRACE, "{");
    checkEOFToken(tokens[1], 1);
  }

  SECTION("Right brace") {
    Scanner scanner{"}"};
    scanner.scanTokens();

    std::vector<Token> tokens = scanner.getTokens();
    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::RIGHT_BRACE, "}");
    checkEOFToken(tokens[1], 1);
  }

  SECTION("Minus") {
    Scanner scanner{"-"};
    scanner.scanTokens();

    std::vector<Token> tokens = scanner.getTokens();
    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::MINUS, "-");
    checkEOFToken(tokens[1], 1);
  }

  SECTION("Plus") {
    Scanner scanner{"+"};
    scanner.scanTokens();

    std::vector<Token> tokens = scanner.getTokens();
    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::PLUS, "+");
    checkEOFToken(tokens[1], 1);
  }

  SECTION("Semicolon") {
    Scanner scanner{";"};
    scanner.scanTokens();

    std::vector<Token> tokens = scanner.getTokens();
    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::SEMICOLON, ";");
    checkEOFToken(tokens[1], 1);
  }

  SECTION("Semicolon") {
    Scanner scanner{";"};
    scanner.scanTokens();

    std::vector<Token> tokens = scanner.getTokens();
    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::SEMICOLON, ";");
    checkEOFToken(tokens[1], 1);
  }

  SECTION("Star") {
    Scanner scanner{"*"};
    scanner.scanTokens();

    std::vector<Token> tokens = scanner.getTokens();
    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::STAR, "*");
    checkEOFToken(tokens[1], 1);
  }

  SECTION("Bang") {
    Scanner scanner{"!"};
    scanner.scanTokens();

    std::vector<Token> tokens = scanner.getTokens();
    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::BANG, "!");
    checkEOFToken(tokens[1], 1);
  }

  SECTION("Bang equal") {
    Scanner scanner{"!="};
    scanner.scanTokens();

    std::vector<Token> tokens = scanner.getTokens();
    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::BANG_EQUAL, "!=");
    checkEOFToken(tokens[1], 1);
  }

  SECTION("Equal") {
    Scanner scanner{"="};
    scanner.scanTokens();

    std::vector<Token> tokens = scanner.getTokens();
    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::EQUAL, "=");
    checkEOFToken(tokens[1], 1);
  }

  SECTION("Equal equal") {
    Scanner scanner{"=="};
    scanner.scanTokens();

    std::vector<Token> tokens = scanner.getTokens();
    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::EQUAL_EQUAL, "==");
    checkEOFToken(tokens[1], 1);
  }

  SECTION("Greater") {
    Scanner scanner{">"};
    scanner.scanTokens();

    std::vector<Token> tokens = scanner.getTokens();
    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::GREATER, ">");
    checkEOFToken(tokens[1], 1);
  }

  SECTION("Greater equal") {
    Scanner scanner{">="};
    scanner.scanTokens();

    std::vector<Token> tokens = scanner.getTokens();
    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::GREATER_EQUAL, ">=");
    checkEOFToken(tokens[1], 1);
  }

  SECTION("Less") {
    Scanner scanner{"<"};
    scanner.scanTokens();

    std::vector<Token> tokens = scanner.getTokens();
    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::LESS, "<");
    checkEOFToken(tokens[1], 1);
  }

  SECTION("Less equal") {
    Scanner scanner{"<="};
    scanner.scanTokens();

    std::vector<Token> tokens = scanner.getTokens();
    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::LESS_EQUAL, "<=");
    checkEOFToken(tokens[1], 1);
  }

  SECTION("Slash") {
    Scanner scanner{"/"};
    scanner.scanTokens();

    std::vector<Token> tokens = scanner.getTokens();
    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::SLASH, "/");
    checkEOFToken(tokens[1], 1);
  }
}

TEST_CASE("Scanner for identifier token", "[scanner][literals]") {
  SECTION("String without newlines") {
    Scanner scanner{"\"Hello, World!\""};
    scanner.scanTokens();
    std::vector<Token> tokens = scanner.getTokens();

    checkTokenSize(tokens, 2);
    checkStringToken(tokens[0], "Hello, World!");
    checkEOFToken(tokens[1]);
  }

  SECTION("String with newlines") {
    Scanner scanner{"\"Hello, \nWorld!\""};
    scanner.scanTokens();
    std::vector<Token> tokens = scanner.getTokens();

    checkTokenSize(tokens, 2);
    checkStringToken(tokens[0], "Hello, \nWorld!", 2);
    checkEOFToken(tokens[1], 2);
  }

  SECTION("String (unterminated string error)") {
    // Hide the error output
    std::ostringstream oss;
    auto oldCerr = std::cerr.rdbuf(oss.rdbuf());

    Scanner scanner{"\"Hello, World!"};
    scanner.scanTokens();
    std::vector<Token> tokens = scanner.getTokens();

    CHECK(hadError == true);

    checkTokenSize(tokens, 1);
    checkEOFToken(tokens[0], 1);

    // Restore the original cerr buffer
    std::cerr.rdbuf(oldCerr);
  }

  SECTION("Number") {
    Scanner scanner{"12345"};
    scanner.scanTokens();
    std::vector<Token> tokens = scanner.getTokens();

    checkTokenSize(tokens, 2);
    checkNumberToken(tokens[0], 12345);
    checkEOFToken(tokens[1]);
  }

  SECTION("Identifier") {
    Scanner scanner{"_myVar123"};
    scanner.scanTokens();
    std::vector<Token> tokens = scanner.getTokens();

    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::IDENTIFIER, "_myVar123");
    checkEOFToken(tokens[1]);
  }
}

TEST_CASE("Scacnner for keywords", "[scanner][keywords]") {
  SECTION("And") {
    Scanner scanner{"and"};
    scanner.scanTokens();
    std::vector<Token> tokens = scanner.getTokens();

    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::AND, "and");
    checkEOFToken(tokens[1]);
  }

  SECTION("Or") {
    Scanner scanner{"or"};
    scanner.scanTokens();
    std::vector<Token> tokens = scanner.getTokens();

    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::OR, "or");
    checkEOFToken(tokens[1]);
  }

  SECTION("If") {
    Scanner scanner{"if"};
    scanner.scanTokens();
    std::vector<Token> tokens = scanner.getTokens();

    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::IF, "if");
    checkEOFToken(tokens[1]);
  }

  SECTION("Else") {
    Scanner scanner{"else"};
    scanner.scanTokens();
    std::vector<Token> tokens = scanner.getTokens();

    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::ELSE, "else");
    checkEOFToken(tokens[1]);
  }

  SECTION("While") {
    Scanner scanner{"while"};
    scanner.scanTokens();
    std::vector<Token> tokens = scanner.getTokens();

    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::WHILE, "while");
    checkEOFToken(tokens[1]);
  }

  SECTION("For") {
    Scanner scanner{"for"};
    scanner.scanTokens();
    std::vector<Token> tokens = scanner.getTokens();

    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::FOR, "for");
    checkEOFToken(tokens[1]);
  }

  SECTION("True") {
    Scanner scanner{"true"};
    scanner.scanTokens();
    std::vector<Token> tokens = scanner.getTokens();

    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::TRUE, "true");
    checkEOFToken(tokens[1]);
  }

  SECTION("False") {
    Scanner scanner{"false"};
    scanner.scanTokens();
    std::vector<Token> tokens = scanner.getTokens();

    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::FALSE, "false");
    checkEOFToken(tokens[1]);
  }

  SECTION("Nil") {
    Scanner scanner{"nil"};
    scanner.scanTokens();
    std::vector<Token> tokens = scanner.getTokens();

    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::NIL, "nil");
    checkEOFToken(tokens[1]);
  }

  SECTION("Print") {
    Scanner scanner{"print"};
    scanner.scanTokens();
    std::vector<Token> tokens = scanner.getTokens();

    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::PRINT, "print");
    checkEOFToken(tokens[1]);
  }

  SECTION("Var") {
    Scanner scanner{"var"};
    scanner.scanTokens();
    std::vector<Token> tokens = scanner.getTokens();

    checkTokenSize(tokens, 2);
    checkSimpleTokens(tokens[0], TokenType::VAR, "var");
    checkEOFToken(tokens[1]);
  }
}

TEST_CASE("Scanner for multiple combinations", "[scanner][general_program]") {
  SECTION("Fibonacci") {
    std::string program = R"( var a = 0;
                              var b = 1;
                              var c = 0;

                              while (c < 100) {
                                print c;
                                c = a + b;
                                a = b;
                                b = c;
                              })";

    Scanner scanner{program};
    scanner.scanTokens();
    std::vector<Token> tokens = scanner.getTokens();

    CHECK(tokens.size() == 41);
    checkSimpleTokens(tokens[0], TokenType::VAR, "var", 1);
    checkSimpleTokens(tokens[12], TokenType::EQUAL, "=", 3);
    checkSimpleTokens(tokens[15], TokenType::WHILE, "while", 5);
    checkNumberToken(tokens[19], 100, 5);
    checkSimpleTokens(tokens[39], TokenType::RIGHT_BRACE, "}", 10);
    checkEOFToken(tokens.back(), 10);
  }
}
