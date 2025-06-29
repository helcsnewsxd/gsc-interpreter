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

TEST_CASE("Variable expression", "[expression][variable]") {
  SECTION("Variable expression with name") {
    Token varToken{TokenType::IDENTIFIER, "x", nullptr, 1};
    std::shared_ptr<Variable> varExpr = std::make_shared<Variable>(varToken);

    REQUIRE(typeid(varExpr->getName()) == typeid(Token));
    REQUIRE(varExpr->getName().getType() == TokenType::IDENTIFIER);
    CHECK(varExpr->getName().getLexeme() == "x");
  }
}

TEST_CASE("Assign expression", "[expression][assign]") {
  SECTION("Assign expression with variable name and value") {
    Token varToken{TokenType::IDENTIFIER, "x", nullptr, 1};
    Token initialValueToken{TokenType::NUMBER, "10", 10, 2};
    std::shared_ptr<Literal> initialValue =
        std::make_shared<Literal>(initialValueToken.getLiteral());
    std::shared_ptr<Assign> assignExpr =
        std::make_shared<Assign>(varToken, initialValue);

    REQUIRE(typeid(assignExpr->getName()) == typeid(Token));
    REQUIRE(assignExpr->getName().getType() == TokenType::IDENTIFIER);
    CHECK(assignExpr->getName().getLexeme() == "x");

    REQUIRE(std::dynamic_pointer_cast<Literal>(assignExpr->getValue()) !=
            nullptr);
    std::shared_ptr<Literal> valueLiteral =
        std::dynamic_pointer_cast<Literal>(assignExpr->getValue());
    REQUIRE(valueLiteral->getValue().type() == typeid(int));
    CHECK(std::any_cast<int>(valueLiteral->getValue()) == 10);
  }
}

TEST_CASE("Logic expression", "[expression][logic]") {
  // Literal true and false values
  Token trueToken{TokenType::TRUE, "true", true, 1};
  Token falseToken{TokenType::FALSE, "false", false, 2};

  SECTION("And operator") {
    Token op{TokenType::AND, "and", nullptr, 1};
    std::shared_ptr<Literal> left =
        std::make_shared<Literal>(trueToken.getLiteral());
    std::shared_ptr<Literal> right =
        std::make_shared<Literal>(falseToken.getLiteral());
    std::shared_ptr<Logical> logicExpr =
        std::make_shared<Logical>(left, op, right);

    REQUIRE(std::dynamic_pointer_cast<Literal>(logicExpr->getLeft()) !=
            nullptr);
    std::shared_ptr<Literal> leftPtr =
        std::dynamic_pointer_cast<Literal>(logicExpr->getLeft());
    REQUIRE(leftPtr != nullptr);
    REQUIRE(leftPtr->getValue().type() == typeid(bool));
    CHECK(std::any_cast<bool>(leftPtr->getValue()) == true);

    REQUIRE(std::dynamic_pointer_cast<Literal>(logicExpr->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> rightPtr =
        std::dynamic_pointer_cast<Literal>(logicExpr->getRight());
    REQUIRE(rightPtr != nullptr);
    REQUIRE(rightPtr->getValue().type() == typeid(bool));
    CHECK(std::any_cast<bool>(rightPtr->getValue()) == false);

    REQUIRE(typeid(logicExpr->getOp()) == typeid(Token));
    CHECK(logicExpr->getOp().getType() == TokenType::AND);
  }

  SECTION("And operator") {
    Token op{TokenType::OR, "or", nullptr, 1};
    std::shared_ptr<Literal> left =
        std::make_shared<Literal>(trueToken.getLiteral());
    std::shared_ptr<Literal> right =
        std::make_shared<Literal>(falseToken.getLiteral());
    std::shared_ptr<Logical> logicExpr =
        std::make_shared<Logical>(left, op, right);

    REQUIRE(std::dynamic_pointer_cast<Literal>(logicExpr->getLeft()) !=
            nullptr);
    std::shared_ptr<Literal> leftPtr =
        std::dynamic_pointer_cast<Literal>(logicExpr->getLeft());
    REQUIRE(leftPtr != nullptr);
    REQUIRE(leftPtr->getValue().type() == typeid(bool));
    CHECK(std::any_cast<bool>(leftPtr->getValue()) == true);

    REQUIRE(std::dynamic_pointer_cast<Literal>(logicExpr->getRight()) !=
            nullptr);
    std::shared_ptr<Literal> rightPtr =
        std::dynamic_pointer_cast<Literal>(logicExpr->getRight());
    REQUIRE(rightPtr != nullptr);
    REQUIRE(rightPtr->getValue().type() == typeid(bool));
    CHECK(std::any_cast<bool>(rightPtr->getValue()) == false);

    REQUIRE(typeid(logicExpr->getOp()) == typeid(Token));
    CHECK(logicExpr->getOp().getType() == TokenType::OR);
  }
}
