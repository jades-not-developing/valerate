#include "generation.hpp"
#include "macros.hpp"

Generator::Generator(Node::Program program) : m_Program(std::move(program)) {}

void Generator::gen_expr(const Node::Expr* expr) {
  struct ExprVisitor {
    Generator *gen;

    void operator()(const Node::ExprIntLit* expr_int_lit) const {
      gen->m_Output << "    mov rax, " << expr_int_lit->int_lit.value.value()
                    << "\n";
      gen->push("rax");
    }

    void operator()(const Node::ExprIdent* expr_ident) const {
      if (!expr_ident->ident.value.has_value()) PANIC("Malformed ExprIdent, ident does not contain value (this is probably a compiler bug)");
      if (!gen->m_Vars.contains(expr_ident->ident.value.value())) PANIC("Undeclared identifier `" << expr_ident->ident.value.value() << "`");

      const auto& var = gen->m_Vars.at(expr_ident->ident.value.value());

      std::stringstream offset;
      offset << "QWORD [rsp + " << (gen->m_StackSize - var.stack_loc - 1) * 8 << "]";

      gen->push(offset.str());
    }

    void operator()(const Node::BinExpr* bin_expr) const {
      PANIC("Not implemented");
    }
  };

  ExprVisitor visitor{.gen = this};
  std::visit(visitor, expr->v);
}

void Generator::gen_stmt(const Node::Stmt &stmt) {
  struct StmtVisitor {
    Generator *gen;

    void operator()(const Node::StmtExit* stmt_exit) const {
      gen->m_HasGeneratedExit = true;
      gen->gen_expr(stmt_exit->expr);
      gen->pop("rcx");
      //gen->m_Output << "    mov rcx, rax\n";
      gen->m_Output << "    call ExitProcess\n";
    }

    void operator()(const Node::StmtLet* stmt_let) const {
      if (!stmt_let->ident.value.has_value()) PANIC("Malformed StmtLet, ident does not contain value (this is probably a compiler bug)");
      if (gen->m_Vars.contains(stmt_let->ident.value.value())) PANIC("Identifier `" << stmt_let->ident.value.value() << "` already declared");

      gen->m_Vars.insert({ stmt_let->ident.value.value(), Var { .stack_loc = gen->m_StackSize } });
      gen->gen_expr(stmt_let->expr);
    }
  };

  StmtVisitor visitor{.gen = this};
  std::visit(visitor, stmt.v);
}

std::string Generator::gen_program() {
  m_Output << "global WinMainCRTStartup\n";
  m_Output << "extern ExitProcess\n";
  m_Output << "section .text\n";
  m_Output << "WinMainCRTStartup:\n";

  for (const Node::Stmt &stmt : m_Program.stmts) {
    gen_stmt(stmt);
  }

  if (!m_HasGeneratedExit) {
    m_Output << "    mov rcx, 0\n";
    m_Output << "    call ExitProcess\n";
  }

  return m_Output.str();
}

void Generator::push(const std::string &reg) {
  m_Output << "    push " << reg << "\n";
  m_StackSize++;
}

void Generator::pop(const std::string &reg) {
  m_Output << "    pop " << reg << "\n";
  m_StackSize--;
}
