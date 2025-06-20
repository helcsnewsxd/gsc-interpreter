#include "gsc/parser.hpp"
#include "catch2/catch_amalgamated.hpp"
#include "gsc/expr.hpp"
#include "gsc/stmt.hpp"
#include <iostream>
#include <memory>

TEST_CASE("Parse an empty program", "[parser][program][empty]") {
  // Hide the error output
  std::ostringstream oss;
  auto oldCerr = std::cerr.rdbuf(oss.rdbuf());

  SECTION("EOF token") {
    Token EOFToken = {TokenType::END_OF_FILE, "", nullptr, 1};
    std::vector<Token> tokens = {EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    CHECK(statements.empty());
  }

  // Restore the original cerr buffer
  std::cerr.rdbuf(oldCerr);
}

TEST_CASE("Parse a primary expression", "[parser][expression][primary]") {
  Token EOFToken = {TokenType::END_OF_FILE, "", nullptr, 1};
  Token semicolonToken = {TokenType::SEMICOLON, ";", nullptr, 1};

  // Hide the error output
  std::ostringstream oss;
  auto oldCerr = std::cerr.rdbuf(oss.rdbuf());

  SECTION("NIL token") {
    Token nilToken = {TokenType::NIL, "nil", nullptr, 1};
    std::vector<Token> tokens = {nilToken, semicolonToken, EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(statements[0]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    std::shared_ptr<Literal> literalExpr =
        std::dynamic_pointer_cast<Literal>(expr);
    REQUIRE(literalExpr != nullptr);
    CHECK(literalExpr->getValue().type() == typeid(std::nullptr_t));
    CHECK(std::any_cast<std::nullptr_t>(literalExpr->getValue()) == nullptr);
  }

  SECTION("True token") {
    Token trueToken = {TokenType::TRUE, "true", nullptr, 1};
    std::vector<Token> tokens = {trueToken, semicolonToken, EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    REQUIRE(expr != nullptr);
    std::shared_ptr<Literal> literalExpr =
        std::dynamic_pointer_cast<Literal>(expr);
    REQUIRE(literalExpr != nullptr);
    REQUIRE(literalExpr->getValue().type() == typeid(bool));
    CHECK(std::any_cast<bool>(literalExpr->getValue()) == true);
  }

  SECTION("False token") {
    Token falseToken = {TokenType::FALSE, "false", nullptr, 1};
    std::vector<Token> tokens = {falseToken, semicolonToken, EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt != nullptr);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    std::shared_ptr<Literal> literalExpr =
        std::dynamic_pointer_cast<Literal>(expr);
    REQUIRE(literalExpr != nullptr);
    REQUIRE(literalExpr->getValue().type() == typeid(bool));
    CHECK(std::any_cast<bool>(literalExpr->getValue()) == false);
  }

  SECTION("Number token") {
    Token numberToken = {TokenType::NUMBER, "42", 42, 1};
    std::vector<Token> tokens = {numberToken, semicolonToken, EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt != nullptr);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    std::shared_ptr<Literal> literalExpr =
        std::dynamic_pointer_cast<Literal>(expr);
    REQUIRE(literalExpr != nullptr);
    REQUIRE(literalExpr->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(literalExpr->getValue()) == 42);
  }

  SECTION("String token") {
    Token stringToken = {TokenType::STRING, "hello", std::string("hello"), 1};
    std::vector<Token> tokens = {stringToken, semicolonToken, EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(statements[0]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    std::shared_ptr<Literal> literalExpr =
        std::dynamic_pointer_cast<Literal>(expr);
    REQUIRE(literalExpr != nullptr);
    REQUIRE(literalExpr->getValue().type() == typeid(std::string));
    CHECK(std::any_cast<std::string>(literalExpr->getValue()) == "hello");
  }

  SECTION("Grouping expression") {
    Token leftParenToken = {TokenType::LEFT_PAREN, "(", nullptr, 1};
    Token rightParenToken = {TokenType::RIGHT_PAREN, ")", nullptr, 1};
    Token numberToken = {TokenType::NUMBER, "42", 42, 1};
    std::vector<Token> tokens = {leftParenToken, numberToken, rightParenToken,
                                 semicolonToken, EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(statements[0]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    std::shared_ptr<Grouping> groupingExpr =
        std::dynamic_pointer_cast<Grouping>(expr);
    REQUIRE(groupingExpr != nullptr);
    REQUIRE(groupingExpr->getExpression() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(groupingExpr->getExpression()) !=
            nullptr);
    std::shared_ptr<Literal> literalExpr =
        std::dynamic_pointer_cast<Literal>(groupingExpr->getExpression());
    REQUIRE(literalExpr->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(literalExpr->getValue()) == 42);
  }

  SECTION("Grouping expression without right parenthesis") {
    Token leftParenToken = {TokenType::LEFT_PAREN, "(", nullptr, 1};
    Token numberToken = {TokenType::NUMBER, "42", 42, 1};
    std::vector<Token> tokens = {leftParenToken, numberToken, semicolonToken,
                                 EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements;
    REQUIRE(statements.empty());
  }

  // Restore the original cerr buffer
  std::cerr.rdbuf(oldCerr);
}

TEST_CASE("Parse an unary expression", "[parser][expression][unary]") {
  // Hide the error output
  std::ostringstream oss;
  auto oldCerr = std::cerr.rdbuf(oss.rdbuf());

  Token minusToken = {TokenType::MINUS, "-", nullptr, 1};
  Token bangToken = {TokenType::BANG, "!", nullptr, 1};
  Token numberToken = {TokenType::NUMBER, "42", 42, 1};
  Token boolToken = {TokenType::TRUE, "true", nullptr, 1};
  Token EOFToken = {TokenType::END_OF_FILE, "", nullptr, 1};
  Token semicolonToken = {TokenType::SEMICOLON, ";", nullptr, 1};

  SECTION("Minus token with a number") {
    std::vector<Token> tokens = {minusToken, numberToken, semicolonToken,
                                 EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(statements[0]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    REQUIRE(std::dynamic_pointer_cast<Unary>(expr) != nullptr);
    std::shared_ptr<Unary> unaryExpr = std::dynamic_pointer_cast<Unary>(expr);
    REQUIRE(unaryExpr->getRight() != nullptr);
    CHECK(unaryExpr->getOp().getType() == TokenType::MINUS);
    REQUIRE(std::dynamic_pointer_cast<Literal>(unaryExpr->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> literalExpr =
        std::dynamic_pointer_cast<Literal>(unaryExpr->getRight());
    REQUIRE(literalExpr->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(literalExpr->getValue()) == 42);
  }

  SECTION("Minus (x2 times) with a number") {
    std::vector<Token> tokens = {minusToken, minusToken, numberToken,
                                 semicolonToken, EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(statements[0]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    REQUIRE(std::dynamic_pointer_cast<Unary>(expr) != nullptr);
    std::shared_ptr<Unary> unaryExpr = std::dynamic_pointer_cast<Unary>(expr);
    REQUIRE(unaryExpr->getRight() != nullptr);
    CHECK(unaryExpr->getOp().getType() == TokenType::MINUS);
    REQUIRE(std::dynamic_pointer_cast<Unary>(unaryExpr->getRight()) != nullptr);
    std::shared_ptr<Unary> innerUnaryExpr =
        std::dynamic_pointer_cast<Unary>(unaryExpr->getRight());
    REQUIRE(innerUnaryExpr->getRight() != nullptr);
    CHECK(innerUnaryExpr->getOp().getType() == TokenType::MINUS);
    REQUIRE(std::dynamic_pointer_cast<Literal>(innerUnaryExpr->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> literalExpr =
        std::dynamic_pointer_cast<Literal>(innerUnaryExpr->getRight());
    REQUIRE(literalExpr->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(literalExpr->getValue()) == 42);
  }

  SECTION("Neg/Bang with a boolean") {
    std::vector<Token> tokens = {bangToken, boolToken, semicolonToken,
                                 EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(statements[0]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    REQUIRE(std::dynamic_pointer_cast<Unary>(expr) != nullptr);
    std::shared_ptr<Unary> unaryExpr = std::dynamic_pointer_cast<Unary>(expr);
    REQUIRE(unaryExpr->getRight() != nullptr);
    CHECK(unaryExpr->getOp().getType() == TokenType::BANG);
    REQUIRE(std::dynamic_pointer_cast<Literal>(unaryExpr->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> literalExpr =
        std::dynamic_pointer_cast<Literal>(unaryExpr->getRight());
    REQUIRE(literalExpr->getValue().type() == typeid(bool));
    CHECK(std::any_cast<bool>(literalExpr->getValue()) == true);
  }

  SECTION("Neg/Bang (x2 times) with a boolean") {
    std::vector<Token> tokens = {bangToken, bangToken, boolToken,
                                 semicolonToken, EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(statements[0]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    REQUIRE(std::dynamic_pointer_cast<Unary>(expr) != nullptr);
    std::shared_ptr<Unary> unaryExpr = std::dynamic_pointer_cast<Unary>(expr);
    CHECK(unaryExpr->getOp().getType() == TokenType::BANG);
    REQUIRE(unaryExpr->getRight() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Unary>(unaryExpr->getRight()) != nullptr);
    std::shared_ptr<Unary> innerUnaryExpr =
        std::dynamic_pointer_cast<Unary>(unaryExpr->getRight());
    CHECK(innerUnaryExpr->getOp().getType() == TokenType::BANG);
    REQUIRE(innerUnaryExpr->getRight() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(innerUnaryExpr->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> literalExpr =
        std::dynamic_pointer_cast<Literal>(innerUnaryExpr->getRight());
    REQUIRE(literalExpr->getValue().type() == typeid(bool));
    CHECK(std::any_cast<bool>(literalExpr->getValue()) == true);
  }

  // Restore the original cerr buffer
  std::cerr.rdbuf(oldCerr);
}

TEST_CASE("Parse a factor expression", "[parser][expression][factor]") {
  Token oneNumToken = {TokenType::NUMBER, "1", 1, 1};
  Token twoNumToken = {TokenType::NUMBER, "2", 2, 1};
  Token threeNumToken = {TokenType::NUMBER, "3", 3, 1};
  Token starToken = {TokenType::STAR, "*", nullptr, 1};
  Token slashToken = {TokenType::SLASH, "/", nullptr, 1};
  Token EOFToken = {TokenType::END_OF_FILE, "", nullptr, 1};
  Token semicolonToken = {TokenType::SEMICOLON, ";", nullptr, 1};

  SECTION("Multiplication of two numbers") {
    std::vector<Token> tokens = {oneNumToken, starToken, twoNumToken,
                                 semicolonToken, EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(statements[0]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    REQUIRE(std::dynamic_pointer_cast<Binary>(expr) != nullptr);
    std::shared_ptr<Binary> binaryExpr =
        std::dynamic_pointer_cast<Binary>(expr);
    CHECK(binaryExpr->getOp().getType() == TokenType::STAR);

    REQUIRE(binaryExpr->getLeft() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getLeft()) !=
            nullptr);
    std::shared_ptr<Literal> leftLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getLeft());
    REQUIRE(leftLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(leftLiteral->getValue()) == 1);

    REQUIRE(binaryExpr->getRight() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> rightLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getRight());
    REQUIRE(rightLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(rightLiteral->getValue()) == 2);
  }

  SECTION("Division of two numbers") {
    std::vector<Token> tokens = {oneNumToken, slashToken, twoNumToken,
                                 semicolonToken, EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(statements[0]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    REQUIRE(std::dynamic_pointer_cast<Binary>(expr) != nullptr);
    std::shared_ptr<Binary> binaryExpr =
        std::dynamic_pointer_cast<Binary>(expr);
    CHECK(binaryExpr->getOp().getType() == TokenType::SLASH);

    REQUIRE(binaryExpr->getLeft() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getLeft()) !=
            nullptr);
    std::shared_ptr<Literal> leftLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getLeft());
    REQUIRE(leftLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(leftLiteral->getValue()) == 1);

    REQUIRE(binaryExpr->getRight() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> rightLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getRight());
    REQUIRE(rightLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(rightLiteral->getValue()) == 2);
  }

  SECTION(
      "Factor expression with three numbers (checking left associativity)") {
    std::vector<Token> tokens = {oneNumToken, starToken,     twoNumToken,
                                 slashToken,  threeNumToken, semicolonToken,
                                 EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(statements[0]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    REQUIRE(std::dynamic_pointer_cast<Binary>(expr) != nullptr);
    std::shared_ptr<Binary> binaryExpr =
        std::dynamic_pointer_cast<Binary>(expr);
    CHECK(binaryExpr->getOp().getType() == TokenType::SLASH);

    REQUIRE(binaryExpr->getRight() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> rightLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getRight());
    REQUIRE(rightLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(rightLiteral->getValue()) == 3);

    REQUIRE(binaryExpr->getLeft() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Binary>(binaryExpr->getLeft()) !=
            nullptr);
    std::shared_ptr<Binary> leftBinaryExpr =
        std::dynamic_pointer_cast<Binary>(binaryExpr->getLeft());
    CHECK(leftBinaryExpr->getOp().getType() == TokenType::STAR);

    REQUIRE(leftBinaryExpr->getLeft() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(leftBinaryExpr->getLeft()) !=
            nullptr);
    std::shared_ptr<Literal> leftLeftLiteral =
        std::dynamic_pointer_cast<Literal>(leftBinaryExpr->getLeft());
    REQUIRE(leftLeftLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(leftLeftLiteral->getValue()) == 1);

    REQUIRE(leftBinaryExpr->getRight() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(leftBinaryExpr->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> leftRightLiteral =
        std::dynamic_pointer_cast<Literal>(leftBinaryExpr->getRight());
    REQUIRE(leftRightLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(leftRightLiteral->getValue()) == 2);
  }
}

TEST_CASE("Parse a term expression", "[parser][expression][term]") {
  Token oneNumToken = {TokenType::NUMBER, "1", 1, 1};
  Token twoNumToken = {TokenType::NUMBER, "2", 2, 1};
  Token minusToken = {TokenType::MINUS, "-", nullptr, 1};
  Token plusToken = {TokenType::PLUS, "+", nullptr, 1};
  Token EOFToken = {TokenType::END_OF_FILE, "", nullptr, 1};
  Token semicolonToken = {TokenType::SEMICOLON, ";", nullptr, 1};

  SECTION("Sum Expression") {
    std::vector<Token> tokens = {oneNumToken, plusToken, twoNumToken,
                                 semicolonToken, EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(statements[0]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    REQUIRE(std::dynamic_pointer_cast<Binary>(expr) != nullptr);
    std::shared_ptr<Binary> binaryExpr =
        std::dynamic_pointer_cast<Binary>(expr);
    CHECK(binaryExpr->getOp().getType() == TokenType::PLUS);

    REQUIRE(binaryExpr->getLeft() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getLeft()) !=
            nullptr);
    std::shared_ptr<Literal> leftLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getLeft());
    REQUIRE(leftLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(leftLiteral->getValue()) == 1);

    REQUIRE(binaryExpr->getRight() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> rightLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getRight());
    REQUIRE(rightLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(rightLiteral->getValue()) == 2);
  }

  SECTION("Minux Expression") {
    std::vector<Token> tokens = {oneNumToken, minusToken, twoNumToken,
                                 semicolonToken, EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(statements[0]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    REQUIRE(std::dynamic_pointer_cast<Binary>(expr) != nullptr);
    std::shared_ptr<Binary> binaryExpr =
        std::dynamic_pointer_cast<Binary>(expr);
    CHECK(binaryExpr->getOp().getType() == TokenType::MINUS);

    REQUIRE(binaryExpr->getLeft() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getLeft()) !=
            nullptr);
    std::shared_ptr<Literal> leftLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getLeft());
    REQUIRE(leftLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(leftLiteral->getValue()) == 1);

    REQUIRE(binaryExpr->getRight() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> rightLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getRight());
    REQUIRE(rightLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(rightLiteral->getValue()) == 2);
  }
}

TEST_CASE("Parse a comparison expression", "[parser][expression][comparison]") {
  Token oneNumToken = {TokenType::NUMBER, "1", 1, 1};
  Token twoNumToken = {TokenType::NUMBER, "2", 2, 1};
  Token greaterToken = {TokenType::GREATER, ">", nullptr, 1};
  Token greaterEqualToken = {TokenType::GREATER_EQUAL, ">=", nullptr, 1};
  Token lessToken = {TokenType::LESS, "<", nullptr, 1};
  Token lessEqualToken = {TokenType::LESS_EQUAL, "<=", nullptr, 1};
  Token EOFToken = {TokenType::END_OF_FILE, "", nullptr, 1};
  Token semicolonToken = {TokenType::SEMICOLON, ";", nullptr, 1};

  SECTION("Greater than comparison") {
    std::vector<Token> tokens = {oneNumToken, greaterToken, twoNumToken,
                                 semicolonToken, EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(statements[0]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    REQUIRE(std::dynamic_pointer_cast<Binary>(expr) != nullptr);
    std::shared_ptr<Binary> binaryExpr =
        std::dynamic_pointer_cast<Binary>(expr);
    CHECK(binaryExpr->getOp().getType() == TokenType::GREATER);

    REQUIRE(binaryExpr->getLeft() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getLeft()) !=
            nullptr);
    std::shared_ptr<Literal> leftLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getLeft());
    REQUIRE(leftLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(leftLiteral->getValue()) == 1);

    REQUIRE(binaryExpr->getRight() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> rightLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getRight());
    REQUIRE(rightLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(rightLiteral->getValue()) == 2);
  }

  SECTION("Greater or equal than comparison") {
    std::vector<Token> tokens = {oneNumToken, greaterEqualToken, twoNumToken,
                                 semicolonToken, EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(statements[0]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    REQUIRE(std::dynamic_pointer_cast<Binary>(expr) != nullptr);
    std::shared_ptr<Binary> binaryExpr =
        std::dynamic_pointer_cast<Binary>(expr);
    CHECK(binaryExpr->getOp().getType() == TokenType::GREATER_EQUAL);

    REQUIRE(binaryExpr->getLeft() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getLeft()) !=
            nullptr);
    std::shared_ptr<Literal> leftLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getLeft());
    REQUIRE(leftLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(leftLiteral->getValue()) == 1);

    REQUIRE(binaryExpr->getRight() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> rightLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getRight());
    REQUIRE(rightLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(rightLiteral->getValue()) == 2);
  }

  SECTION("Less than comparison") {
    std::vector<Token> tokens = {oneNumToken, lessToken, twoNumToken,
                                 semicolonToken, EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(statements[0]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    REQUIRE(std::dynamic_pointer_cast<Binary>(expr) != nullptr);
    std::shared_ptr<Binary> binaryExpr =
        std::dynamic_pointer_cast<Binary>(expr);
    CHECK(binaryExpr->getOp().getType() == TokenType::LESS);

    REQUIRE(binaryExpr->getLeft() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getLeft()) !=
            nullptr);
    std::shared_ptr<Literal> leftLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getLeft());
    REQUIRE(leftLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(leftLiteral->getValue()) == 1);

    REQUIRE(binaryExpr->getRight() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> rightLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getRight());
    REQUIRE(rightLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(rightLiteral->getValue()) == 2);
  }

  SECTION("Less or equal than comparison") {
    std::vector<Token> tokens = {oneNumToken, lessEqualToken, twoNumToken,
                                 semicolonToken, EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(statements[0]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    REQUIRE(std::dynamic_pointer_cast<Binary>(expr) != nullptr);
    std::shared_ptr<Binary> binaryExpr =
        std::dynamic_pointer_cast<Binary>(expr);
    CHECK(binaryExpr->getOp().getType() == TokenType::LESS_EQUAL);

    REQUIRE(binaryExpr->getLeft() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getLeft()) !=
            nullptr);
    std::shared_ptr<Literal> leftLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getLeft());
    REQUIRE(leftLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(leftLiteral->getValue()) == 1);

    REQUIRE(binaryExpr->getRight() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> rightLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getRight());
    REQUIRE(rightLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(rightLiteral->getValue()) == 2);
  }
}

TEST_CASE("Parse equality expression", "[parser][expression][equality]") {
  Token oneNumToken = {TokenType::NUMBER, "1", 1, 1};
  Token twoNumToken = {TokenType::NUMBER, "2", 2, 1};
  Token bangEqualToken = {TokenType::BANG_EQUAL, "!=", nullptr, 1};
  Token equalEqualToken = {TokenType::EQUAL_EQUAL, "==", nullptr, 1};
  Token EOFToken = {TokenType::END_OF_FILE, "", nullptr, 1};
  Token semicolonToken = {TokenType::SEMICOLON, ";", nullptr, 1};

  SECTION("Equal comparison") {
    std::vector<Token> tokens = {oneNumToken, equalEqualToken, twoNumToken,
                                 semicolonToken, EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(statements[0]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    REQUIRE(std::dynamic_pointer_cast<Binary>(expr) != nullptr);
    std::shared_ptr<Binary> binaryExpr =
        std::dynamic_pointer_cast<Binary>(expr);
    CHECK(binaryExpr->getOp().getType() == TokenType::EQUAL_EQUAL);

    REQUIRE(binaryExpr->getLeft() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getLeft()) !=
            nullptr);
    std::shared_ptr<Literal> leftLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getLeft());
    REQUIRE(leftLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(leftLiteral->getValue()) == 1);

    REQUIRE(binaryExpr->getRight() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> rightLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getRight());
    REQUIRE(rightLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(rightLiteral->getValue()) == 2);
  }

  SECTION("Different comparison") {
    std::vector<Token> tokens = {oneNumToken, bangEqualToken, twoNumToken,
                                 semicolonToken, EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(statements[0]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    REQUIRE(std::dynamic_pointer_cast<Binary>(expr) != nullptr);
    std::shared_ptr<Binary> binaryExpr =
        std::dynamic_pointer_cast<Binary>(expr);
    CHECK(binaryExpr->getOp().getType() == TokenType::BANG_EQUAL);

    REQUIRE(binaryExpr->getLeft() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getLeft()) !=
            nullptr);
    std::shared_ptr<Literal> leftLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getLeft());
    REQUIRE(leftLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(leftLiteral->getValue()) == 1);

    REQUIRE(binaryExpr->getRight() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> rightLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getRight());
    REQUIRE(rightLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(rightLiteral->getValue()) == 2);
  }
}

TEST_CASE("Parsing statements (2 or more)", "[parser][statement][multiple]") {
  Token oneNumToken = {TokenType::NUMBER, "1", 1, 1};
  Token twoNumToken = {TokenType::NUMBER, "2", 2, 1};
  Token threeNumToken = {TokenType::NUMBER, "3", 3, 1};
  Token plusToken = {TokenType::PLUS, "+", nullptr, 1};
  Token semicolonToken = {TokenType::SEMICOLON, ";", nullptr, 1};
  Token EOFToken = {TokenType::END_OF_FILE, "", nullptr, 1};

  std::vector<Token> tokens = {oneNumToken,    plusToken,      twoNumToken,
                               semicolonToken, threeNumToken,  plusToken,
                               oneNumToken,    semicolonToken, EOFToken};
  Parser parser(tokens);

  std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
  REQUIRE(statements.size() == 2);
  for (int i = 0; i < 2; i++) {
    REQUIRE(statements[i] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(statements[i]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[i]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    REQUIRE(std::dynamic_pointer_cast<Binary>(expr) != nullptr);
    std::shared_ptr<Binary> binaryExpr =
        std::dynamic_pointer_cast<Binary>(expr);
    CHECK(binaryExpr->getOp().getType() == TokenType::PLUS);

    REQUIRE(binaryExpr->getLeft() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getLeft()) !=
            nullptr);
    std::shared_ptr<Literal> leftLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getLeft());
    REQUIRE(leftLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(leftLiteral->getValue()) == (i == 0 ? 1 : 3));

    REQUIRE(binaryExpr->getRight() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(binaryExpr->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> rightLiteral =
        std::dynamic_pointer_cast<Literal>(binaryExpr->getRight());
    REQUIRE(rightLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(rightLiteral->getValue()) == (i == 0 ? 2 : 1));
  }
}

TEST_CASE("Parsing print statement", "[parser][statement][print]") {
  Token oneNumToken = {TokenType::NUMBER, "1", 1, 1};
  Token printToken = {TokenType::PRINT, "print", nullptr, 1};
  Token semicolonToken = {TokenType::SEMICOLON, ";", nullptr, 1};
  Token EOFToken = {TokenType::END_OF_FILE, "", nullptr, 1};

  std::vector<Token> tokens = {printToken, oneNumToken, semicolonToken,
                               EOFToken};
  Parser parser(tokens);

  std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
  REQUIRE(statements.size() == 1);
  REQUIRE(statements[0] != nullptr);
  REQUIRE(std::dynamic_pointer_cast<Print>(statements[0]) != nullptr);
  std::shared_ptr<Print> printStmt =
      std::dynamic_pointer_cast<Print>(statements[0]);
  REQUIRE(printStmt->getExpression() != nullptr);
  std::shared_ptr<Expr> expr =
      std::dynamic_pointer_cast<Expr>(printStmt->getExpression());
  REQUIRE(std::dynamic_pointer_cast<Literal>(expr) != nullptr);
  std::shared_ptr<Literal> literalExpr =
      std::dynamic_pointer_cast<Literal>(expr);
  REQUIRE(literalExpr->getValue().type() == typeid(int));
  CHECK(std::any_cast<int>(literalExpr->getValue()) == 1);
}

TEST_CASE("Parsing block statements", "[oaser][statement][block]") {
  Token oneNumToken = {TokenType::NUMBER, "1", 1, 1};
  Token twoNumToken = {TokenType::NUMBER, "2", 2, 1};
  Token lbraceToken = {TokenType::LEFT_BRACE, "{", nullptr, 1};
  Token rbraceToken = {TokenType::RIGHT_BRACE, "}", nullptr, 1};
  Token semicolonToken = {TokenType::SEMICOLON, ";", nullptr, 1};
  Token EOFToken = {TokenType::END_OF_FILE, "", nullptr, 1};

  std::vector<Token> tokens = {oneNumToken, semicolonToken, lbraceToken,
                               twoNumToken, semicolonToken, rbraceToken,
                               EOFToken};
  Parser parser(tokens);

  std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
  REQUIRE(statements.size() == 2);

  REQUIRE(statements[0] != nullptr);
  REQUIRE(std::dynamic_pointer_cast<Expression>(statements[0]) != nullptr);
  std::shared_ptr<Expression> exprStmt =
      std::dynamic_pointer_cast<Expression>(statements[0]);
  REQUIRE(exprStmt->getExpression() != nullptr);
  std::shared_ptr<Expr> expr =
      std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
  REQUIRE(std::dynamic_pointer_cast<Literal>(expr) != nullptr);
  std::shared_ptr<Literal> literalExpr =
      std::dynamic_pointer_cast<Literal>(expr);
  REQUIRE(literalExpr->getValue().type() == typeid(int));
  CHECK(std::any_cast<int>(literalExpr->getValue()) == 1);

  REQUIRE(statements[1] != nullptr);
  REQUIRE(std::dynamic_pointer_cast<Block>(statements[1]) != nullptr);
  std::shared_ptr<Block> blockStmt =
      std::dynamic_pointer_cast<Block>(statements[1]);
  REQUIRE(blockStmt->getStatements().size() == 1);
  std::shared_ptr<Stmt> blockInnerStmt = blockStmt->getStatements()[0];
  REQUIRE(blockInnerStmt != nullptr);
  REQUIRE(std::dynamic_pointer_cast<Expression>(blockInnerStmt) != nullptr);
  std::shared_ptr<Expression> blockExprStmt =
      std::dynamic_pointer_cast<Expression>(blockInnerStmt);
  REQUIRE(blockExprStmt->getExpression() != nullptr);
  std::shared_ptr<Expr> blockExpr =
      std::dynamic_pointer_cast<Expr>(blockExprStmt->getExpression());
  REQUIRE(std::dynamic_pointer_cast<Literal>(blockExpr) != nullptr);
  std::shared_ptr<Literal> blockLiteralExpr =
      std::dynamic_pointer_cast<Literal>(blockExpr);
  REQUIRE(blockLiteralExpr->getValue().type() == typeid(int));
  CHECK(std::any_cast<int>(blockLiteralExpr->getValue()) == 2);
}

TEST_CASE("Parsing declaring statements", "[parser][statement][declare]") {
  Token varToken = {TokenType::VAR, "var", nullptr, 1};
  Token identifierToken = {TokenType::IDENTIFIER, "x", nullptr, 1};
  Token equalToken = {TokenType::EQUAL, "=", nullptr, 1};
  Token oneNumToken = {TokenType::NUMBER, "1", 1, 1};
  Token semicolonToken = {TokenType::SEMICOLON, ";", nullptr, 1};
  Token EOFToken = {TokenType::END_OF_FILE, "", nullptr, 1};

  SECTION("Declare a variable without value") {
    std::vector<Token> tokens = {varToken, identifierToken, semicolonToken,
                                 EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Var>(statements[0]) != nullptr);
    std::shared_ptr<Var> varStmt =
        std::dynamic_pointer_cast<Var>(statements[0]);

    REQUIRE(varStmt->getName().getType() == TokenType::IDENTIFIER);
    std::string varName = varStmt->getName().getLexeme();
    CHECK(varName == "x");

    REQUIRE(varStmt->getInitializer() == nullptr);
  }

  SECTION("Declare a variable with value") {
    std::vector<Token> tokens = {varToken,    identifierToken, equalToken,
                                 oneNumToken, semicolonToken,  EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Var>(statements[0]) != nullptr);
    std::shared_ptr<Var> varStmt =
        std::dynamic_pointer_cast<Var>(statements[0]);

    REQUIRE(varStmt->getName().getType() == TokenType::IDENTIFIER);
    std::string varName = varStmt->getName().getLexeme();
    CHECK(varName == "x");

    REQUIRE(varStmt->getInitializer() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expr>(varStmt->getInitializer()) !=
            nullptr);
    std::shared_ptr<Expr> initializerExpr =
        std::dynamic_pointer_cast<Expr>(varStmt->getInitializer());
    REQUIRE(std::dynamic_pointer_cast<Literal>(initializerExpr) != nullptr);
    std::shared_ptr<Literal> literalExpr =
        std::dynamic_pointer_cast<Literal>(initializerExpr);
    REQUIRE(literalExpr->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(literalExpr->getValue()) == 1);
  }
}

TEST_CASE("Parsing logical operators", "[parser][statement][logical]") {
  Token trueToken = {TokenType::TRUE, "true", nullptr, 1};
  Token falseToken = {TokenType::FALSE, "false", nullptr, 1};
  Token andToken = {TokenType::AND, "and", nullptr, 1};
  Token orToken = {TokenType::OR, "or", nullptr, 1};
  Token semicolonToken = {TokenType::SEMICOLON, ";", nullptr, 1};
  Token EOFToken = {TokenType::END_OF_FILE, "", nullptr, 1};

  SECTION("And statement") {
    std::vector<Token> tokens = {trueToken, andToken, falseToken,
                                 semicolonToken, EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(statements[0]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    REQUIRE(std::dynamic_pointer_cast<Logical>(expr) != nullptr);
    std::shared_ptr<Logical> logicalExpr =
        std::dynamic_pointer_cast<Logical>(expr);

    REQUIRE(logicalExpr->getLeft() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(logicalExpr->getLeft()) !=
            nullptr);
    std::shared_ptr<Literal> leftLiteral =
        std::dynamic_pointer_cast<Literal>(logicalExpr->getLeft());
    REQUIRE(leftLiteral->getValue().type() == typeid(bool));
    CHECK(std::any_cast<bool>(leftLiteral->getValue()) == true);

    REQUIRE(logicalExpr->getRight() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(logicalExpr->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> rightLiteral =
        std::dynamic_pointer_cast<Literal>(logicalExpr->getRight());
    REQUIRE(rightLiteral->getValue().type() == typeid(bool));
    CHECK(std::any_cast<bool>(rightLiteral->getValue()) == false);

    REQUIRE(logicalExpr->getOp().getType() == TokenType::AND);
  }

  SECTION("Or statement") {
    std::vector<Token> tokens = {trueToken, orToken, falseToken, semicolonToken,
                                 EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(statements[0]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(statements[0]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    std::shared_ptr<Expr> expr =
        std::dynamic_pointer_cast<Expr>(exprStmt->getExpression());
    REQUIRE(std::dynamic_pointer_cast<Logical>(expr) != nullptr);
    std::shared_ptr<Logical> logicalExpr =
        std::dynamic_pointer_cast<Logical>(expr);

    REQUIRE(logicalExpr->getLeft() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(logicalExpr->getLeft()) !=
            nullptr);
    std::shared_ptr<Literal> leftLiteral =
        std::dynamic_pointer_cast<Literal>(logicalExpr->getLeft());
    REQUIRE(leftLiteral->getValue().type() == typeid(bool));
    CHECK(std::any_cast<bool>(leftLiteral->getValue()) == true);

    REQUIRE(logicalExpr->getRight() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(logicalExpr->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> rightLiteral =
        std::dynamic_pointer_cast<Literal>(logicalExpr->getRight());
    REQUIRE(rightLiteral->getValue().type() == typeid(bool));
    CHECK(std::any_cast<bool>(rightLiteral->getValue()) == false);

    REQUIRE(logicalExpr->getOp().getType() == TokenType::OR);
  }
}

TEST_CASE("Parsing an if statement", "[parser][statement][if]") {
  Token ifToken = {TokenType::IF, "if", nullptr, 1};
  Token elseToken = {TokenType::ELSE, "else", nullptr, 1};
  Token leftParenToken = {TokenType::LEFT_PAREN, "(", nullptr, 1};
  Token rightParenToken = {TokenType::RIGHT_PAREN, ")", nullptr, 1};
  Token trueToken = {TokenType::TRUE, "true", nullptr, 1};
  Token oneNumToken = {TokenType::NUMBER, "1", 1, 1};
  Token twoNumToken = {TokenType::NUMBER, "2", 2, 1};
  Token semicolonToken = {TokenType::SEMICOLON, ";", nullptr, 1};
  Token EOFToken = {TokenType::END_OF_FILE, "", nullptr, 1};

  SECTION("If alone case") {
    std::vector<Token> tokens = {
        ifToken,     leftParenToken, trueToken, rightParenToken,
        oneNumToken, semicolonToken, EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<If>(statements[0]) != nullptr);
    std::shared_ptr<If> ifStmt = std::dynamic_pointer_cast<If>(statements[0]);

    REQUIRE(ifStmt->getCondition() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(ifStmt->getCondition()) !=
            nullptr);
    std::shared_ptr<Literal> conditionLiteral =
        std::dynamic_pointer_cast<Literal>(ifStmt->getCondition());
    REQUIRE(conditionLiteral->getValue().type() == typeid(bool));
    CHECK(std::any_cast<bool>(conditionLiteral->getValue()) == true);

    REQUIRE(ifStmt->getThenBranch() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(ifStmt->getThenBranch()) !=
            nullptr);
    std::shared_ptr<Expression> thenExprStmt =
        std::dynamic_pointer_cast<Expression>(ifStmt->getThenBranch());
    REQUIRE(thenExprStmt->getExpression() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(thenExprStmt->getExpression()) !=
            nullptr);
    std::shared_ptr<Literal> thenLiteralExpr =
        std::dynamic_pointer_cast<Literal>(thenExprStmt->getExpression());
    REQUIRE(thenLiteralExpr->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(thenLiteralExpr->getValue()) == 1);

    REQUIRE(ifStmt->getElseBranch() == nullptr);
  }

  SECTION("If-else case") {
    std::vector<Token> tokens = {
        ifToken,        leftParenToken, trueToken, rightParenToken,
        oneNumToken,    semicolonToken, elseToken, twoNumToken,
        semicolonToken, EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<If>(statements[0]) != nullptr);
    std::shared_ptr<If> ifStmt = std::dynamic_pointer_cast<If>(statements[0]);

    REQUIRE(ifStmt->getCondition() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(ifStmt->getCondition()) !=
            nullptr);
    std::shared_ptr<Literal> conditionLiteral =
        std::dynamic_pointer_cast<Literal>(ifStmt->getCondition());
    REQUIRE(conditionLiteral->getValue().type() == typeid(bool));
    CHECK(std::any_cast<bool>(conditionLiteral->getValue()) == true);

    REQUIRE(ifStmt->getThenBranch() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(ifStmt->getThenBranch()) !=
            nullptr);
    std::shared_ptr<Expression> thenExprStmt =
        std::dynamic_pointer_cast<Expression>(ifStmt->getThenBranch());
    REQUIRE(thenExprStmt->getExpression() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(thenExprStmt->getExpression()) !=
            nullptr);
    std::shared_ptr<Literal> thenLiteralExpr =
        std::dynamic_pointer_cast<Literal>(thenExprStmt->getExpression());
    REQUIRE(thenLiteralExpr->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(thenLiteralExpr->getValue()) == 1);

    REQUIRE(ifStmt->getElseBranch() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(ifStmt->getElseBranch()) !=
            nullptr);
    std::shared_ptr<Expression> elseExprStmt =
        std::dynamic_pointer_cast<Expression>(ifStmt->getElseBranch());
    REQUIRE(elseExprStmt->getExpression() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(elseExprStmt->getExpression()) !=
            nullptr);
    std::shared_ptr<Literal> elseLiteralExpr =
        std::dynamic_pointer_cast<Literal>(elseExprStmt->getExpression());
    REQUIRE(elseLiteralExpr->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(elseLiteralExpr->getValue()) == 2);
  }
}

TEST_CASE("Parsing while statement", "[parser][statement][while]") {
  Token whileToken = {TokenType::WHILE, "while", nullptr, 1};
  Token leftParenToken = {TokenType::LEFT_PAREN, "(", nullptr, 1};
  Token rightParenToken = {TokenType::RIGHT_PAREN, ")", nullptr, 1};
  Token trueToken = {TokenType::TRUE, "true", nullptr, 1};
  Token oneNumToken = {TokenType::NUMBER, "1", 1, 1};
  Token semicolonToken = {TokenType::SEMICOLON, ";", nullptr, 1};
  Token EOFToken = {TokenType::END_OF_FILE, "", nullptr, 1};

  std::vector<Token> tokens = {whileToken,      leftParenToken, trueToken,
                               rightParenToken, oneNumToken,    semicolonToken,
                               EOFToken};
  Parser parser(tokens);

  std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
  REQUIRE(statements.size() == 1);
  REQUIRE(statements[0] != nullptr);
  REQUIRE(std::dynamic_pointer_cast<While>(statements[0]) != nullptr);
  std::shared_ptr<While> whileStmt =
      std::dynamic_pointer_cast<While>(statements[0]);

  REQUIRE(whileStmt->getCondition() != nullptr);
  REQUIRE(std::dynamic_pointer_cast<Literal>(whileStmt->getCondition()) !=
          nullptr);
  std::shared_ptr<Literal> conditionLiteral =
      std::dynamic_pointer_cast<Literal>(whileStmt->getCondition());
  REQUIRE(conditionLiteral->getValue().type() == typeid(bool));
  CHECK(std::any_cast<bool>(conditionLiteral->getValue()) == true);

  REQUIRE(whileStmt->getBody() != nullptr);
  REQUIRE(std::dynamic_pointer_cast<Expression>(whileStmt->getBody()) !=
          nullptr);
  std::shared_ptr<Expression> bodyExprStmt =
      std::dynamic_pointer_cast<Expression>(whileStmt->getBody());
  REQUIRE(bodyExprStmt->getExpression() != nullptr);
  REQUIRE(std::dynamic_pointer_cast<Literal>(bodyExprStmt->getExpression()) !=
          nullptr);
  std::shared_ptr<Literal> bodyLiteralExpr =
      std::dynamic_pointer_cast<Literal>(bodyExprStmt->getExpression());
  REQUIRE(bodyLiteralExpr->getValue().type() == typeid(int));
  CHECK(std::any_cast<int>(bodyLiteralExpr->getValue()) == 1);
}

TEST_CASE("Parsing and desugaring for statement", "[parser][statement][for]") {
  Token forToken = {TokenType::FOR, "for", nullptr, 1};
  Token leftParenToken = {TokenType::LEFT_PAREN, "(", nullptr, 1};
  Token rightParenToken = {TokenType::RIGHT_PAREN, ")", nullptr, 1};
  Token varToken = {TokenType::VAR, "var", nullptr, 1};
  Token identifierToken = {TokenType::IDENTIFIER, "i", nullptr, 1};
  Token equalToken = {TokenType::EQUAL, "=", nullptr, 1};
  Token zeroNumToken = {TokenType::NUMBER, "0", 0, 1};
  Token lessToken = {TokenType::LESS, "<", nullptr, 1};
  Token tenNumToken = {TokenType::NUMBER, "10", 10, 1};
  Token plusToken = {TokenType::PLUS, "+", nullptr, 1};
  Token oneNumToken = {TokenType::NUMBER, "1", 1, 1};
  Token printToken = {TokenType::PRINT, "print", nullptr, 1};
  Token semicolonToken = {TokenType::SEMICOLON, ";", nullptr, 1};
  Token EOFToken = {TokenType::END_OF_FILE, "", nullptr, 1};

  SECTION("For without parameters") {
    /* Code:          for(;;) print 1;
     * Desugaring it: while(true) print 1;
     */
    std::vector<Token> tokens = {
        forToken,       leftParenToken,  semicolonToken,
        semicolonToken, rightParenToken, printToken,
        oneNumToken,    semicolonToken,  EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<While>(statements[0]) != nullptr);
    std::shared_ptr<While> whileStmt =
        std::dynamic_pointer_cast<While>(statements[0]);

    REQUIRE(whileStmt->getCondition() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(whileStmt->getCondition()) !=
            nullptr);
    std::shared_ptr<Literal> conditionLiteral =
        std::dynamic_pointer_cast<Literal>(whileStmt->getCondition());
    REQUIRE(conditionLiteral->getValue().type() == typeid(bool));
    CHECK(std::any_cast<bool>(conditionLiteral->getValue()) == true);

    REQUIRE(whileStmt->getBody() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Print>(whileStmt->getBody()) != nullptr);
    std::shared_ptr<Print> printStmt =
        std::dynamic_pointer_cast<Print>(whileStmt->getBody());
    REQUIRE(printStmt->getExpression() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(printStmt->getExpression()) !=
            nullptr);
    std::shared_ptr<Literal> literalExpr =
        std::dynamic_pointer_cast<Literal>(printStmt->getExpression());
    REQUIRE(literalExpr->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(literalExpr->getValue()) == 1);
  }

  SECTION("For with only initializer") {
    /* Code:          for(var i = 0; ;) print 1;
     * Desugaring it: {var i = 0; while(true) print 1;}
     */
    std::vector<Token> tokens = {
        forToken,        leftParenToken, varToken,       identifierToken,
        equalToken,      zeroNumToken,   semicolonToken, semicolonToken,
        rightParenToken, printToken,     oneNumToken,    semicolonToken,
        EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Block>(statements[0]) != nullptr);
    std::shared_ptr<Block> blockStmt =
        std::dynamic_pointer_cast<Block>(statements[0]);
    REQUIRE(blockStmt->getStatements().size() == 2);

    REQUIRE(blockStmt->getStatements()[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Var>(blockStmt->getStatements()[0]) !=
            nullptr);
    std::shared_ptr<Var> varStmt =
        std::dynamic_pointer_cast<Var>(blockStmt->getStatements()[0]);
    REQUIRE(varStmt->getName().getType() == TokenType::IDENTIFIER);
    CHECK(varStmt->getName().getLexeme() == "i");
    REQUIRE(varStmt->getInitializer() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(varStmt->getInitializer()) !=
            nullptr);
    std::shared_ptr<Literal> initializerLiteral =
        std::dynamic_pointer_cast<Literal>(varStmt->getInitializer());
    REQUIRE(initializerLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(initializerLiteral->getValue()) == 0);

    REQUIRE(blockStmt->getStatements()[1] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<While>(blockStmt->getStatements()[1]) !=
            nullptr);
    std::shared_ptr<While> whileStmt =
        std::dynamic_pointer_cast<While>(blockStmt->getStatements()[1]);
    REQUIRE(whileStmt->getCondition() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(whileStmt->getCondition()) !=
            nullptr);
    std::shared_ptr<Literal> conditionLiteral =
        std::dynamic_pointer_cast<Literal>(whileStmt->getCondition());
    REQUIRE(conditionLiteral->getValue().type() == typeid(bool));
    CHECK(std::any_cast<bool>(conditionLiteral->getValue()) == true);

    REQUIRE(whileStmt->getBody() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Print>(whileStmt->getBody()) != nullptr);
    std::shared_ptr<Print> printStmt =
        std::dynamic_pointer_cast<Print>(whileStmt->getBody());
    REQUIRE(printStmt->getExpression() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(printStmt->getExpression()) !=
            nullptr);
    std::shared_ptr<Literal> literalExpr =
        std::dynamic_pointer_cast<Literal>(printStmt->getExpression());
    REQUIRE(literalExpr->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(literalExpr->getValue()) == 1);
  }

  SECTION("For with only condition") {
    /* Code:          for(; 1 < 10;) print 1;
     * Desugaring it: while(1 < 10) print 1;
     */
    std::vector<Token> tokens = {
        forToken,   leftParenToken, semicolonToken, oneNumToken,
        lessToken,  tenNumToken,    semicolonToken, rightParenToken,
        printToken, oneNumToken,    semicolonToken, EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<While>(statements[0]) != nullptr);
    std::shared_ptr<While> whileStmt =
        std::dynamic_pointer_cast<While>(statements[0]);

    REQUIRE(whileStmt->getCondition() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Binary>(whileStmt->getCondition()) !=
            nullptr);
    std::shared_ptr<Binary> conditionBinary =
        std::dynamic_pointer_cast<Binary>(whileStmt->getCondition());
    CHECK(conditionBinary->getOp().getType() == TokenType::LESS);
    REQUIRE(conditionBinary->getLeft() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(conditionBinary->getLeft()) !=
            nullptr);
    std::shared_ptr<Literal> leftLiteral =
        std::dynamic_pointer_cast<Literal>(conditionBinary->getLeft());
    REQUIRE(leftLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(leftLiteral->getValue()) == 1);
    REQUIRE(conditionBinary->getRight() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(conditionBinary->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> rightLiteral =
        std::dynamic_pointer_cast<Literal>(conditionBinary->getRight());
    REQUIRE(rightLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(rightLiteral->getValue()) == 10);

    REQUIRE(whileStmt->getBody() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Print>(whileStmt->getBody()) != nullptr);
    std::shared_ptr<Print> printStmt =
        std::dynamic_pointer_cast<Print>(whileStmt->getBody());
    REQUIRE(printStmt->getExpression() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(printStmt->getExpression()) !=
            nullptr);
    std::shared_ptr<Literal> literalExpr =
        std::dynamic_pointer_cast<Literal>(printStmt->getExpression());
    REQUIRE(literalExpr->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(literalExpr->getValue()) == 1);
  }

  SECTION("For with only increment") {
    /* Code:          for(;;i = i + 1) print 1;
     * Desugaring it: while(true) {print 1; i = i + 1;}
     */
    std::vector<Token> tokens = {
        forToken,        leftParenToken,  semicolonToken,  semicolonToken,
        identifierToken, equalToken,      identifierToken, plusToken,
        oneNumToken,     rightParenToken, printToken,      oneNumToken,
        semicolonToken,  EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<While>(statements[0]) != nullptr);
    std::shared_ptr<While> whileStmt =
        std::dynamic_pointer_cast<While>(statements[0]);

    REQUIRE(whileStmt->getCondition() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(whileStmt->getCondition()) !=
            nullptr);
    std::shared_ptr<Literal> conditionLiteral =
        std::dynamic_pointer_cast<Literal>(whileStmt->getCondition());
    REQUIRE(conditionLiteral->getValue().type() == typeid(bool));
    CHECK(std::any_cast<bool>(conditionLiteral->getValue()) == true);

    REQUIRE(whileStmt->getBody() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Block>(whileStmt->getBody()) != nullptr);
    std::shared_ptr<Block> blockStmt =
        std::dynamic_pointer_cast<Block>(whileStmt->getBody());
    REQUIRE(blockStmt->getStatements().size() == 2);

    REQUIRE(blockStmt->getStatements()[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Print>(blockStmt->getStatements()[0]) !=
            nullptr);
    std::shared_ptr<Print> printStmt =
        std::dynamic_pointer_cast<Print>(blockStmt->getStatements()[0]);
    REQUIRE(printStmt->getExpression() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(printStmt->getExpression()) !=
            nullptr);
    std::shared_ptr<Literal> literalExpr =
        std::dynamic_pointer_cast<Literal>(printStmt->getExpression());
    REQUIRE(literalExpr->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(literalExpr->getValue()) == 1);

    REQUIRE(blockStmt->getStatements()[1] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(
                blockStmt->getStatements()[1]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(blockStmt->getStatements()[1]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Assign>(exprStmt->getExpression()) !=
            nullptr);
    std::shared_ptr<Assign> assignExpr =
        std::dynamic_pointer_cast<Assign>(exprStmt->getExpression());
    REQUIRE(assignExpr->getName().getType() == TokenType::IDENTIFIER);
    CHECK(assignExpr->getName().getLexeme() == "i");
    REQUIRE(assignExpr->getValue() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Binary>(assignExpr->getValue()) !=
            nullptr);
    std::shared_ptr<Binary> incrementBinary =
        std::dynamic_pointer_cast<Binary>(assignExpr->getValue());
    CHECK(incrementBinary->getOp().getType() == TokenType::PLUS);

    REQUIRE(incrementBinary->getLeft() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expr>(incrementBinary->getLeft()) !=
            nullptr);
    std::shared_ptr<Expr> leftExpr =
        std::dynamic_pointer_cast<Expr>(incrementBinary->getLeft());
    REQUIRE(std::dynamic_pointer_cast<Variable>(leftExpr) != nullptr);
    std::shared_ptr<Variable> leftVar =
        std::dynamic_pointer_cast<Variable>(leftExpr);
    REQUIRE(leftVar->getName().getType() == TokenType::IDENTIFIER);
    CHECK(leftVar->getName().getLexeme() == "i");

    REQUIRE(incrementBinary->getRight() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expr>(incrementBinary->getRight()) !=
            nullptr);
    std::shared_ptr<Expr> rightExpr =
        std::dynamic_pointer_cast<Expr>(incrementBinary->getRight());
    REQUIRE(std::dynamic_pointer_cast<Literal>(rightExpr) != nullptr);
    std::shared_ptr<Literal> rightLiteral =
        std::dynamic_pointer_cast<Literal>(rightExpr);
    REQUIRE(rightLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(rightLiteral->getValue()) == 1);
  }

  SECTION("Complete for statement") {
    /* Code:          for(var i = 0; i < 10; i = i + 1) print 1;
     * Desugaring it: { var i = 0; while(i < 10) { print 1; i = i + 1; } }
     */
    std::vector<Token> tokens = {
        forToken,        leftParenToken,  varToken,       identifierToken,
        equalToken,      zeroNumToken,    semicolonToken, identifierToken,
        lessToken,       tenNumToken,     semicolonToken, identifierToken,
        equalToken,      identifierToken, plusToken,      oneNumToken,
        rightParenToken, printToken,      oneNumToken,    semicolonToken,
        EOFToken};
    Parser parser(tokens);

    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    REQUIRE(statements.size() == 1);
    REQUIRE(statements[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Block>(statements[0]) != nullptr);
    std::shared_ptr<Block> blockStmt =
        std::dynamic_pointer_cast<Block>(statements[0]);
    REQUIRE(blockStmt->getStatements().size() == 2);

    REQUIRE(blockStmt->getStatements()[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Var>(blockStmt->getStatements()[0]) !=
            nullptr);
    std::shared_ptr<Var> varStmt =
        std::dynamic_pointer_cast<Var>(blockStmt->getStatements()[0]);
    REQUIRE(varStmt->getName().getType() == TokenType::IDENTIFIER);
    CHECK(varStmt->getName().getLexeme() == "i");
    REQUIRE(varStmt->getInitializer() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(varStmt->getInitializer()) !=
            nullptr);
    std::shared_ptr<Literal> initializerLiteral =
        std::dynamic_pointer_cast<Literal>(varStmt->getInitializer());
    REQUIRE(initializerLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(initializerLiteral->getValue()) == 0);

    REQUIRE(blockStmt->getStatements()[1] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<While>(blockStmt->getStatements()[1]) !=
            nullptr);
    std::shared_ptr<While> whileStmt =
        std::dynamic_pointer_cast<While>(blockStmt->getStatements()[1]);

    REQUIRE(whileStmt->getCondition() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Binary>(whileStmt->getCondition()) !=
            nullptr);
    std::shared_ptr<Binary> conditionBinary =
        std::dynamic_pointer_cast<Binary>(whileStmt->getCondition());
    CHECK(conditionBinary->getOp().getType() == TokenType::LESS);
    REQUIRE(conditionBinary->getLeft() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Variable>(conditionBinary->getLeft()) !=
            nullptr);
    std::shared_ptr<Variable> leftVar =
        std::dynamic_pointer_cast<Variable>(conditionBinary->getLeft());
    REQUIRE(leftVar->getName().getType() == TokenType::IDENTIFIER);
    CHECK(leftVar->getName().getLexeme() == "i");
    REQUIRE(conditionBinary->getRight() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(conditionBinary->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> rightLiteral =
        std::dynamic_pointer_cast<Literal>(conditionBinary->getRight());
    REQUIRE(rightLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(rightLiteral->getValue()) == 10);

    REQUIRE(whileStmt->getBody() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Block>(whileStmt->getBody()) != nullptr);
    std::shared_ptr<Block> whileBlockStmt =
        std::dynamic_pointer_cast<Block>(whileStmt->getBody());
    REQUIRE(whileBlockStmt->getStatements().size() == 2);

    REQUIRE(whileBlockStmt->getStatements()[0] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Print>(
                whileBlockStmt->getStatements()[0]) != nullptr);
    std::shared_ptr<Print> printStmt =
        std::dynamic_pointer_cast<Print>(whileBlockStmt->getStatements()[0]);
    REQUIRE(printStmt->getExpression() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(printStmt->getExpression()) !=
            nullptr);
    std::shared_ptr<Literal> literalExpr =
        std::dynamic_pointer_cast<Literal>(printStmt->getExpression());
    REQUIRE(literalExpr->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(literalExpr->getValue()) == 1);

    REQUIRE(whileBlockStmt->getStatements()[1] != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Expression>(
                whileBlockStmt->getStatements()[1]) != nullptr);
    std::shared_ptr<Expression> exprStmt =
        std::dynamic_pointer_cast<Expression>(
            whileBlockStmt->getStatements()[1]);
    REQUIRE(exprStmt->getExpression() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Assign>(exprStmt->getExpression()) !=
            nullptr);
    std::shared_ptr<Assign> assignExpr =
        std::dynamic_pointer_cast<Assign>(exprStmt->getExpression());
    REQUIRE(assignExpr->getName().getType() == TokenType::IDENTIFIER);
    CHECK(assignExpr->getName().getLexeme() == "i");
    REQUIRE(assignExpr->getValue() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Binary>(assignExpr->getValue()) !=
            nullptr);
    std::shared_ptr<Binary> incrementBinary =
        std::dynamic_pointer_cast<Binary>(assignExpr->getValue());
    CHECK(incrementBinary->getOp().getType() == TokenType::PLUS);
    REQUIRE(incrementBinary->getLeft() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Variable>(incrementBinary->getLeft()) !=
            nullptr);
    std::shared_ptr<Variable> leftVar2 =
        std::dynamic_pointer_cast<Variable>(incrementBinary->getLeft());
    REQUIRE(leftVar2->getName().getType() == TokenType::IDENTIFIER);
    CHECK(leftVar2->getName().getLexeme() == "i");
    REQUIRE(incrementBinary->getRight() != nullptr);
    REQUIRE(std::dynamic_pointer_cast<Literal>(incrementBinary->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> rightLiteral2 =
        std::dynamic_pointer_cast<Literal>(incrementBinary->getRight());
    REQUIRE(rightLiteral2->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(rightLiteral2->getValue()) == 1);
  }
}
