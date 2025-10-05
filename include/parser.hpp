#pragma once

#include <variant>
#include <vector>

#include "arena.hpp"
#include "token.hpp"
#include "types.h"

using namespace token;

namespace Node {

struct Expr;
struct BinExprAdd {
  Expr *lhs;
  Expr *rhs;
};
// struct BinExprMul {
//   Expr *lhs;
//   Expr *rhs;
// };
struct BinExpr {
  BinExprAdd *v;
};

struct TermIntLit {
  Token int_lit;
};
struct TermIdent {
  Token ident;
};
struct Term {
  std::variant<TermIntLit *, TermIdent *> v;
};

struct Expr {
  std::variant<Term *, BinExpr *> v;
};

struct StmtExit {
  Expr *expr;
};
struct StmtLet {
  Token ident;
  Expr *expr;
};
struct Stmt {
  std::variant<StmtExit *, StmtLet *> v;
};

struct Program {
  std::vector<Stmt> stmts;
};
} // namespace Node

class Parser {
public:
  explicit Parser(std::vector<Token> tokens);

  Node::Program parse_program();
  std::optional<Node::Stmt> parse_stmt();

private:
  [[nodiscard]] std::optional<Token> peek(i32 offset = 0) const;
  Token consume();
  bool expect(TokenType type, i32 offset = 0);

  std::optional<Node::Expr *> parse_expr();
  std::optional<Node::Term *> parse_term();

  const std::vector<Token> m_Tokens;
  usize m_Index = 0;
  ArenaAllocator m_Alloc;
};
