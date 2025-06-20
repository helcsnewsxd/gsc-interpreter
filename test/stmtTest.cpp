#include "gsc/stmt.hpp"
#include "catch2/catch_amalgamated.hpp"

TEST_CASE("Expression statement", "[statement][expression]") {
  std::shared_ptr<Expr> expr = std::make_shared<Literal>(42);
  std::shared_ptr<Stmt> exprStmt = std::make_shared<Expression>(expr);

  REQUIRE(exprStmt != nullptr);
  REQUIRE(std::dynamic_pointer_cast<Expression>(exprStmt) != nullptr);
  std::shared_ptr<Expression> expressionStmt =
      std::dynamic_pointer_cast<Expression>(exprStmt);
  REQUIRE(expressionStmt->getExpression() != nullptr);
  REQUIRE(std::dynamic_pointer_cast<Literal>(expressionStmt->getExpression()) !=
          nullptr);
  std::shared_ptr<Literal> literalExpr =
      std::dynamic_pointer_cast<Literal>(expressionStmt->getExpression());
  REQUIRE(literalExpr->getValue().type() == typeid(int));
  CHECK(std::any_cast<int>(literalExpr->getValue()) == 42);
}

TEST_CASE("Print statement", "[statement][print]") {
  std::shared_ptr<Expr> expr =
      std::make_shared<Literal>(std::string("Hello, World!"));
  std::shared_ptr<Stmt> printStmt = std::make_shared<Print>(expr);

  REQUIRE(printStmt != nullptr);
  REQUIRE(std::dynamic_pointer_cast<Print>(printStmt) != nullptr);
  std::shared_ptr<Print> printStatement =
      std::dynamic_pointer_cast<Print>(printStmt);
  REQUIRE(printStatement->getExpression() != nullptr);
  REQUIRE(std::dynamic_pointer_cast<Literal>(printStatement->getExpression()) !=
          nullptr);
  std::shared_ptr<Literal> literalExpr =
      std::dynamic_pointer_cast<Literal>(printStatement->getExpression());
  REQUIRE(literalExpr->getValue().type() == typeid(std::string));
  CHECK(std::any_cast<std::string>(literalExpr->getValue()) == "Hello, World!");
}

TEST_CASE("Variable statement", "[statement][variable]") {
  Token name(TokenType::IDENTIFIER, "x", "x", 1);
  std::shared_ptr<Expr> initializer = std::make_shared<Literal>(100);
  std::shared_ptr<Var> varStmt = std::make_shared<Var>(name, initializer);
  std::shared_ptr<Stmt> stmt = std::dynamic_pointer_cast<Stmt>(varStmt);

  REQUIRE(stmt != nullptr);
  REQUIRE(std::dynamic_pointer_cast<Var>(stmt) != nullptr);
  std::shared_ptr<Var> varStatement = std::dynamic_pointer_cast<Var>(stmt);
  REQUIRE(varStatement->getName().getLexeme() == "x");
  REQUIRE(varStatement->getInitializer() != nullptr);
  REQUIRE(std::dynamic_pointer_cast<Literal>(varStatement->getInitializer()) !=
          nullptr);
  std::shared_ptr<Literal> literalExpr =
      std::dynamic_pointer_cast<Literal>(varStatement->getInitializer());
  REQUIRE(literalExpr->getValue().type() == typeid(int));
  CHECK(std::any_cast<int>(literalExpr->getValue()) == 100);
}

TEST_CASE("Block statement", "[statement][block]") {
  std::vector<std::shared_ptr<Stmt>> statements;
  statements.push_back(
      std::make_shared<Expression>(std::make_shared<Literal>(1)));
  statements.push_back(
      std::make_shared<Expression>(std::make_shared<Literal>(2)));
  std::shared_ptr<Block> blockStmt = std::make_shared<Block>(statements);
  std::shared_ptr<Stmt> stmt = std::dynamic_pointer_cast<Stmt>(blockStmt);

  REQUIRE(stmt != nullptr);
  REQUIRE(std::dynamic_pointer_cast<Block>(stmt) != nullptr);
  std::shared_ptr<Block> blockStatement =
      std::dynamic_pointer_cast<Block>(stmt);
  REQUIRE(blockStatement->getStatements().size() == 2);
}

TEST_CASE("If statement", "[statement][if]") {
  std::shared_ptr<Expr> condition = std::make_shared<Literal>(true);
  std::shared_ptr<Stmt> thenBranch =
      std::make_shared<Expression>(std::make_shared<Literal>(42));
  std::shared_ptr<Stmt> elseBranch =
      std::make_shared<Expression>(std::make_shared<Literal>(0));
  std::shared_ptr<If> ifStmt =
      std::make_shared<If>(condition, thenBranch, elseBranch);
  std::shared_ptr<Stmt> stmt = std::dynamic_pointer_cast<Stmt>(ifStmt);

  REQUIRE(stmt != nullptr);
  REQUIRE(std::dynamic_pointer_cast<If>(stmt) != nullptr);
  std::shared_ptr<If> ifStatement = std::dynamic_pointer_cast<If>(stmt);

  REQUIRE(ifStatement->getCondition() != nullptr);
  REQUIRE(std::dynamic_pointer_cast<Literal>(ifStatement->getCondition()) !=
          nullptr);
  std::shared_ptr<Literal> literalCondition =
      std::dynamic_pointer_cast<Literal>(ifStatement->getCondition());
  REQUIRE(literalCondition->getValue().type() == typeid(bool));
  CHECK(std::any_cast<bool>(literalCondition->getValue()) == true);

  REQUIRE(ifStatement->getThenBranch() != nullptr);
  REQUIRE(std::dynamic_pointer_cast<Expression>(ifStatement->getThenBranch()) !=
          nullptr);
  std::shared_ptr<Expression> thenExpr =
      std::dynamic_pointer_cast<Expression>(ifStatement->getThenBranch());
  REQUIRE(thenExpr->getExpression() != nullptr);
  REQUIRE(std::dynamic_pointer_cast<Literal>(thenExpr->getExpression()) !=
          nullptr);
  std::shared_ptr<Literal> thenLiteral =
      std::dynamic_pointer_cast<Literal>(thenExpr->getExpression());
  REQUIRE(thenLiteral->getValue().type() == typeid(int));
  CHECK(std::any_cast<int>(thenLiteral->getValue()) == 42);

  REQUIRE(ifStatement->getElseBranch() != nullptr);
  REQUIRE(std::dynamic_pointer_cast<Expression>(ifStatement->getElseBranch()) !=
          nullptr);
  std::shared_ptr<Expression> elseExpr =
      std::dynamic_pointer_cast<Expression>(ifStatement->getElseBranch());
  REQUIRE(elseExpr->getExpression() != nullptr);
  REQUIRE(std::dynamic_pointer_cast<Literal>(elseExpr->getExpression()) !=
          nullptr);
  std::shared_ptr<Literal> elseLiteral =
      std::dynamic_pointer_cast<Literal>(elseExpr->getExpression());
  REQUIRE(elseLiteral->getValue().type() == typeid(int));
  CHECK(std::any_cast<int>(elseLiteral->getValue()) == 0);
}
