#include "eval.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <iostream>

int main() {
  std::string input;
  std::string line;
  while (true) {
    std::cout << "λ> ";
    if (!std::getline(std::cin, line))
      break; // EOF (Ctrl+D)
    if (line == "exit" || line == "quit")
      break;
    input += line + "\n";
  }

  Lexer lex(input);
  auto tokens = lex.tokenize();
  Parser p(tokens);
  auto prog = p.parseProgram();
  Eval e(std::move(prog));
  e.run();
}
