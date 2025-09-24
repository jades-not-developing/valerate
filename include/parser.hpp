#pragma once

#include <variant>
#include <vector>

#include "token.hpp"
#include "types.h"
#include "arena.hpp"

using namespace token;

namespace Node {

struct Expr;
struct BinExprAdd {
  Expr* lhs;
  Expr* rhs;
};
struct BinExprMul {
  Expr* lhs;
  Expr* rhs;
};
struct BinExpr {
  std::variant<BinExprAdd*, BinExprMul*> v;
};

struct ExprIntLit {
  Token int_lit;
};
struct ExprIdent {
  Token ident;
};
struct Expr {
  std::variant<ExprIntLit*, ExprIdent*, BinExpr*> v;
};

struct StmtExit {
  Expr* expr;
};
struct StmtLet {
  Token ident;
  Expr* expr;
};
struct Stmt {
  std::variant<StmtExit*, StmtLet*> v;
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

  std::optional<Node::Expr*> parse_expr();

  const std::vector<Token> m_Tokens;
  usize m_Index = 0;
  ArenaAllocator m_Alloc;
};
