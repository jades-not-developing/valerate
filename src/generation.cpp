#include "generation.hpp"

Generator::Generator(Node::Program program)
  : m_Program(std::move(program))
{

}

void Generator::gen_expr(const Node::Expr& expr)
{
  struct ExprVisitor {
    Generator* gen;

    void operator()(const Node::ExprIntLit& expr_int_lit) const
    {
      gen->m_Output << "    mov rax, " << expr_int_lit.int_lit.value.value() << "\n";
      gen->m_Output << "    push rax" << "\n";
    }

    void operator()(const Node::ExprIdent& expr_ident) const
    {
    }
  };

  ExprVisitor visitor{.gen = this};
  std::visit(visitor, expr.v);
}

void Generator::gen_stmt(const Node::Stmt& stmt) 
{
  struct StmtVisitor {
    Generator* gen;

    void operator()(const Node::StmtExit& stmt_exit) const
    {
      gen->m_HasGeneratedExit = true;
      gen->gen_expr(stmt_exit.expr);

      gen->m_Output << "    pop rax\n";      
      gen->m_Output << "    ret\n";
    }

    void operator()(const Node::StmtLet& stmt_let) const
    {
    }
  };

  StmtVisitor visitor{ .gen = this };
  std::visit(visitor, stmt.v);
}

std::string Generator::gen_program()
{
  m_Output << "global _start\n_start:\n";

  for (const Node::Stmt& stmt : m_Program.stmts) {
    gen_stmt(stmt);
  }

  if (!m_HasGeneratedExit) {
    m_Output << "    mov rax, 0\n";
    m_Output << "    ret\n";
  }

  return m_Output.str();
}
