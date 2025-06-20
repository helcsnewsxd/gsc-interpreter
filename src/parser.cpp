#include "gsc/parser.hpp"
#include "gsc/error.hpp"
#include <cassert>

Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens) {}

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
  std::vector<std::shared_ptr<Stmt>> statements;
  while (!isAtEnd()) {
    statements.push_back(declaration());
  }

  return statements;
}

std::shared_ptr<Stmt> Parser::declaration() {
  try {
    if (match(TokenType::VAR))
      return varDeclaration();
    return statement();
  } catch (ParseError &error) {
    synchronize();
    return nullptr; // Return null on error
  }
}

std::shared_ptr<Stmt> Parser::statement() {
  if (match(TokenType::PRINT))
    return printStatement();
  else if (match(TokenType::IF))
    return ifStatement();
  else if (match(TokenType::WHILE))
    return whileStatement();
  else if (match(TokenType::LEFT_BRACE))
    return std::make_shared<Block>(block());
  else
    return expressionStatement();
}

std::shared_ptr<Stmt> Parser::printStatement() {
  std::shared_ptr<Expr> value = expression();
  consume(TokenType::SEMICOLON, "Expect ';' after value.");
  return std::make_shared<Print>(value);
}

std::shared_ptr<Stmt> Parser::ifStatement() {
  consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
  std::shared_ptr<Expr> condition = expression();
  consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");

  std::shared_ptr<Stmt> thenBranch = statement();
  std::shared_ptr<Stmt> elseBranch =
      match(TokenType::ELSE) ? statement() : nullptr;

  return std::make_shared<If>(condition, thenBranch, elseBranch);
}

std::shared_ptr<Stmt> Parser::whileStatement() {
  consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
  std::shared_ptr<Expr> condition = expression();
  consume(TokenType::RIGHT_PAREN, "Expect ')' after while condition.");

  std::shared_ptr<Stmt> body = statement();
  return std::make_shared<While>(condition, body);
}

std::shared_ptr<Stmt> Parser::varDeclaration() {
  Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
  std::shared_ptr<Expr> initializer = nullptr;

  if (match(TokenType::EQUAL)) {
    initializer = expression();
  }

  consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
  return std::make_shared<Var>(std::move(name), initializer);
}

std::shared_ptr<Stmt> Parser::expressionStatement() {
  std::shared_ptr<Expr> expr = expression();
  consume(TokenType::SEMICOLON, "Expect ';' after expression.");
  return std::make_shared<Expression>(expr);
}

std::vector<std::shared_ptr<Stmt>> Parser::block() {
  std::vector<std::shared_ptr<Stmt>> statements;

  while (!isAtEnd() && !check(TokenType::RIGHT_BRACE)) {
    statements.push_back(declaration());
  }

  consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
  return statements;
}

std::shared_ptr<Expr> Parser::assignment() {
  std::shared_ptr<Expr> expr = orLogical();

  if (match(TokenType::EQUAL)) {
    Token equals = previous();
    std::shared_ptr<Expr> value = assignment();

    if (std::shared_ptr<Variable> var =
            std::dynamic_pointer_cast<Variable>(expr)) {
      return std::make_shared<Assign>(var->getName(), value);
    }

    throw error(equals, "Invalid assignment target.");
  }

  return expr;
}

std::shared_ptr<Expr> Parser::orLogical() {
  std::shared_ptr<Expr> expr = andLogical();

  while (match(TokenType::OR)) {
    Token operatorToken = previous();
    std::shared_ptr<Expr> right = andLogical();
    expr = std::make_shared<Logical>(expr, std::move(operatorToken), right);
  }

  return expr;
}

std::shared_ptr<Expr> Parser::andLogical() {
  std::shared_ptr<Expr> expr = equality();

  while (match(TokenType::AND)) {
    Token operatorToken = previous();
    std::shared_ptr<Expr> right = equality();
    expr = std::make_shared<Logical>(expr, std::move(operatorToken), right);
  }

  return expr;
}

std::shared_ptr<Expr> Parser::expression() { return assignment(); }

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
  } else if (match(TokenType::IDENTIFIER)) {
    return std::make_shared<Variable>(previous());
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
