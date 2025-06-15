#pragma once

#include "gsc/expr.hpp"
#include "gsc/token.hpp"
#include <memory>
#include <stdexcept>
#include <vector>

class Parser {
private:
  struct ParseError : public std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  const std::vector<Token> &tokens;
  int current = 0;

  std::shared_ptr<Expr> expression();
  std::shared_ptr<Expr> equality();
  std::shared_ptr<Expr> comparison();
  std::shared_ptr<Expr> term();
  std::shared_ptr<Expr> factor();
  std::shared_ptr<Expr> unary();
  std::shared_ptr<Expr> primary();

  template <class... T> bool match(T... types);
  bool check(TokenType type) const;
  bool isAtEnd() const;

  Token consume(TokenType type, std::string_view message);
  Token advance();
  Token peek() const;
  Token previous() const;

  ParseError error(const Token &token, std::string_view message);

  void synchronize();

public:
  Parser(const std::vector<Token> &tokens);
  ~Parser() = default;

  std::shared_ptr<Expr> parse();
};
