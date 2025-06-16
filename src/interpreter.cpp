#include "gsc/interpreter.hpp"
#include "gsc/error.hpp"
#include <cassert>
#include <iostream>

void Interpreter::interpret(
    const std::vector<std::shared_ptr<Stmt>> &statements) {
  try {
    for (const std::shared_ptr<Stmt> &stmt : statements) {
      execute(stmt);
    }
  } catch (RuntimeError &error) {
    runtimeError(error);
  }
}

std::any Interpreter::evaluate(std::shared_ptr<Expr> expr) {
  return expr->accept(*this);
}

void Interpreter::execute(std::shared_ptr<Stmt> stmt) { stmt->accept(*this); }

void Interpreter::executeBlock(
    const std::vector<std::shared_ptr<Stmt>> statements,
    std::shared_ptr<Environment> environment) {
  std::shared_ptr<Environment> previous = this->environment;
  try {
    this->environment = environment;

    for (const std::shared_ptr<Stmt> &stmt : statements) {
      execute(stmt);
    }
  } catch (...) {
    this->environment = previous;
    throw; // Re-throw the exception to be handled by the caller
  }

  this->environment = previous;
}

template <class... N>
void Interpreter::checkNumberOperands(const Token &op, N... operands) {
  if (((operands.type() != typeid(int)) && ...)) {
    throw RuntimeError(std::make_shared<Token>(op),
                       "Operands must be numbers.");
  }
}

bool Interpreter::isTruthy(const std::any &value) const {
  if (value.type() == typeid(std::nullptr_t)) {
    return false;
  } else if (value.type() == typeid(int)) {
    return std::any_cast<int>(value) != 0; // Non-zero integers are truthy
  } else if (value.type() == typeid(std::string)) {
    return !std::any_cast<std::string>(value)
                .empty(); // Non-empty strings are truthy
  } else if (value.type() == typeid(bool)) {
    return std::any_cast<bool>(value);
  }
  return true; // All other values are truthy
}

bool Interpreter::isEqual(const std::any &a, const std::any &b) const {
  if (a.type() != b.type()) {
    return false;
  } else if (a.type() == typeid(std::nullptr_t)) {
    return true; // Both are nil
  } else if (a.type() == typeid(bool)) {
    return std::any_cast<bool>(a) == std::any_cast<bool>(b);
  } else if (a.type() == typeid(int)) {
    return std::any_cast<int>(a) == std::any_cast<int>(b);
  } else if (a.type() == typeid(std::string)) {
    return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
  } else {
    return false; // Unsupported types
  }
}

std::string Interpreter::stringify(const std::any &value) const {
  if (value.type() == typeid(std::nullptr_t)) {
    return "nil";
  } else if (value.type() == typeid(bool)) {
    return std::any_cast<bool>(value) ? "true" : "false";
  } else if (value.type() == typeid(int)) {
    return std::to_string(std::any_cast<int>(value));
  } else if (value.type() == typeid(std::string)) {
    return std::any_cast<std::string>(value);
  } else if (value.type() == typeid(const char *)) {
    return std::any_cast<const char *>(value);
  }
  return "Internal error (interpreter): object type not recognized"; // Unsupported
                                                                     // types
}

std::any Interpreter::visitGroupingExpr(std::shared_ptr<Grouping> expr) {
  return evaluate(expr->getExpression());
}

std::any Interpreter::visitLiteralExpr(std::shared_ptr<Literal> expr) {
  return expr->getValue();
}

std::any Interpreter::visitUnaryExpr(std::shared_ptr<Unary> expr) {
  std::any right = evaluate(expr->getRight());
  Token op = expr->getOp();

  switch (op.getType()) {
  case TokenType::MINUS:
    checkNumberOperands(op, right);
    return -std::any_cast<int>(right);
  case TokenType::BANG:
    return !isTruthy(right);
  default:
    // This should never be reached, but just in case
    assert(false && "Unknown unary operator");
    return {};
  }
}

std::any Interpreter::visitBinaryExpr(std::shared_ptr<Binary> expr) {
  std::any left = evaluate(expr->getLeft());
  std::any right = evaluate(expr->getRight());
  Token op = expr->getOp();

  switch (op.getType()) {
  case TokenType::PLUS:
    if (left.type() == typeid(int) && right.type() == typeid(int)) {
      return std::any_cast<int>(left) + std::any_cast<int>(right);
    } else if (left.type() == typeid(std::string) &&
               right.type() == typeid(std::string)) {
      return std::any_cast<std::string>(left) +
             std::any_cast<std::string>(right);
    } else {
      throw RuntimeError(std::make_shared<Token>(op),
                         "Operands must be two numbers or two strings.");
    }
  case TokenType::MINUS:
    checkNumberOperands(op, left, right);
    return std::any_cast<int>(left) - std::any_cast<int>(right);
  case TokenType::STAR:
    checkNumberOperands(op, left, right);
    return std::any_cast<int>(left) * std::any_cast<int>(right);
  case TokenType::SLASH:
    checkNumberOperands(op, left, right);
    if (std::any_cast<int>(right) == 0) {
      throw RuntimeError(std::make_shared<Token>(op), "Division by zero.");
    }
    return std::any_cast<int>(left) / std::any_cast<int>(right);
  case TokenType::GREATER:
    checkNumberOperands(op, left, right);
    return std::any_cast<int>(left) > std::any_cast<int>(right);
  case TokenType::GREATER_EQUAL:
    checkNumberOperands(op, left, right);
    return std::any_cast<int>(left) >= std::any_cast<int>(right);
  case TokenType::LESS:
    checkNumberOperands(op, left, right);
    return std::any_cast<int>(left) < std::any_cast<int>(right);
  case TokenType::LESS_EQUAL:
    checkNumberOperands(op, left, right);
    return std::any_cast<int>(left) <= std::any_cast<int>(right);
  case TokenType::EQUAL_EQUAL:
    return isEqual(left, right);
  case TokenType::BANG_EQUAL:
    return !isEqual(left, right);
  default:
    // This should never be reached, but just in case
    assert(false && "Unknown binary operator");
    return {};
  }
}

std::any Interpreter::visitAssignExpr(std::shared_ptr<Assign> expr) {
  std::any value = evaluate(expr->getValue());
  environment->assign(expr->getName(), value);
  return value;
}

std::any Interpreter::visitVariableExpr(std::shared_ptr<Variable> expr) {
  return environment->get(expr->getName());
}

std::any Interpreter::visitBlockStmt(std::shared_ptr<Block> stmt) {
  executeBlock(stmt->getStatements(),
               std::make_shared<Environment>(environment));
  return {};
}

std::any Interpreter::visitExpressionStmt(std::shared_ptr<Expression> stmt) {
  evaluate(stmt->getExpression());
  return {};
}

std::any Interpreter::visitPrintStmt(std::shared_ptr<Print> stmt) {
  std::any value = evaluate(stmt->getExpression());
  std::cout << stringify(value) << std::endl;
  return {};
}

std::any Interpreter::visitVarStmt(std::shared_ptr<Var> stmt) {
  std::any value = nullptr;
  if (stmt->getInitializer()) {
    value = evaluate(stmt->getInitializer());
  }
  environment->define(stmt->getName().getLexeme(), std::move(value));
  return {};
}
