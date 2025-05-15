#include "gsc/tokenType.hpp"

std::string toString(const TokenType &tokenType) {
  return tokenStrings[static_cast<int>(tokenType)];
}
