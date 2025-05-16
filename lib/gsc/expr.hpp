#pragma once

#include "token.hpp"
#include <any>
#include <memory>

class Binary;
class Grouping;
class Literal;
class Unary;

class Visitor {
public:
  virtual std::any visitBinaryExpr(std::shared_ptr<Binary> expr) = 0;
  virtual std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) = 0;
  virtual std::any visitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
  virtual std::any visitUnaryExpr(std::shared_ptr<Unary> expr) = 0;

  virtual ~Visitor() = default;
};

class Expr {
  virtual std::any accept(Visitor &visitor) = 0;
};

class Binary : public Expr, public std::enable_shared_from_this<Binary> {
private:
  const std::shared_ptr<Expr> left;
  const Token op;
  const std::shared_ptr<Expr> right;

public:
  Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
      : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitBinaryExpr(shared_from_this());
  }

  std::shared_ptr<Expr> getLeft() const { return left; }
  std::shared_ptr<Expr> getRight() const { return right; }
  Token getOp() const { return op; }
};

class Grouping : public Expr, public std::enable_shared_from_this<Grouping> {
private:
  const std::shared_ptr<Expr> expression;

public:
  Grouping(std::shared_ptr<Expr> expression)
      : expression(std::move(expression)) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitGroupingExpr(shared_from_this());
  }

  std::shared_ptr<Expr> getExpression() const { return expression; }
};

class Literal : public Expr, public std::enable_shared_from_this<Literal> {
private:
  const std::any value;

public:
  Literal(std::any value) : value(std::move(value)) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitLiteralExpr(shared_from_this());
  }

  std::any getValue() const { return value; }
};

class Unary : public Expr, public std::enable_shared_from_this<Unary> {
private:
  const Token op;
  const std::shared_ptr<Expr> right;

public:
  Unary(Token op, std::shared_ptr<Expr> right)
      : op(std::move(op)), right(std::move(right)) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitUnaryExpr(shared_from_this());
  }

  std::shared_ptr<Expr> getRight() const { return right; }
  Token getOp() const { return op; }
};
