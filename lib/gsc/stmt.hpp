#pragma once

#include "gsc/expr.hpp"
#include <any>
#include <memory>
#include <vector>

class Block;
class Expression;
class Print;
class Var;
class If;
class While;

/** @class StmtVisitor
 * @brief Abstract base class for statement visitors.
 *
 * @note This class defines the interface for visiting different types of
 * statements into the GSC interpreter.
 */
class StmtVisitor {
public:
  virtual std::any visitBlockStmt(std::shared_ptr<Block> stmt) = 0;
  virtual std::any visitExpressionStmt(std::shared_ptr<Expression> expr) = 0;
  virtual std::any visitPrintStmt(std::shared_ptr<Print> stmt) = 0;
  virtual std::any visitVarStmt(std::shared_ptr<Var> stmt) = 0;
  virtual std::any visitIfStmt(std::shared_ptr<If> stmt) = 0;
  virtual std::any visitWhileStmt(std::shared_ptr<While> stmt) = 0;

  virtual ~StmtVisitor() = default;
};

/** @class Stmt
 * @brief Abstract base class for statements.
 *
 * @note This class defines the interface for statements in the GSC interpreter.
 */
class Stmt {
public:
  virtual std::any accept(StmtVisitor &visitor) = 0;
};

/** @class Block
 * @brief Represents a block of statements.
 *
 * @note This class holds a vector of statements and allows visiting them.
 */
class Block : public Stmt, public std::enable_shared_from_this<Block> {
private:
  const std::vector<std::shared_ptr<Stmt>> statements;

public:
  Block(std::vector<std::shared_ptr<Stmt>> statements)
      : statements(std::move(statements)) {}

  std::any accept(StmtVisitor &visitor) override {
    return visitor.visitBlockStmt(shared_from_this());
  }

  std::vector<std::shared_ptr<Stmt>> getStatements() const {
    return statements;
  }
};

/** @class Expression
 * @brief Represents an expression statement.
 *
 * @note This class holds a single expression and allows visiting it.
 */
class Expression : public Stmt,
                   public std::enable_shared_from_this<Expression> {
private:
  std::shared_ptr<Expr> expression;

public:
  Expression(const std::shared_ptr<Expr> &expression)
      : expression(std::move(expression)) {}

  std::any accept(StmtVisitor &visitor) override {
    return visitor.visitExpressionStmt(shared_from_this());
  }

  std::shared_ptr<Expr> getExpression() const { return expression; }
};

/** @class Print
 * @brief Represents a print statement.
 *
 * @note This class holds an expression to be printed and allows visiting it.
 */
class Print : public Stmt, public std::enable_shared_from_this<Print> {
private:
  std::shared_ptr<Expr> expression;

public:
  Print(const std::shared_ptr<Expr> &expression)
      : expression(std::move(expression)) {}

  std::any accept(StmtVisitor &visitor) override {
    return visitor.visitPrintStmt(shared_from_this());
  }

  std::shared_ptr<Expr> getExpression() const { return expression; }
};

/** @class Var
 * @brief Represents a variable declaration statement.
 *
 * @note This class holds the variable name and its initializer expression,
 * allowing visiting it.
 */
class Var : public Stmt, public std::enable_shared_from_this<Var> {
private:
  const Token name;
  std::shared_ptr<Expr> initializer;

public:
  Var(const Token &name, const std::shared_ptr<Expr> &initializer)
      : name(std::move(name)), initializer(std::move(initializer)) {}

  std::any accept(StmtVisitor &visitor) override {
    return visitor.visitVarStmt(shared_from_this());
  }

  Token getName() const { return name; }

  std::shared_ptr<Expr> getInitializer() const { return initializer; }
};

/** @class If
 * @brief Represents an if statement.
 *
 * @note This class holds the condition expression, the then branch, and the
 * else branch, allowing visiting it.
 */
class If : public Stmt, public std::enable_shared_from_this<If> {
private:
  const std::shared_ptr<Expr> condition;
  const std::shared_ptr<Stmt> thenBranch;
  const std::shared_ptr<Stmt> elseBranch;

public:
  If(const std::shared_ptr<Expr> &condition,
     const std::shared_ptr<Stmt> &thenBranch,
     const std::shared_ptr<Stmt> &elseBranch)
      : condition(std::move(condition)), thenBranch(std::move(thenBranch)),
        elseBranch(std::move(elseBranch)) {}

  std::any accept(StmtVisitor &visitor) override {
    return visitor.visitIfStmt(shared_from_this());
  }

  std::shared_ptr<Expr> getCondition() const { return condition; }

  std::shared_ptr<Stmt> getThenBranch() const { return thenBranch; }

  std::shared_ptr<Stmt> getElseBranch() const { return elseBranch; }
};

class While : public Stmt, public std::enable_shared_from_this<While> {
private:
  const std::shared_ptr<Expr> condition;
  const std::shared_ptr<Stmt> body;

public:
  While(const std::shared_ptr<Expr> &condition,
        const std::shared_ptr<Stmt> &body)
      : condition(std::move(condition)), body(std::move(body)) {}

  std::any accept(StmtVisitor &visitor) override {
    return visitor.visitWhileStmt(shared_from_this());
  }

  std::shared_ptr<Expr> getCondition() const { return condition; }

  std::shared_ptr<Stmt> getBody() const { return body; }
};
