#pragma once
#include "ast.hpp"
#include "lexer.hpp"

class Parser {
  std::vector<Lexer::Token> tokens;
  int pos;

  int sz() { return (int)tokens.size(); }
  Lexer::Token &peek();
  Lexer::Token advance();
  bool check(Lexer::TokenType t);
  Lexer::Token expect(Lexer::TokenType t, const std::string &msg);

  std::unique_ptr<node> parseStatement();
  std::unique_ptr<node> parseAssignment();
  std::unique_ptr<node> parseExpression();
  std::unique_ptr<node> parseAbstraction();
  std::unique_ptr<node> parseApplication();
  std::unique_ptr<node> parseAtom();

public:
  Parser(const std::vector<Lexer::Token> &t) : tokens(t) {}
  program parseProgram();
};
