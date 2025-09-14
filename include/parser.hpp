#pragma once

#include <variant>
#include <vector>

#include "token.hpp"
#include "types.h"

using namespace token;

namespace Node {
struct ExprIntLit {
  Token int_lit;
};
struct ExprIdent {
  Token ident;
};
struct Expr {
  std::variant<ExprIntLit, ExprIdent> v;
};

struct StmtExit {
  Node::Expr expr;
};
struct StmtLet {
  Token ident;
  Node::Expr expr;
};
struct Stmt {
  std::variant<StmtExit, StmtLet> v;
};

struct Program {
  std::vector<Node::Stmt> stmts;
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

  std::optional<Node::Expr> parse_expr();

  const std::vector<Token> m_Tokens;
  usize m_Index = 0;
};
