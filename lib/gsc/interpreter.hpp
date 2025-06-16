#pragma once

#include "gsc/expr.hpp"

/** @class Interpreter
 * @brief The Interpreter class evaluates expressions in the GSC language.
 *
 * @note It implements the ExprVisitor interface to visit different types of
 * expressions and evaluate them accordingly. The results of the evaluations are
 * returned as std::any types, allowing for flexibility in the types of values
 * that can be returned.
 */
class Interpreter : public ExprVisitor {
private:
  std::any evaluate(std::shared_ptr<Expr> expr);

  std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override;
  std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override;
  std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override;
  std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override;

  template <class... N>
  void checkNumberOperands(const Token &op, N... operands);

  bool isTruthy(const std::any &value) const;
  bool isEqual(const std::any &a, const std::any &b) const;

  std::string stringify(const std::any &value) const;

public:
  /** @brief
   * Interpret the given expression.
   *
   * @param expr The expression to interpret.
   *
   * @note This method evaluates the expression and prints the result to
   * stdout. If an error occurs during evaluation, it will print a runtime
   * error and set the global variable `hadRuntimeError` to true.
   */
  void interpret(std::shared_ptr<Expr> expr);
};
