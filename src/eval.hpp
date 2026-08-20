#pragma once

#include "ast.hpp"
#include <map>

class Eval {
  std::vector<std::unique_ptr<node>> program;
  std::map<std::string, std::unique_ptr<node>> defs;

  std::unique_ptr<node> shift(node *target, int amount, int cutoff);
  std::unique_ptr<node> substitute(node *target, int idx, node *arg);
  std::unique_ptr<node> betaReduction(node *body, node *arg);

  std::unique_ptr<node>
  eval(node *n, std::map<std::string, std::unique_ptr<node>> &defs);

  std::unique_ptr<node>
  normalize(node *n, std::map<std::string, std::unique_ptr<node>> &defs);

public:
  Eval(std::vector<std::unique_ptr<node>> p) : program(std::move(p)) {};
  void run();
};
