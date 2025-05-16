#pragma once

#include "gsc/expr.hpp"

/** @class AstPrinter
 * @brief Class for printing the abstract syntax tree (AST).
 *
 * This class implements the ExprVisitor interface to traverse and print
 * different types of expressions in the AST. It provides a method to print
 * expressions in a parenthesized format.
 */
class AstPrinter : ExprVisitor {
private:
  template <class... E>
  std::string parenthesize(std::string_view name, E... exprs);

  std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override;
  std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override;
  std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override;
  std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override;

public:
  AstPrinter() = default;
  ~AstPrinter() override = default;

  /** @brief Prints the expression in a parenthesized format.
   *
   * This method takes an expression and returns its string representation in
   * a parenthesized format.
   *
   * @param expr The expression to print.
   * @return The string representation of the expression.
   *
   * @throws std::invalid_argument if there are sub-expressions that are not of
   * expected type.
   * @throws std::runtime_error if the literal type is unknown.
   */
  std::string print(std::shared_ptr<Expr> expr);
};
