#include "eval.hpp"
#include <iostream>
#include <stdexcept>

std::unique_ptr<node> Eval::shift(node *target, int amount, int cutoff) {
  if (auto *v = dynamic_cast<variable *>(target)) {
    auto copy = std::make_unique<variable>();
    copy->name = v->name;
    if (v->index >= cutoff) {
      copy->index = v->index + amount;
    } else {
      copy->index = v->index;
    }
    return copy;
  } else if (auto *v = dynamic_cast<abstraction *>(target)) {
    auto copy = std::make_unique<abstraction>();
    copy->param = v->param;
    copy->body = shift(v->body.get(), amount, cutoff + 1);
    return copy;
  } else if (auto *v = dynamic_cast<application *>(target)) {
    auto copy = std::make_unique<application>();
    for (auto &x : v->atoms) {
      copy->atoms.push_back(shift(x.get(), amount, cutoff));
    }
    return copy;
  } else if (auto *v = dynamic_cast<namedref *>(target)) {
    auto copy = std::make_unique<namedref>();
    copy->name = v->name;
    return copy;
  }
  throw std::runtime_error("shift: unknown node type");
}
std::unique_ptr<node> Eval::substitute(node *target, int idx, node *arg) {
  if (auto *v = dynamic_cast<variable *>(target)) {
    if (v->index == idx) {
      return shift(arg, 0, 0);
    }
    auto copy = std::make_unique<variable>();
    copy->name = v->name;
    copy->index = v->index;
    return copy;
  }
  if (auto *v = dynamic_cast<abstraction *>(target)) {
    auto copy = std::make_unique<abstraction>();
    copy->param = v->param;
    auto shiftedArg = shift(arg, 0, 1);
    copy->body = substitute(v->body.get(), idx + 1, shiftedArg.get());
    return copy;
  }
  if (auto *v = dynamic_cast<application *>(target)) {
    auto copy = std::make_unique<application>();
    for (auto &x : v->atoms) {
      copy->atoms.push_back(substitute(x.get(), idx, arg));
    }
    return copy;
  }
  if (auto *v = dynamic_cast<namedref *>(target)) {
    auto copy = std::make_unique<namedref>();
    copy->name = v->name;
    return copy;
  }
  throw std::runtime_error("substitute: invalid node");
}

std::unique_ptr<node> Eval::betaReduction(node *body, node *arg) {
  auto shiftedArg = shift(arg, 1, 0);
  auto substituted = substitute(body, 0, shiftedArg.get());
  return shift(substituted.get(), 0, -1);
}

std::unique_ptr<node>
Eval::eval(node *n, std::map<std::string, std::unique_ptr<node>> &defs) {
  std::unique_ptr<node> current;
  while (true) {
    if (auto *v = dynamic_cast<namedref *>(n)) {
      auto it = defs.find(v->name);
      if (it == defs.end())
        return current ? std::move(current) : shift(n, 0, 0);
      n = it->second.get();
      continue;
    }
    if (auto *v = dynamic_cast<application *>(n)) {
      auto lhs = eval(v->atoms[0].get(), defs);
      for (size_t i = 1; i < v->atoms.size(); i++) {
        // if lhs is abstraction then get the arguments and apply beta reduction
        if (auto *abs = dynamic_cast<abstraction *>(lhs.get())) {
          current = betaReduction(abs->body.get(), v->atoms[i].get());
          lhs = eval(current.get(), defs);
        } else {
          // if lhs is not abstraction it's an application make a new app and
          // return it
          auto rest = std::make_unique<application>();
          rest->atoms.push_back(std::move(lhs));
          for (size_t j = i; j < v->atoms.size(); j++)
            rest->atoms.push_back(shift(v->atoms[j].get(), 0, 0));
          return rest;
        }
      }
      return lhs;
    }
    if (dynamic_cast<abstraction *>(n)) {
      if (current)
        return current;
      return shift(n, 0, 0);
    }
  }
}

void Eval::run() {
  for (auto &stmt : program) {
    if (auto *as = dynamic_cast<assignment *>(stmt.get())) {
      auto result = eval(as->rvalue.get(), defs);
      // std::cout << as->lvalue << " = " << result->repr() << "\n";
      defs[as->lvalue] = std::move(result);
    } else {
      auto result = eval(stmt.get(), defs);
      std::cout << result->repr() << "\n";
    }
  }
}
