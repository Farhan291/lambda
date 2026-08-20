#include "eval.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <iostream>

int main() {
  std::string line;
  Eval e;
  while (true) {
    std::cout << "λ> ";
    if (!std::getline(std::cin, line))
      break; // EOF (Ctrl+D)
    if (line == "exit" || line == "quit")
      break;
    e.runLine(line);
  }
}
