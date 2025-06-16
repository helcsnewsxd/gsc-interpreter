#pragma once

#include "gsc/runtimeError.hpp"
#include <string>

inline bool hadError = false;
inline bool hadRuntimeError = false;

/** @brief
 * Report an error in the given line and location.
 *
 * @param line The line number where the error occurred.
 * @param where The location of the error.
 * @param message The error message to display.
 *
 * @note This function sets the global variable `hadError` to true and writes in
 * stderr.
 */
void report(const int &line, const std::string &where,
            const std::string &message);

/** @brief
 * Report an error in the given line.
 *
 * @param line The line number where the error occurred.
 * @param message The error message to display.
 *
 * @note This function sets the global variable `hadError` to true and writes in
 * stderr.
 * @see report
 */
void error(const int &line, const std::string &message);

/** @brief
 * Report an error in the given token.
 *
 * @param token The token where the error occurred.
 * @param message The error message to display.
 *
 * @note This function sets the global variable `hadError` to true and writes in
 * stderr.
 * @see report
 *
 */
void error(const Token &token, const std::string &message);

/** @brief
 * Report a runtime error given a RuntimeError object.
 *
 * @param error The RuntimeError object containing the error details.
 *
 * @note This function sets the global variable `hadRuntimeError` to true and
 * writes in stderr.
 */
void runtimeError(const RuntimeError &error);
