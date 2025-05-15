#include "gsc/error.hpp"
#include "catch2/catch_amalgamated.hpp"
#include <iostream>
#include <sstream>

TEST_CASE("Error handling", "[error]") {
  // Hide the error output
  std::ostringstream oss;
  auto oldCerr = std::cerr.rdbuf(oss.rdbuf());

  SECTION("Error reporting") {
    hadError = false;
    error(1, "Test error");
    REQUIRE(hadError == true);
  }

  SECTION("Report function") {
    hadError = false;
    report(1, "Test location", "Test error");
    REQUIRE(hadError == true);
  }

  // Restore the original cerr buffer
  std::cerr.rdbuf(oldCerr);
}
