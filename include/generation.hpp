#pragma once

#include "parser.hpp"
#include <sstream>
#include <unordered_map>

class Generator {
public:
  Generator(Node::Program program);

  void gen_stmt(const Node::Stmt &stmt);
  void gen_expr(const Node::Expr &expr);

  std::string gen_program();

private:
  struct Var {
    usize stack_loc;
  };

private:
  const Node::Program m_Program;
  std::stringstream m_Output;
  usize m_StackSize = 0;
  bool m_HasGeneratedExit;
  std::unordered_map<std::string, Var> m_Vars{};

private:
  void push(const std::string &reg);
  void pop(const std::string &reg);
};
