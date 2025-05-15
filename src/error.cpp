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
