#include "lexer.hpp"

std::vector<Lexer::Token> Lexer::tokenize() {
  std::vector<Token> tokens;

  for (int i = 0; i < (int)input.size(); i++) {
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
    case ';':
      tokens.push_back({Semicolon, ";"});
      break;
    case ' ':
    case '\t':
    case '\n':
    case '\r':
      break;
    default:
      if (isalpha(input[i])) {
        std::string name = "";
        while (i < (int)input.size() && isalpha(input[i])) {
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
