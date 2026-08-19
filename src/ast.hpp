#pragma once
#include <memory>
#include <string>
#include <vector>
/* grammer
 * program := (statement ;)*
 * statement := assignment | expression
 * assignment := ident = expression
 * expression := abstraction | application
 * abstraction := lamdaident.expression
 * application := atom+
 * atom := ident | (expression)
 */

struct node {
  virtual ~node() = default;
  virtual std::string repr() = 0;
};

struct variable : node {
  int index = -1; // free var
  std::string name;
  std::string repr() override { return name; }
};

struct namedref : node {
  std::string name;
  std::string repr() override { return name; }
};

struct application : node {
  std::vector<std::unique_ptr<node>> atoms;
  std::string repr() override {
    std::string ans;
    for (auto &x : atoms) {
      ans += x->repr() + " ";
    }
    return ans;
  }
};

struct abstraction : node {
  std::string param;
  std::unique_ptr<node> body;
  std::string repr() override { return "\\" + param + "." + body->repr(); }
};

struct assignment : node {
  std::string lvalue;
  std::unique_ptr<node> rvalue;
  std::string repr() override { return lvalue + '=' + rvalue->repr(); }
};

using program = std::vector<std::unique_ptr<node>>;
