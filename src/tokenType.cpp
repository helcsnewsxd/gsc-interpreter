#include "gsc/tokenType.hpp"

std::string toString(TokenType tokenType) {
  return tokenStrings[static_cast<int>(tokenType)];
}
