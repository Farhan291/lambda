#pragma once

#include <string>
#include <vector>
class Lexer {
  std::string input;

public:
  enum TokenType { Lambda, Dot, Identifier, OpenParen, CloseParen, Assign };
  struct Token {
    TokenType type;
    std::string value;
  };
  Lexer(const std::string &input) : input(input) {}
  std::vector<Token> tokenize();
};
