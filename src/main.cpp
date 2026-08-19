#include "ast.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <iostream>
std::string tokenTypeToString(Lexer::TokenType type) {
  switch (type) {
  case Lexer::Lambda:
    return "Lambda";
  case Lexer::Dot:
    return "Dot";
  case Lexer::Assign:
    return "Assign";
  case Lexer::OpenParen:
    return "OpenParen";
  case Lexer::CloseParen:
    return "CloseParen";
  case Lexer::Semicolon:
    return "Semicolon";
  case Lexer::Identifier:
    return "Identifier";
  }
  return "Unknown";
}
int main() {
  // Lexer a("\\x.(\\y.x)");
  std::cout << "enter input : ";
  std::string input;
  std::getline(std::cin, input);
  Lexer a(input);
  auto tokens = a.tokenize();
  for (auto &x : tokens) {
    std::cout << tokenTypeToString(x.type) << " ";
  }
  std::cout << "\n";
  Parser p(tokens);
  auto prog = p.parseProgram();
  for (auto &stmt : prog) {
    std::cout << stmt->repr() << "\n";
  }
}
