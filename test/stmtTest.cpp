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
