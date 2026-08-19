#include "parser.hpp"
#include <memory>
#include <stdexcept>

Lexer::Token &Parser::peek() { return tokens[pos]; }

Lexer::Token Parser::advance() { return tokens[pos++]; }

bool Parser::check(Lexer::TokenType t) {
  if (pos >= sz())
    return false;
  return tokens[pos].type == t;
}

Lexer::Token Parser::expect(Lexer::TokenType t, const std::string &msg) {
  if (!check(t))
    throw std::runtime_error("Parser error expected " + msg);
  return advance();
}

void Parser::pushVar(const std::string &name) { context.push_back(name); }
void Parser::popVar() { context.pop_back(); }

int Parser::resolve(const std::string &name) {
  for (int i = (int)context.size() - 1; i >= 0; i--) {
    if (context[i] == name)
      return (int)context.size() - i - 1;
  }
  return -1; // free var
}

// program := statement*
program Parser::parseProgram() {
  program result;
  while (pos < sz()) {
    result.push_back(parseStatement());
    expect(Lexer::Semicolon, ";");
  }
  return result;
}

// statement := assignment | expression
std::unique_ptr<node> Parser::parseStatement() {
  if (check(Lexer::Identifier) && pos + 1 < sz() &&
      tokens[pos + 1].type == Lexer::Assign) {
    return parseAssignment();
  }
  return parseExpression();
}

// assignment := ident = expression
std::unique_ptr<node> Parser::parseAssignment() {
  std::string lvalue = expect(Lexer::Identifier, "Identifier").value;
  expect(Lexer::Assign, "=");
  auto rvalue = parseExpression();
  auto as = std::make_unique<assignment>();
  as->lvalue = lvalue;
  as->rvalue = std::move(rvalue);
  return as;
}

// expression := abstraction | application
std::unique_ptr<node> Parser::parseExpression() {
  if (check(Lexer::Lambda)) {
    return parseAbstraction();
  }
  return parseApplication();
}

// abstraction := lamdaident.expression
std::unique_ptr<node> Parser::parseAbstraction() {
  expect(Lexer::Lambda, "\\");
  std::string param = expect(Lexer::Identifier, "Identifier").value;
  pushVar(param);
  expect(Lexer::Dot, ".");
  std::unique_ptr<node> body = parseExpression();
  popVar();
  auto ab = std::make_unique<abstraction>();
  ab->param = param;
  ab->body = std::move(body);
  return ab;
}

// application := atom+
std::unique_ptr<node> Parser::parseApplication() {
  auto app = std::make_unique<application>();
  while (check(Lexer::Identifier) || check(Lexer::OpenParen)) {
    app->atoms.push_back(parseAtom());
  }
  return app;
}

// atom := ident | (expression)
std::unique_ptr<node> Parser::parseAtom() {
  if (check(Lexer::Identifier)) {
    auto atom = std::make_unique<variable>();
    atom->name = advance().value;
    int idx = resolve(atom->name);
    if (idx == -1) {
      auto ref = std::make_unique<namedref>();
      ref->name = atom->name;
      return ref;
    }
    atom->index = idx;
    return atom;
  } else if (check(Lexer::OpenParen)) {
    advance();
    auto e = parseExpression();
    expect(Lexer::CloseParen, ")");
    return e;
  }
  throw std::runtime_error("Parse error: unexpected token");
}
