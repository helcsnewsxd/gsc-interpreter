#pragma once

#include "token.hpp"
#include <any>
#include <memory>

class Binary;
class Grouping;
class Literal;
class Unary;
class Assign;
class Variable;
class Logical;

/** @class ExprVisitor
 * @brief Abstract base class for expression visitors.
 *
 * This class defines the interface for visitors that can traverse and process
 * different types of expressions in an abstract syntax tree (AST).
 */
class ExprVisitor {
public:
  virtual std::any visitBinaryExpr(std::shared_ptr<Binary> expr) = 0;
  virtual std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) = 0;
  virtual std::any visitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
  virtual std::any visitUnaryExpr(std::shared_ptr<Unary> expr) = 0;
  virtual std::any visitAssignExpr(std::shared_ptr<Assign> expr) = 0;
  virtual std::any visitVariableExpr(std::shared_ptr<Variable> expr) = 0;
  virtual std::any visitLogicalExpr(std::shared_ptr<Logical> expr) = 0;

  virtual ~ExprVisitor() = default;
};

/** @class Expr
 * @brief Abstract base class for expressions.
 *
 * This class serves as the base class for all expression types in the AST.
 * It defines a method for accepting visitors that implement the ExprVisitor
 * interface.
 */
class Expr {
public:
  virtual std::any accept(ExprVisitor &visitor) = 0;
};

/** @class Binary
 * @brief Class representing a binary expression.
 *
 * This class represents a binary expression in the AST, which consists of a
 * left operand, an operator, and a right operand. It inherits from the Expr
 * class and implements the accept method for visitor pattern.
 */
class Binary : public Expr, public std::enable_shared_from_this<Binary> {
private:
  const std::shared_ptr<Expr> left;
  const Token op;
  const std::shared_ptr<Expr> right;

public:
  Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
      : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

  std::any accept(ExprVisitor &visitor) override {
    return visitor.visitBinaryExpr(shared_from_this());
  }

  std::shared_ptr<Expr> getLeft() const { return left; }
  std::shared_ptr<Expr> getRight() const { return right; }
  Token getOp() const { return op; }
};

/** @class Grouping
 * @brief Class representing a grouping expression.
 *
 * This class represents a grouping expression in the AST, which is used to
 * group sub-expressions. It inherits from the Expr class and implements the
 * accept method for visitor pattern.
 */
class Grouping : public Expr, public std::enable_shared_from_this<Grouping> {
private:
  const std::shared_ptr<Expr> expression;

public:
  Grouping(std::shared_ptr<Expr> expression)
      : expression(std::move(expression)) {}

  std::any accept(ExprVisitor &visitor) override {
    return visitor.visitGroupingExpr(shared_from_this());
  }

  std::shared_ptr<Expr> getExpression() const { return expression; }
};

/** @class Literal
 * @brief Class representing a literal expression.
 *
 * This class represents a literal expression in the AST, which can hold a
 * value of any type. It inherits from the Expr class and implements the accept
 * method for visitor pattern.
 */
class Literal : public Expr, public std::enable_shared_from_this<Literal> {
private:
  const std::any value;

public:
  Literal(std::any value) : value(std::move(value)) {}

  std::any accept(ExprVisitor &visitor) override {
    return visitor.visitLiteralExpr(shared_from_this());
  }

  std::any getValue() const { return value; }
};

/** @class Unary
 * @brief Class representing a unary expression.
 *
 * This class represents a unary expression in the AST, which consists of an
 * operator and a right operand. It inherits from the Expr class and implements
 * the accept method for visitor pattern.
 */
class Unary : public Expr, public std::enable_shared_from_this<Unary> {
private:
  const Token op;
  const std::shared_ptr<Expr> right;

public:
  Unary(Token op, std::shared_ptr<Expr> right)
      : op(std::move(op)), right(std::move(right)) {}

  std::any accept(ExprVisitor &visitor) override {
    return visitor.visitUnaryExpr(shared_from_this());
  }

  std::shared_ptr<Expr> getRight() const { return right; }
  Token getOp() const { return op; }
};

/** @brief Class representing an assignment expression.
 *
 * This class represents an assignment expression in the AST, which consists
 * of a variable name and a value to assign to it. It inherits from the Expr
 * class and implements the accept method for visitor pattern.
 */
class Assign : public Expr, public std::enable_shared_from_this<Assign> {
private:
  const Token name;
  const std::shared_ptr<Expr> value;

public:
  Assign(Token name, std::shared_ptr<Expr> value)
      : name(std::move(name)), value(std::move(value)) {}

  std::any accept(ExprVisitor &visitor) override {
    return visitor.visitAssignExpr(shared_from_this());
  }

  Token getName() const { return name; }

  std::shared_ptr<Expr> getValue() const { return value; }
};

/** @class Variable
 * @brief Class representing a variable expression.
 *
 * This class represents a variable expression in the AST, which consists of a
 * variable name. It inherits from the Expr class and implements the accept
 * method for visitor pattern.
 */
class Variable : public Expr, public std::enable_shared_from_this<Variable> {
private:
  const Token name;

public:
  Variable(Token name) : name(std::move(name)) {}

  std::any accept(ExprVisitor &visitor) override {
    return visitor.visitVariableExpr(shared_from_this());
  }

  Token getName() const { return name; }
};

/** @class Logical
 * @brief Class representing a logical expression.
 *
 * This class represents a logical expression in the AST, which consists of a
 * left operand, an operator, and a right operand. It inherits from the Expr
 * class and implements the accept method for visitor pattern.
 */
class Logical : public Expr, public std::enable_shared_from_this<Logical> {
private:
  const std::shared_ptr<Expr> left;
  const Token op;
  const std::shared_ptr<Expr> right;

public:
  Logical(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
      : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

  std::any accept(ExprVisitor &visitor) override {
    return visitor.visitLogicalExpr(shared_from_this());
  }

  std::shared_ptr<Expr> getLeft() const { return left; }

  std::shared_ptr<Expr> getRight() const { return right; }

  Token getOp() const { return op; }
};
