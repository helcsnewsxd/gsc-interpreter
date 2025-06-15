#include "dbg/astPrinter.hpp"
#include "gsc/error.hpp"
#include "gsc/parser.hpp"
#include "gsc/scanner.hpp"
#include "gsc/token.hpp"
#include <fstream>
#include <iostream>
#include <vector>

void runFile(std::string_view filename);
void runPrompt();

int main(int argc, char *argv[]) {
  if (argc > 2) {
    std::cerr << "Usage: " << argv[0] << " [file.gsc]" << std::endl;
    std::exit(EXIT_FAILURE);
  } else if (argc == 2) {
    runFile(argv[1]);
  } else {
    runPrompt();
  }
}

std::string readFile(std::string_view filename) {
  std::ifstream file(filename.data());

  if (!file) {
    std::cerr << "Could not open file: " << filename << std::endl;
    std::exit(EXIT_FAILURE);
  } else {
    std::string content{std::istreambuf_iterator<char>(file),
                        std::istreambuf_iterator<char>()};
    return content;
  }
}

void run(std::string_view program) {
  Scanner scanner{program};
  scanner.scanTokens();
  std::vector<Token> tokens = scanner.getTokens();
  Parser parser{tokens};
  std::shared_ptr<Expr> expression = parser.parse();

  // To debug
  for (const auto &token : tokens) {
    std::cout << token.toString() << std::endl;
  }

  if (hadError) {
    std::cerr << "Error while parsing the program." << std::endl;
    std::exit(EXIT_FAILURE);
  } else {
    std::cout << "Parsed expression: " << AstPrinter().print(expression)
              << std::endl;
  }
}

void runFile(std::string_view filename) {
  std::string program = readFile(filename);
  run(program);

  if (hadError) {
    std::cerr << "Error while running file: " << filename << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

void runPrompt() {
  while (true) {
    std::cout << ">> ";
    std::string line;
    if (!std::getline(std::cin, line)) {
      std::cout << std::endl;
      break;
    } else {
      run(line);
      hadError = false; // Reset error state for the next line
    }
  }
}
