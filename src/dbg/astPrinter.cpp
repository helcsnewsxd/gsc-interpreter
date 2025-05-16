#include "dbg/astPrinter.hpp"

#include <sstream>

template <class... E>
std::string AstPrinter::parenthesize(std::string_view name, E... exprs) {
  // Check that all expressions are of type std::shared_ptr<Expr>.
  if (!(std::is_same_v<E, std::shared_ptr<Expr>> && ...)) {
    throw std::invalid_argument(
        "All arguments must be of type std::shared_ptr<Expr>");
  }

  // Construct the parenthesized string
  std::ostringstream oss;
  oss << "(" << name;
  ((oss << " " << print(exprs)), ...);
  oss << ")";

  return oss.str();
}

std::any AstPrinter::visitBinaryExpr(std::shared_ptr<Binary> expr) {
  return parenthesize(expr->getOp().getLexeme(), expr->getLeft(),
                      expr->getRight());
}

std::any AstPrinter::visitGroupingExpr(std::shared_ptr<Grouping> expr) {
  return parenthesize("group", expr->getExpression());
}

std::any AstPrinter::visitLiteralExpr(std::shared_ptr<Literal> expr) {
  auto value = expr->getValue();
  auto &type = value.type();
  std::string literal;

  if (type == typeid(nullptr)) {
    literal = "nil";
  } else if (type == typeid(bool)) {
    literal = std::any_cast<bool>(expr->getValue()) ? "true" : "false";
  } else if (type == typeid(int)) {
    literal = std::to_string(std::any_cast<int>(expr->getValue()));
  } else if (type == typeid(std::string)) {
    literal = std::any_cast<std::string>(expr->getValue());
  } else if (type == typeid(const char *)) {
    literal = std::any_cast<const char *>(expr->getValue());
  } else {
    throw std::runtime_error("Unknown literal type");
  }

  return literal;
}

std::any AstPrinter::visitUnaryExpr(std::shared_ptr<Unary> expr) {
  return parenthesize(expr->getOp().getLexeme(), expr->getRight());
}

std::string AstPrinter::print(std::shared_ptr<Expr> expr) {
  return std::any_cast<std::string>(expr->accept(*this));
}
