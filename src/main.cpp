#include "eval.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <iostream>
int main() {
  std::cout << "enter input : ";
  std::string input;
  std::getline(std::cin, input);
  Lexer a(input);
  auto tokens = a.tokenize();
  Parser p(tokens);
  auto prog = p.parseProgram();
  Eval e(prog);
  e.run();
}
