#include "gsc/interpreter.hpp"
#include "catch2/catch_amalgamated.hpp"
#include "gsc/error.hpp"
#include <iostream>
#include <memory>

TEST_CASE("Interpreting Literal Expressions", "[interpreter][literal]") {
  Token EOFToken(TokenType::END_OF_FILE, "", 0, 1);

  SECTION("Integer literal") {
    Token intToken(TokenType::NUMBER, "42", 42, 1);
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(intToken.getLiteral());
    std::shared_ptr<Expr> expr = literalExpr;

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "42");
  }

  SECTION("NIL literal") {
    Token nilToken(TokenType::NIL, "nil", nullptr, 1);
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(nilToken.getLiteral());
    std::shared_ptr<Expr> expr = literalExpr;

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "nil");
  }

  SECTION("Boolean literal (true)") {
    Token trueToken(TokenType::TRUE, "true", true, 1);
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(trueToken.getLiteral());
    std::shared_ptr<Expr> expr = literalExpr;

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "true");
  }

  SECTION("Boolean literal (false)") {
    Token falseToken(TokenType::FALSE, "false", false, 1);
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(falseToken.getLiteral());
    std::shared_ptr<Expr> expr = literalExpr;

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "false");
  }

  SECTION("String literal") {
    Token stringToken(TokenType::STRING, "Hello, World!", "Hello, World!", 1);
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(stringToken.getLiteral());
    std::shared_ptr<Expr> expr = literalExpr;

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "Hello, World!");
  }
}

TEST_CASE("Interpreting Grouping Expressions", "[interpreter][grouping]") {
  SECTION("Integer grouping expressions") {
    Token intToken(TokenType::NUMBER, "42", 42, 1);
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(intToken.getLiteral());
    std::shared_ptr<Expr> expr = std::make_shared<Grouping>(literalExpr);

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "42");
  }

  SECTION("NIL grouping expressions") {
    Token nilToken(TokenType::NIL, "nil", nullptr, 1);
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(nilToken.getLiteral());
    std::shared_ptr<Expr> expr = std::make_shared<Grouping>(literalExpr);

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "nil");
  }

  SECTION("Boolean grouping expressions (true)") {
    Token trueToken(TokenType::TRUE, "true", true, 1);
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(trueToken.getLiteral());
    std::shared_ptr<Expr> expr = std::make_shared<Grouping>(literalExpr);

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "true");
  }

  SECTION("Boolean grouping expressions (false)") {
    Token falseToken(TokenType::FALSE, "false", false, 1);
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(falseToken.getLiteral());
    std::shared_ptr<Expr> expr = std::make_shared<Grouping>(literalExpr);

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "false");
  }

  SECTION("String grouping expressions") {
    Token stringToken(TokenType::STRING, "Hello, World!", "Hello, World!", 1);
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(stringToken.getLiteral());
    std::shared_ptr<Expr> expr = std::make_shared<Grouping>(literalExpr);

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "Hello, World!");
  }
}

TEST_CASE("Interpreting Unary Expressions", "[interpreter][unary]") {
  Token integerToken(TokenType::NUMBER, "42", 42, 1);
  Token booleanToken(TokenType::TRUE, "true", true, 1);
  Token minusToken(TokenType::MINUS, "-", nullptr, 1);
  Token bangToken(TokenType::BANG, "!", nullptr, 1);

  SECTION("MINUS unary expression") {
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(integerToken.getLiteral());
    std::shared_ptr<Unary> unaryExpr =
        std::make_shared<Unary>(minusToken, literalExpr);
    std::shared_ptr<Expr> expr = unaryExpr;

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "-42");
  }

  SECTION("MINUS (x2) unary expression") {
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(integerToken.getLiteral());
    std::shared_ptr<Unary> unaryExpr1 =
        std::make_shared<Unary>(minusToken, literalExpr);
    std::shared_ptr<Unary> unaryExpr2 =
        std::make_shared<Unary>(minusToken, unaryExpr1);
    std::shared_ptr<Expr> expr = unaryExpr2;

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "42");
  }

  SECTION("BANG unary expression (true)") {
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(booleanToken.getLiteral());
    std::shared_ptr<Unary> unaryExpr =
        std::make_shared<Unary>(bangToken, literalExpr);
    std::shared_ptr<Expr> expr = unaryExpr;

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "false");
  }

  SECTION("BANG (x2) unary expression (true)") {
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(booleanToken.getLiteral());
    std::shared_ptr<Unary> unaryExpr1 =
        std::make_shared<Unary>(bangToken, literalExpr);
    std::shared_ptr<Unary> unaryExpr2 =
        std::make_shared<Unary>(bangToken, unaryExpr1);
    std::shared_ptr<Expr> expr = unaryExpr2;

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "true");
  }
}

TEST_CASE("Intrpreting Binary Expressions", "[interpreter][binary]") {
  Token zeroToken(TokenType::NUMBER, "0", 0, 1);
  Token oneToken(TokenType::NUMBER, "1", 1, 1);
  Token twoToken(TokenType::NUMBER, "2", 2, 1);
  Token threeToken(TokenType::NUMBER, "3", 3, 1);
  Token plusToken(TokenType::PLUS, "+", nullptr, 1);
  Token minusToken(TokenType::MINUS, "-", nullptr, 1);
  Token starToken(TokenType::STAR, "*", nullptr, 1);
  Token slashToken(TokenType::SLASH, "/", nullptr, 1);
  Token bangEqualToken(TokenType::BANG_EQUAL, "!=", nullptr, 1);
  Token equalEqualToken(TokenType::EQUAL_EQUAL, "==", nullptr, 1);
  Token greaterToken(TokenType::GREATER, ">", nullptr, 1);
  Token greaterEqualToken(TokenType::GREATER_EQUAL, ">=", nullptr, 1);
  Token lessToken(TokenType::LESS, "<", nullptr, 1);
  Token lessEqualToken(TokenType::LESS_EQUAL, "<=", nullptr, 1);
  Token stringToken(TokenType::STRING, "Hello, World!",
                    std::string("Hello, World!"), 1);

  // Hide the error output
  std::ostringstream oss;
  auto oldCerr = std::cerr.rdbuf(oss.rdbuf());

  SECTION("Sumation (1+2)") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(oneToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(twoToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, plusToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "3");
  }

  SECTION("Concatenation of strings") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(stringToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(stringToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, plusToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "Hello, World!Hello, World!");
  }

  SECTION("Subtraction (3-2)") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(threeToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(twoToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, minusToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "1");
  }

  SECTION("Multiplication (2*3)") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(twoToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(threeToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, starToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "6");
  }

  SECTION("Division (3/2)") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(threeToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(twoToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, slashToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "1");
  }

  SECTION("Division by Zero") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(threeToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(zeroToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, slashToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "");
    CHECK(hadRuntimeError == true);
  }

  SECTION("Equality (1==1)") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(oneToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(oneToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, equalEqualToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "true");
  }

  SECTION("Inequality (1!=2)") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(oneToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(twoToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, bangEqualToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "true");
  }

  SECTION("Greater than (2>1)") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(twoToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(oneToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, greaterToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "true");
  }

  SECTION("Greater or equal than (1>=1)") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(oneToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(oneToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, greaterEqualToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "true");
  }

  SECTION("Less than (1<2)") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(oneToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(twoToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, lessToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "true");
  }

  SECTION("Less or equal than (1<=1)") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(oneToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(oneToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, lessEqualToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;

    std::string result = Interpreter().interpret(expr);
    CHECK(result == "true");
  }

  // Restore the original cerr buffer
  std::cerr.rdbuf(oldCerr);
}
