#include "gsc/error.hpp"
#include "catch2/catch_amalgamated.hpp"

TEST_CASE("Error handling", "[error]") {
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
}
