#include "gsc/environment.hpp"
#include "gsc/runtimeError.hpp"

Environment::Environment() : enclosing(nullptr) {}

Environment::Environment(std::shared_ptr<Environment> enclosing)
    : enclosing(std::move(enclosing)) {}

std::any Environment::get(const Token &name) const {
  auto it = values.find(name.getLexeme());
  if (it != values.end()) {
    return it->second;
  }
  if (enclosing) {
    return enclosing->get(name);
  }
  throw RuntimeError(std::make_shared<Token>(name),
                     "Undefined variable '" + name.getLexeme() + "'.");
}

void Environment::assign(const Token &name, std::any value) {
  auto it = values.find(name.getLexeme());
  if (it != values.end()) {
    it->second = std::move(value);
    return;
  }
  if (enclosing) {
    enclosing->assign(name, std::move(value));
    return;
  }
  throw RuntimeError(std::make_shared<Token>(name),
                     "Undefined variable '" + name.getLexeme() + "'.");
}

void Environment::define(const std::string &name, std::any value) {
  values[name] = std::move(value);
}
