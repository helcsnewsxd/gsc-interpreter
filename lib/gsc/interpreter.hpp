#pragma once

#include "gsc/environment.hpp"
#include "gsc/expr.hpp"
#include "gsc/stmt.hpp"
#include <vector>

/** @class Interpreter
 * @brief The Interpreter class evaluates statements and expressions in the GSC
 *
 * @note It implements the ExprVisitor interface to visit different types of
 * expressions and statements, and evaluate them accordingly. The results of the
 * evaluations are returned as std::any types, allowing for flexibility in the
 * types of values that can be returned.
 */
class Interpreter : public ExprVisitor, public StmtVisitor {
private:
  std::shared_ptr<Environment> environment{new Environment};

  std::any evaluate(std::shared_ptr<Expr> expr);
  void execute(std::shared_ptr<Stmt> stmt);

  void executeBlock(const std::vector<std::shared_ptr<Stmt>> statements,
                    std::shared_ptr<Environment> environment);

  std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override;
  std::any visitLogicalExpr(std::shared_ptr<Logical> expr) override;
  std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override;
  std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override;
  std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override;
  std::any visitAssignExpr(std::shared_ptr<Assign> expr) override;
  std::any visitVariableExpr(std::shared_ptr<Variable> expr) override;
  std::any visitBlockStmt(std::shared_ptr<Block> stmt) override;
  std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) override;
  std::any visitPrintStmt(std::shared_ptr<Print> stmt) override;
  std::any visitIfStmt(std::shared_ptr<If> stmt) override;
  std::any visitWhileStmt(std::shared_ptr<While> stmt) override;
  std::any visitVarStmt(std::shared_ptr<Var> stmt) override;

  template <class... N>
  void checkNumberOperands(const Token &op, N... operands);

  bool isTruthy(const std::any &value) const;
  bool isEqual(const std::any &a, const std::any &b) const;

  std::string stringify(const std::any &value) const;

public:
  /** @brief
   * Interpret the given statement list and execute them
   *
   * @param statements A vector of shared pointers to Stmt objects representing
   *
   * @note This method evaluates the provided statements in the context of the
   * GSC interpreter.
   * @note If an error occurs during interpretation, it will shown an error
   * message to the user and set the `hadRuntimeError` variable to true.
   */
  void interpret(const std::vector<std::shared_ptr<Stmt>> &statements);
};
