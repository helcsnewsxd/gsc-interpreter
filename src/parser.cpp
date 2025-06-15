#include "gsc/parser.hpp"
#include "gsc/error.hpp"
#include <cassert>

Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens) {}

std::shared_ptr<Expr> Parser::parse() {
  try {
    return expression();
  } catch (const ParseError &e) {
    return nullptr;
  }
}

std::shared_ptr<Expr> Parser::expression() { return equality(); }

std::shared_ptr<Expr> Parser::equality() {
  std::shared_ptr<Expr> expr = comparison();

  while (match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
    Token operatorToken = previous();
    std::shared_ptr<Expr> right = comparison();
    expr = std::make_shared<Binary>(expr, std::move(operatorToken), right);
  }

  return expr;
}

std::shared_ptr<Expr> Parser::comparison() {
  std::shared_ptr<Expr> expr = term();

  while (match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS,
               TokenType::LESS_EQUAL)) {
    Token operatorToken = previous();
    std::shared_ptr<Expr> right = term();
    expr = std::make_shared<Binary>(expr, std::move(operatorToken), right);
  }

  return expr;
}

std::shared_ptr<Expr> Parser::term() {
  std::shared_ptr<Expr> expr = factor();

  while (match(TokenType::MINUS, TokenType::PLUS)) {
    Token operatorToken = previous();
    std::shared_ptr<Expr> right = factor();
    expr = std::make_shared<Binary>(expr, std::move(operatorToken), right);
  }

  return expr;
}

std::shared_ptr<Expr> Parser::factor() {
  std::shared_ptr<Expr> expr = unary();

  while (match(TokenType::SLASH, TokenType::STAR)) {
    Token operatorToken = previous();
    std::shared_ptr<Expr> right = unary();
    expr = std::make_shared<Binary>(expr, std::move(operatorToken), right);
  }

  return expr;
}

std::shared_ptr<Expr> Parser::unary() {
  if (match(TokenType::BANG, TokenType::MINUS)) {
    Token operatorToken = previous();
    std::shared_ptr<Expr> right = unary();
    return std::make_shared<Unary>(std::move(operatorToken), right);
  }

  return primary();
}

std::shared_ptr<Expr> Parser::primary() {
  if (match(TokenType::NIL))
    return std::make_shared<Literal>(nullptr);
  else if (match(TokenType::TRUE))
    return std::make_shared<Literal>(true);
  else if (match(TokenType::FALSE))
    return std::make_shared<Literal>(false);
  else if (match(TokenType::NUMBER, TokenType::STRING)) {
    return std::make_shared<Literal>(previous().getLiteral());
  } else if (match(TokenType::LEFT_PAREN)) {
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
    return std::make_shared<Grouping>(std::move(expr));
  } else {
    std::string_view message = "Expect expression.";
    throw error(peek(), message);
  }
}

template <class... T> bool Parser::match(T... types) {
  assert((std::is_same_v<T, TokenType> && ...));

  for (TokenType type : {types...}) {
    if (check(type)) {
      advance();
      return true;
    }
  }
  return false;
}

bool Parser::check(TokenType type) const {
  if (isAtEnd())
    return false;
  return peek().getType() == type;
}

bool Parser::isAtEnd() const {
  return peek().getType() == TokenType::END_OF_FILE;
}

Token Parser::consume(TokenType type, std::string_view message) {
  if (check(type))
    return advance();

  throw error(peek(), message);
}

Token Parser::advance() {
  if (!isAtEnd())
    current++;
  return previous();
}

Token Parser::peek() const { return tokens[current]; }

Token Parser::previous() const {
  assert(current > 0);
  return tokens[current - 1];
}

Parser::ParseError Parser::error(const Token &token, std::string_view message) {
  ::error(token, std::string(message));
  return ParseError("");
}

void Parser::synchronize() {
  advance();

  while (!isAtEnd()) {
    if (previous().getType() == TokenType::SEMICOLON)
      return;

    switch (peek().getType()) {
    case TokenType::VAR:
    case TokenType::FOR:
    case TokenType::IF:
    case TokenType::WHILE:
    case TokenType::PRINT:
      return;
    default:
      break;
    }

    advance();
  }
}
