#include "gsc/environment.hpp"
#include "catch2/catch_amalgamated.hpp"
#include "gsc/runtimeError.hpp"

TEST_CASE("Environment without enclosing", "[environment]") {
  Environment env{};

  SECTION("Get undefined variable") {
    Token name(TokenType::IDENTIFIER, "x", "x", 1);
    REQUIRE_THROWS_AS(env.get(name), RuntimeError);
  }

  SECTION("Define and get variable") {
    Token name(TokenType::IDENTIFIER, "x", "x", 1);
    env.define(name.getLexeme(), 42);
    REQUIRE(env.get(name).type() == typeid(int));
    CHECK(std::any_cast<int>(env.get(name)) == 42);
  }

  SECTION("Assign to undefined variable") {
    Token name(TokenType::IDENTIFIER, "x", "x", 1);
    REQUIRE_THROWS_AS(env.assign(name, 42), RuntimeError);
  }

  SECTION("Assign to defined variable") {
    Token name(TokenType::IDENTIFIER, "x", "x", 1);
    env.define(name.getLexeme(), 42);
    env.assign(name, 100);
    REQUIRE(env.get(name).type() == typeid(int));
    CHECK(std::any_cast<int>(env.get(name)) == 100);
  }
}

TEST_CASE("Environment with enclosing", "[environment][enclosing]") {
  Environment auxEnv{};
  auxEnv.define("auxVar", 10);
  auxEnv.define("auxVar2", 20);
  Environment env{std::make_shared<Environment>(auxEnv)};

  SECTION("Get variable from enclosing environment") {
    Token name(TokenType::IDENTIFIER, "auxVar", "auxVar", 1);
    REQUIRE(env.get(name).type() == typeid(int));
    CHECK(std::any_cast<int>(env.get(name)) == 10);
  }

  SECTION("Define variable in environment") {
    Token name(TokenType::IDENTIFIER, "newVar", "newVar", 1);
    env.define(name.getLexeme(), 20);
    REQUIRE(env.get(name).type() == typeid(int));
    CHECK(std::any_cast<int>(env.get(name)) == 20);
  }

  SECTION("Assign to variable in enclosing environment") {
    Token name(TokenType::IDENTIFIER, "auxVar2", "auxVar2", 1);
    env.assign(name, 30);
    REQUIRE(env.get(name).type() == typeid(int));
    CHECK(std::any_cast<int>(env.get(name)) == 30);
  }

  SECTION("Get undefined variable in enclosing environment") {
    Token name(TokenType::IDENTIFIER, "undefinedVar", "undefinedVar", 1);
    REQUIRE_THROWS_AS(env.get(name), RuntimeError);
  }

  SECTION("Assign to undefined variable in enclosing environment") {
    Token name(TokenType::IDENTIFIER, "undefinedVar", "undefinedVar", 1);
    REQUIRE_THROWS_AS(env.assign(name, 42), RuntimeError);
  }
}
