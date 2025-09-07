#pragma once

#include "parser.hpp"
#include <sstream>

class Generator {
public:
  Generator(Node::Program program);

  void gen_stmt(const Node::Stmt& stmt);
  void gen_expr(const Node::Expr& expr);

  std::string gen_program();

private:
  const Node::Program m_Program;
  std::stringstream m_Output;

  bool m_HasGeneratedExit;
};
