#include "gsc/interpreter.hpp"
#include "catch2/catch_amalgamated.hpp"
#include "gsc/error.hpp"
#include <iostream>
#include <memory>

TEST_CASE("Interpreting Print of Literal Expressions",
          "[interpreter][print][literal]") {
  Token EOFToken(TokenType::END_OF_FILE, "", 0, 3);

  // Redirect output to a string stream
  std::ostringstream oss;
  auto oldCout = std::cout.rdbuf(oss.rdbuf());

  SECTION("Integer literal") {
    Token intToken(TokenType::NUMBER, "42", 42, 1);
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(intToken.getLiteral());
    std::shared_ptr<Expr> expr = literalExpr;
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(oss.str() == "42\n");
  }

  SECTION("NIL literal") {
    Token nilToken(TokenType::NIL, "nil", nullptr, 1);
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(nilToken.getLiteral());
    std::shared_ptr<Expr> expr = literalExpr;
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(oss.str() == "nil\n");
  }

  SECTION("Boolean literal (true)") {
    Token trueToken(TokenType::TRUE, "true", true, 1);
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(trueToken.getLiteral());
    std::shared_ptr<Expr> expr = literalExpr;
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(oss.str() == "true\n");
  }

  SECTION("Boolean literal (false)") {
    Token falseToken(TokenType::FALSE, "false", false, 1);
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(falseToken.getLiteral());
    std::shared_ptr<Expr> expr = literalExpr;
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(oss.str() == "false\n");
  }

  SECTION("String literal") {
    Token stringToken(TokenType::STRING, "Hello, World!", "Hello, World!", 1);
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(stringToken.getLiteral());
    std::shared_ptr<Expr> expr = literalExpr;
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(oss.str() == "Hello, World!\n");
  }

  // Restore the original cout buffer
  std::cout.rdbuf(oldCout);
}

TEST_CASE("Interpreting Grouping Expressions", "[interpreter][grouping]") {
  // Redirect output to a string stream
  std::ostringstream oss;
  auto oldCout = std::cout.rdbuf(oss.rdbuf());

  SECTION("Integer grouping expressions") {
    Token intToken(TokenType::NUMBER, "42", 42, 1);
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(intToken.getLiteral());
    std::shared_ptr<Expr> expr = std::make_shared<Grouping>(literalExpr);
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(oss.str() == "42\n");
  }

  SECTION("NIL grouping expressions") {
    Token nilToken(TokenType::NIL, "nil", nullptr, 1);
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(nilToken.getLiteral());
    std::shared_ptr<Expr> expr = std::make_shared<Grouping>(literalExpr);
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(oss.str() == "nil\n");
  }

  SECTION("Boolean grouping expressions (true)") {
    Token trueToken(TokenType::TRUE, "true", true, 1);
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(trueToken.getLiteral());
    std::shared_ptr<Expr> expr = std::make_shared<Grouping>(literalExpr);
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(oss.str() == "true\n");
  }

  SECTION("Boolean grouping expressions (false)") {
    Token falseToken(TokenType::FALSE, "false", false, 1);
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(falseToken.getLiteral());
    std::shared_ptr<Expr> expr = std::make_shared<Grouping>(literalExpr);
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(oss.str() == "false\n");
  }

  SECTION("String grouping expressions") {
    Token stringToken(TokenType::STRING, "Hello, World!", "Hello, World!", 1);
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(stringToken.getLiteral());
    std::shared_ptr<Expr> expr = std::make_shared<Grouping>(literalExpr);
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(oss.str() == "Hello, World!\n");
  }

  // Restore the original cout buffer
  std::cout.rdbuf(oldCout);
}

TEST_CASE("Interpreting Unary Expressions", "[interpreter][unary]") {
  Token integerToken(TokenType::NUMBER, "42", 42, 1);
  Token booleanToken(TokenType::TRUE, "true", true, 1);
  Token minusToken(TokenType::MINUS, "-", nullptr, 1);
  Token bangToken(TokenType::BANG, "!", nullptr, 1);

  // Redirect output to a string stream
  std::ostringstream oss;
  auto oldCout = std::cout.rdbuf(oss.rdbuf());

  SECTION("MINUS unary expression") {
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(integerToken.getLiteral());
    std::shared_ptr<Unary> unaryExpr =
        std::make_shared<Unary>(minusToken, literalExpr);
    std::shared_ptr<Expr> expr = unaryExpr;
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(oss.str() == "-42\n");
  }

  SECTION("MINUS (x2) unary expression") {
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(integerToken.getLiteral());
    std::shared_ptr<Unary> unaryExpr1 =
        std::make_shared<Unary>(minusToken, literalExpr);
    std::shared_ptr<Unary> unaryExpr2 =
        std::make_shared<Unary>(minusToken, unaryExpr1);
    std::shared_ptr<Expr> expr = unaryExpr2;
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(oss.str() == "42\n");
  }

  SECTION("BANG unary expression (true)") {
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(booleanToken.getLiteral());
    std::shared_ptr<Unary> unaryExpr =
        std::make_shared<Unary>(bangToken, literalExpr);
    std::shared_ptr<Expr> expr = unaryExpr;
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(oss.str() == "false\n");
  }

  SECTION("BANG (x2) unary expression (true)") {
    std::shared_ptr<Literal> literalExpr =
        std::make_shared<Literal>(booleanToken.getLiteral());
    std::shared_ptr<Unary> unaryExpr1 =
        std::make_shared<Unary>(bangToken, literalExpr);
    std::shared_ptr<Unary> unaryExpr2 =
        std::make_shared<Unary>(bangToken, unaryExpr1);
    std::shared_ptr<Expr> expr = unaryExpr2;
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(oss.str() == "true\n");
  }

  // Restore the original cout buffer
  std::cout.rdbuf(oldCout);
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

  // Redirect output to a string stream
  std::ostringstream outputStream;
  auto oldCout = std::cout.rdbuf(outputStream.rdbuf());

  SECTION("Sumation (1+2)") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(oneToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(twoToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, plusToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(outputStream.str() == "3\n");
  }

  SECTION("Concatenation of strings") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(stringToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(stringToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, plusToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(outputStream.str() == "Hello, World!Hello, World!\n");
  }

  SECTION("Subtraction (3-2)") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(threeToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(twoToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, minusToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(outputStream.str() == "1\n");
  }

  SECTION("Multiplication (2*3)") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(twoToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(threeToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, starToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(outputStream.str() == "6\n");
  }

  SECTION("Division (3/2)") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(threeToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(twoToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, slashToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(outputStream.str() == "1\n");
  }

  SECTION("Division by Zero") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(threeToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(zeroToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, slashToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(outputStream.str() == "");
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
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(outputStream.str() == "true\n");
  }

  SECTION("Inequality (1!=2)") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(oneToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(twoToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, bangEqualToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(outputStream.str() == "true\n");
  }

  SECTION("Greater than (2>1)") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(twoToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(oneToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, greaterToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(outputStream.str() == "true\n");
  }

  SECTION("Greater or equal than (1>=1)") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(oneToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(oneToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, greaterEqualToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(outputStream.str() == "true\n");
  }

  SECTION("Less than (1<2)") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(oneToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(twoToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, lessToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(outputStream.str() == "true\n");
  }

  SECTION("Less or equal than (1<=1)") {
    std::shared_ptr<Literal> leftExpr =
        std::make_shared<Literal>(oneToken.getLiteral());
    std::shared_ptr<Literal> rightExpr =
        std::make_shared<Literal>(oneToken.getLiteral());
    std::shared_ptr<Binary> binaryExpr =
        std::make_shared<Binary>(leftExpr, lessEqualToken, rightExpr);
    std::shared_ptr<Expr> expr = binaryExpr;
    std::shared_ptr<Print> printExpr = std::make_shared<Print>(expr);
    std::shared_ptr<Stmt> stmt = printExpr;

    Interpreter().interpret({stmt});
    CHECK(outputStream.str() == "true\n");
  }

  // Restore the original cerr buffer
  std::cerr.rdbuf(oldCerr);

  // Restore the original cout buffer
  std::cout.rdbuf(oldCout);
}

TEST_CASE("Interpreting multiple print statements",
          "[interpreter][statement][multiple]") {
  Token intToken(TokenType::NUMBER, "42", 42, 1);
  Token stringToken(TokenType::STRING, "Hello, World!", "Hello, World!", 1);

  // Redirect output to a string stream
  std::ostringstream oss;
  auto oldCout = std::cout.rdbuf(oss.rdbuf());

  std::shared_ptr<Literal> literalExpr =
      std::make_shared<Literal>(intToken.getLiteral());
  std::shared_ptr<Expr> expr1 = literalExpr;
  std::shared_ptr<Print> printExpr1 = std::make_shared<Print>(expr1);
  std::shared_ptr<Stmt> stmt1 = printExpr1;

  std::shared_ptr<Literal> stringLiteralExpr =
      std::make_shared<Literal>(stringToken.getLiteral());
  std::shared_ptr<Expr> expr2 = stringLiteralExpr;
  std::shared_ptr<Print> printExpr2 = std::make_shared<Print>(expr2);
  std::shared_ptr<Stmt> stmt2 = printExpr2;

  std::vector<std::shared_ptr<Stmt>> statements = {stmt1, stmt2};
  Interpreter().interpret(statements);
  CHECK(oss.str() == "42\nHello, World!\n");

  // Restore the original cout buffer
  std::cout.rdbuf(oldCout);
}

TEST_CASE("Interpreting variable assignment",
          "[interpreter][statement][variable]") {
  Token nameToken(TokenType::IDENTIFIER, "x", "x", 1);
  Token intToken(TokenType::NUMBER, "42", 42, 1);
  Token stringToken(TokenType::STRING, "Hello, World!", "Hello, World!", 1);

  // Redirect output to a string stream
  std::ostringstream oss;
  auto oldCout = std::cout.rdbuf(oss.rdbuf());

  // var x = 42; {var x = "Hello, World!"; print x;}; print x;
  // stdout: Hello, World!\n42\n
  std::shared_ptr<Literal> literalExpr =
      std::make_shared<Literal>(intToken.getLiteral());
  std::shared_ptr<Expr> expr = literalExpr;
  std::shared_ptr<Var> varStmt = std::make_shared<Var>(nameToken, expr);
  std::shared_ptr<Stmt> stmt = varStmt;

  std::shared_ptr<Literal> stringLiteralExpr =
      std::make_shared<Literal>(stringToken.getLiteral());
  std::shared_ptr<Expr> stringExpr = stringLiteralExpr;
  std::shared_ptr<Var> varStringStmt =
      std::make_shared<Var>(nameToken, stringExpr);
  std::shared_ptr<Stmt> stringStmt = varStringStmt;

  std::shared_ptr<Print> printStmt =
      std::make_shared<Print>(std::make_shared<Variable>(nameToken));
  std::shared_ptr<Stmt> printStatement = printStmt;

  std::vector<std::shared_ptr<Stmt>> statements = {stringStmt, printStatement};
  std::shared_ptr<Block> blockStmt = std::make_shared<Block>(statements);
  std::shared_ptr<Stmt> block = blockStmt;

  std::vector<std::shared_ptr<Stmt>> allStatements = {stmt, block,
                                                      printStatement};
  Interpreter().interpret(allStatements);
  CHECK(oss.str() == "Hello, World!\n42\n");

  // Restore the original cout buffer
  std::cout.rdbuf(oldCout);
}
