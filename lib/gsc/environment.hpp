#pragma once

#include "gsc/token.hpp"
#include <any>
#include <map>
#include <memory>
#include <string>

/** @class Environment
 * @brief Represents an environment for variable storage in the GSC interpreter.
 *
 * @note This class manages variable scopes, allowing for nested environments
 * to handle variable declarations and lookups.
 */
class Environment : public std::enable_shared_from_this<Environment> {
private:
  std::shared_ptr<Environment> enclosing;
  std::map<std::string, std::any> values;

public:
  /** @brief Constructs a new Environment.
   *
   * @note This constructor creates a new environment without an enclosing
   * environment.
   */
  Environment();

  /** @brief Constructs a new Environment with an enclosing environment.
   *
   * @param enclosing A shared pointer to the enclosing environment.
   *
   * @note This constructor allows for nested environments, enabling variable
   * shadowing and scope management.
   */
  Environment(std::shared_ptr<Environment> enclosing);

  /** @brief Retrieves the value of a variable by its name.
   *
   * @param name The Token representing the variable name.
   * @return The value associated with the variable.
   *
   * @throws RuntimeError if the variable is not defined in this or any
   * enclosing environment.
   */
  std::any get(const Token &name) const;

  /** @brief Assigns a value to a variable by its name.
   *
   * @param name The Token representing the variable name.
   * @param value The value to assign to the variable.
   *
   * @throws RuntimeError if the variable is not defined in this or any
   * enclosing environment.
   */
  void assign(const Token &name, std::any value);

  /** @brief Defines a new variable in the current environment.
   *
   * @param name The name of the variable to define.
   * @param value The value to assign to the variable.
   *
   * @note This method does not check for existing variables with the same name,
   * allowing for redefinition within the same environment.
   */
  void define(const std::string &name, std::any value);
};
