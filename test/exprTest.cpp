#include "gsc/expr.hpp"
#include "catch2/catch_amalgamated.hpp"

TEST_CASE("Literal expression", "[expression][literal]") {
  Literal literalExpr(1);

  REQUIRE(literalExpr.getValue().type() == typeid(int));
  CHECK(std::any_cast<int>(literalExpr.getValue()) == 1);
}

TEST_CASE("Grouping expression", "[expression][grouping]") {
  Literal innerLiteral(1);
  Grouping groupingExpr(std::make_shared<Literal>(innerLiteral));

  // Check if it's a pointer to a Literal class object
  std::shared_ptr<Literal> literalPtr =
      std::dynamic_pointer_cast<Literal>(groupingExpr.getExpression());
  REQUIRE(literalPtr != nullptr);

  // Check if the Literal object has the expected value
  CHECK(literalPtr->getValue().type() == typeid(int));
  CHECK(std::any_cast<int>(literalPtr->getValue()) == 1);
}

TEST_CASE("Unary expression", "[expression][unary]") {
  Token op{TokenType::MINUS, "-", nullptr, 1};
  Literal innerLiteral(1);
  Unary unaryExpr(op, std::make_shared<Literal>(innerLiteral));

  // Check right side of the expression
  std::shared_ptr<Literal> rightPtr =
      std::dynamic_pointer_cast<Literal>(unaryExpr.getRight());
  REQUIRE(rightPtr != nullptr);
  CHECK(rightPtr->getValue().type() == typeid(int));
  CHECK(std::any_cast<int>(rightPtr->getValue()) == 1);

  // Check if the operator is correct
  Token tokOp = std::any_cast<Token>(unaryExpr.getOp());

  REQUIRE(typeid(tokOp) == typeid(Token));
  CHECK(tokOp.getType() == TokenType::MINUS);
  CHECK(tokOp.getLexeme() == "-");
  CHECK(tokOp.getLine() == 1);

  std::any opLiteral = tokOp.getLiteral();
  CHECK(opLiteral.type() == typeid(std::nullptr_t));
}

TEST_CASE("Binary expression", "[expression][binary]") {
  Token op{TokenType::MINUS, "-", nullptr, 1};
  Literal leftLiteral(1);
  Literal rightLiteral(2);
  Binary binaryExpr(std::make_shared<Literal>(leftLiteral), op,
                    std::make_shared<Literal>(rightLiteral));

  // Check left side of the expression
  std::shared_ptr<Literal> leftPtr =
      std::dynamic_pointer_cast<Literal>(binaryExpr.getLeft());
  REQUIRE(leftPtr != nullptr);
  CHECK(leftPtr->getValue().type() == typeid(int));
  CHECK(std::any_cast<int>(leftPtr->getValue()) == 1);

  // Check right side of the expression
  std::shared_ptr<Literal> rightPtr =
      std::dynamic_pointer_cast<Literal>(binaryExpr.getRight());
  REQUIRE(rightPtr != nullptr);
  CHECK(rightPtr->getValue().type() == typeid(int));
  CHECK(std::any_cast<int>(rightPtr->getValue()) == 2);

  // Check if the operator is correct
  Token tokOp = std::any_cast<Token>(binaryExpr.getOp());
  REQUIRE(typeid(tokOp) == typeid(Token));
  CHECK(tokOp.getType() == TokenType::MINUS);
  CHECK(tokOp.getLexeme() == "-");

  std::any opLiteral = tokOp.getLiteral();
  CHECK(opLiteral.type() == typeid(std::nullptr_t));
}
