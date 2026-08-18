#include "lexer.hpp"

std::vector<Lexer::Token> Lexer::tokenize() {
  std::vector<Token> tokens;

  for (int i = 0; i < input.size(); i++) {
    switch (input[i]) {
    case '\\':
      tokens.push_back({Lambda, "\\"});
      break;
    case '.':
      tokens.push_back({Dot, "."});
      break;
    case '=':
      tokens.push_back({Assign, "="});
      break;
    case '(':
      tokens.push_back({OpenParen, "("});
      break;
    case ')':
      tokens.push_back({CloseParen, ")"});
      break;
    default:
      if (isalpha(input[i])) {
        std::string name = "";
        while (i < input.size() && isalpha(input[i])) {
          name += input[i];
          i++;
        }
        i--;
        tokens.push_back({Identifier, name});
      }
    }
  }
  return tokens;
}
