#include "gsc/scanner.hpp"
#include "gsc/error.hpp"

bool isDigit(const char c) { return c >= '0' && c <= '9'; }

bool isAlpha(const char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool isAlphaNumeric(const char c) { return isAlpha(c) || isDigit(c); }

void Scanner::addToken(TokenType type) { addToken(type, nullptr); }

void Scanner::addToken(TokenType type, std::any literal) {
  std::string text{program.substr(start, current - start)};
  tokens.emplace_back(type, std::move(text), std::move(literal), line);
}

void Scanner::scanToken() {
  char c = advance();
  switch (c) {
  // One or two character tokens
  case '(':
    addToken(LEFT_PAREN);
    break;
  case ')':
    addToken(RIGHT_PAREN);
    break;
  case '{':
    addToken(LEFT_BRACE);
    break;
  case '}':
    addToken(RIGHT_BRACE);
    break;
  case '-':
    addToken(MINUS);
    break;
  case '+':
    addToken(PLUS);
    break;
  case ';':
    addToken(SEMICOLON);
    break;
  case '*':
    addToken(STAR);
    break;
  case '!':
    addToken(match('=') ? BANG_EQUAL : BANG);
    break;
  case '=':
    addToken(match('=') ? EQUAL_EQUAL : EQUAL);
    break;
  case '<':
    addToken(match('=') ? LESS_EQUAL : LESS);
    break;
  case '>':
    addToken(match('=') ? GREATER_EQUAL : GREATER);
    break;

  // Integer division vs. C-style comments
  case '/':
    if (match('/')) {
      while (peek() != '\n' && !isAtEnd())
        advance();
    } else {
      addToken(SLASH);
    }
    break;

  // Ignore white spaces
  case ' ':
  case '\r':
  case '\t':
    break;

  // New line
  case '\n':
    line++;
    break;

  // Identifiers
  case '"':
    string();
    break;
  default:
    if (isDigit(c)) {
      number();
    } else if (isAlpha(c)) {
      identifier();
    } else {
      // Report error but continue scanning
      error(line, "Unexpected character.");
    }
    break;
  };
}

void Scanner::identifier() {
  while (isAlphaNumeric(peek())) {
    advance();
  }

  std::string text = std::string{program.substr(start, current - start)};

  TokenType type = keywords.count(text) ? keywords.at(text) : IDENTIFIER;
  addToken(type);
}

void Scanner::number() {
  while (isDigit(peek())) {
    advance();
  }

  int numberLiteral =
      std::stoi(std::string{program.substr(start, current - start)});
  addToken(NUMBER, numberLiteral);
}

void Scanner::string() {
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n')
      line++;
    advance();
  }

  if (isAtEnd()) {
    // Report error but finish scanning without crashing
    error(line, "Unterminated string.");
    return;
  }

  advance();

  std::string word{program.substr(start + 1, current - start - 2)};
  addToken(STRING, std::move(word));
}

bool Scanner::match(const char &expected) {
  if (isAtEnd() || program[current] != expected) {
    return false;
  }

  current++;
  return true;
}

char Scanner::peek() const {
  if (isAtEnd()) {
    return '\0';
  }
  return program[current];
}

char Scanner::peekNext() const {
  if (current + 1 >= static_cast<int>(program.size())) {
    return '\0';
  }
  return program[current + 1];
}

bool Scanner::isAtEnd() const {
  return current >= static_cast<int>(program.size());
}

char Scanner::advance() { return program[current++]; }

Scanner::Scanner(std::string_view program) : program{program} {
  tokens.reserve(256);
}

void Scanner::scanTokens() {
  while (!isAtEnd()) {
    start = current;
    scanToken();
  }

  tokens.emplace_back(END_OF_FILE, "", nullptr, line);
}

std::vector<Token> Scanner::getTokens() const { return tokens; }
