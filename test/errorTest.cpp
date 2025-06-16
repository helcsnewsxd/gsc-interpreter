#include "gsc/error.hpp"
#include "catch2/catch_amalgamated.hpp"
#include "gsc/token.hpp"
#include <iostream>
#include <sstream>

TEST_CASE("Error handling", "[error]") {
  // Hide the error output
  std::ostringstream oss;
  auto oldCerr = std::cerr.rdbuf(oss.rdbuf());

  SECTION("Error reporting") {
    hadError = false;
    error(1, "Test error");
    CHECK(hadError == true);
  }

  SECTION("Report function") {
    hadError = false;
    report(1, "Test location", "Test error");
    CHECK(hadError == true);
  }

  SECTION("Token error reporting") {
    hadError = false;
    Token token(TokenType::NUMBER, "2", 2, 1);
    error(token, "Test token error");
    CHECK(hadError == true);
  }

  SECTION("Runtime error reporting") {
    hadRuntimeError = false;
    Token token(TokenType::NUMBER, "2", 2, 1);
    RuntimeError runErr(std::make_shared<Token>(token), "Test runtime error");
    runtimeError(runErr);
    CHECK(hadRuntimeError == true);
  }

  // Restore the original cerr buffer
  std::cerr.rdbuf(oldCerr);
}
