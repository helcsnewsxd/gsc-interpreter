#pragma once

#include "gsc/token.hpp"
#include <memory>
#include <stdexcept>

/** @class RuntimeError
 * @brief Represents a runtime error in the GSC interpreter.
 *
 * @note This class extends std::runtime_error to provide additional context
 * about the error, including the token that caused it and a message describing
 * the error.
 */
class RuntimeError : public std::runtime_error {
private:
  std::shared_ptr<Token> token;

public:
  /** @brief Constructs a RuntimeError with a token and a message.
   *
   * @param token A shared pointer to the Token that caused the error.
   * @param message A string view containing the error message.
   */
  RuntimeError(std::shared_ptr<Token> token, std::string_view message)
      : std::runtime_error(message.data()), token(token) {}

  std::shared_ptr<Token> getToken() const { return token; }

  std::string getMessage() const { return what(); }
};
