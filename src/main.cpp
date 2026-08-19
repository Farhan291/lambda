#include "ast.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <iostream>
int main() {
  // Lexer a("\\x.(\\y.x)");
  std::string input = "id = \\x. x; id y;";
  Lexer a(input);
  auto tokens = a.tokenize();
  Parser p(tokens);
  auto prog = p.parseProgram();
  for (auto &stmt : prog) {
    std::cout << stmt->repr() << "\n";
  }
}
