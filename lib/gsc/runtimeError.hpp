#pragma once

#include "gsc/token.hpp"
#include <memory>
#include <stdexcept>

class RuntimeError : public std::runtime_error {
private:
  std::shared_ptr<Token> token;

public:
  RuntimeError(std::shared_ptr<Token> token, std::string_view message)
      : std::runtime_error(message.data()), token(token) {}

  std::shared_ptr<Token> getToken() const { return token; }

  std::string getMessage() const { return what(); }
};
