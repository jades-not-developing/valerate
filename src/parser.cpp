#include "parser.hpp"
#include "macros.hpp"
#include "token.hpp"

Parser::Parser(std::vector<Token> tokens)
    : m_Tokens(std::move(tokens)), m_Alloc(1024 * 1024 * 4) {}

std::optional<Token> Parser::peek(i32 offset) const {
  if (m_Index + offset >= m_Tokens.size()) {
    return {};
  } else {
    return m_Tokens.at(m_Index + offset);
  }
}

Token Parser::consume() { return m_Tokens.at(m_Index++); }

bool Parser::expect(TokenType type, i32 offset) {
  return peek(offset).has_value() && peek(offset).value().type == type;
}

std::optional<Node::Term *> Parser::parse_term() {
  if (expect(TokenType::int_lit)) {
    auto term_int_lit = m_Alloc.alloc<Node::TermIntLit>();
    term_int_lit->int_lit = consume();

    auto term = m_Alloc.alloc<Node::Term>();
    term->v = term_int_lit;

    return term;
  }

  else if (expect(TokenType::ident)) {
    auto term_ident = m_Alloc.alloc<Node::TermIdent>();
    term_ident->ident = consume();

    auto term = m_Alloc.alloc<Node::Term>();
    term->v = term_ident;

    return term;
  } else {
    return {};
  }
}

std::optional<Node::Expr *> Parser::parse_expr() {
  if (auto term = parse_term()) {
    if (expect(TokenType::plus)) {
      auto bin_expr = m_Alloc.alloc<Node::BinExpr>();
      auto bin_expr_add = m_Alloc.alloc<Node::BinExprAdd>();
      auto lhs_expr = m_Alloc.alloc<Node::Expr>();
      lhs_expr->v = term.value();
      bin_expr_add->lhs = lhs_expr;
      consume();

      auto rhs = parse_expr();
      if (rhs.has_value()) {
        bin_expr_add->rhs = rhs.value();
        bin_expr->v = bin_expr_add;
        auto expr = m_Alloc.alloc<Node::Expr>();
        expr->v = bin_expr;
        return expr;
      } else {
        PANIC("Expected expression");
      }
    }

    else {
      auto expr = m_Alloc.alloc<Node::Expr>();
      expr->v = term.value();
      return expr;
    }
  }

  else
    return {};
}

std::optional<Node::Stmt> Parser::parse_stmt() {
  if (expect(TokenType::exit) && expect(TokenType::open_paren, 1)) {
    consume();
    consume();

    auto stmt_exit = m_Alloc.alloc<Node::StmtExit>();

    if (auto node_expr = parse_expr())
      stmt_exit->expr = node_expr.value();
    else
      PANIC("Invalid Expression!");

    if (expect(TokenType::close_paren))
      consume();
    else
      PANIC("Expected `)`");

    if (expect(TokenType::semi))
      consume();
    else
      PANIC("Expected `;`");

    return Node::Stmt{.v = stmt_exit};
  } else if (expect(TokenType::let)) {
    if (!expect(TokenType::ident, 1))
      PANIC("Expected ident, found " << peek(1).value());
    if (!expect(TokenType::eq, 2))
      PANIC("Expected `=`, found " << peek(1).value());
    consume();
    auto stmt_let = m_Alloc.alloc<Node::StmtLet>();
    stmt_let->ident = consume();
    consume();

    if (auto expr = parse_expr()) {
      stmt_let->expr = expr.value();
    } else {
      PANIC("Invalid expression");
    }

    if (!expect(TokenType::semi))
      PANIC("Expected `;`, found " << peek().value());
    consume();

    return Node::Stmt{.v = stmt_let};
  } else {
    return {};
  }
}

Node::Program Parser::parse_program() {
  Node::Program prog;

  while (peek().has_value()) {
    if (auto stmt = parse_stmt()) {
      prog.stmts.push_back(stmt.value());
    } else {
      PANIC("Invalid Statement");
    }
  }

  return prog;
}
