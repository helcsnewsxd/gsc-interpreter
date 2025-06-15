#include "gsc/error.hpp"
#include <iostream>

void report(const int &line, const std::string &where,
            const std::string &message) {
  std::cerr << "[line " << line << "] Error " << where << ": " << message
            << "\n";

  hadError = true;
}

void error(const int &line, const std::string &message) {
  report(line, "", message);
}

void error(const Token &token, const std::string &message) {
  if (token.getType() == TokenType::END_OF_FILE) {
    report(token.getLine(), "at end", message);
  } else {
    report(token.getLine(), "at '" + token.getLexeme() + "'", message);
  }
}
