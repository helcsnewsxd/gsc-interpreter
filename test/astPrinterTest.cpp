#include "dbg/astPrinter.hpp"
#include "catch2/catch_amalgamated.hpp"

TEST_CASE("ASTPrinter for literal expression", "[astPrinter][literal]") {
  AstPrinter printer;

  SECTION("Literal nullptr") {
    Literal literalExpr(nullptr);
    std::string result = printer.print(std::make_shared<Literal>(literalExpr));

    CHECK(result == "nil");
  }

  SECTION("Literal boolean") {
    Literal literalExpr(true);
    std::string result = printer.print(std::make_shared<Literal>(literalExpr));
    CHECK(result == "true");

    Literal literalExprFalse(false);
    std::string resultFalse =
        printer.print(std::make_shared<Literal>(literalExprFalse));
    CHECK(resultFalse == "false");
  }

  SECTION("Literal number") {
    Literal literalExpr(42);
    std::string result = printer.print(std::make_shared<Literal>(literalExpr));
    CHECK(result == "42");
  }

  SECTION("Literal string") {
    Literal literalExpr("Hello, World!");
    std::string result = printer.print(std::make_shared<Literal>(literalExpr));
    CHECK(result == "Hello, World!");
  }

  SECTION("Literal unknown type") {
    AstPrinter literalValue;
    Literal literalExpr(literalValue);

    CHECK_THROWS_AS(printer.print(std::make_shared<Literal>(literalExpr)),
                    std::runtime_error);
  }
}

TEST_CASE("ASTPrinter for grouping expression", "[astPrinter][grouping]") {
  AstPrinter printer;
  Literal literalExpr(nullptr);
  Grouping groupingExpr(std::make_shared<Literal>(literalExpr));

  std::string result = printer.print(std::make_shared<Grouping>(groupingExpr));

  CHECK(result == "(group nil)");
}

TEST_CASE("ASTPrinter for unary expression", "[astPrinter][unary]") {
  AstPrinter printer;
  Literal literalExpr(42);
  Token op(TokenType::MINUS, "-", nullptr, 1);
  Unary unaryExpr(op, std::make_shared<Literal>(literalExpr));

  std::string result = printer.print(std::make_shared<Unary>(unaryExpr));

  CHECK(result == "(- 42)");
}

TEST_CASE("ASTPrinter for binary expression", "[astPrinter][binary]") {
  AstPrinter printer;
  Literal leftLiteral(42);
  Literal rightLiteral(43);
  Token op(TokenType::PLUS, "+", nullptr, 1);
  Binary binaryExpr(std::make_shared<Literal>(leftLiteral), op,
                    std::make_shared<Literal>(rightLiteral));

  std::string result = printer.print(std::make_shared<Binary>(binaryExpr));

  CHECK(result == "(+ 42 43)");
}
