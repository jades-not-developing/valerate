#include "parser.hpp"
#include "macros.hpp"
#include "token.hpp"

Parser::Parser(std::vector<Token> tokens) : m_Tokens(std::move(tokens)) {}

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

std::optional<Node::Expr> Parser::parse_expr() {
  if (expect(TokenType::int_lit))
    return Node::Expr{.v = Node::ExprIntLit{.int_lit = consume()}};

  else if (expect(TokenType::ident))
    return Node::Expr{.v = Node::ExprIdent{.ident = consume()}};

  else
    return {};
}

std::optional<Node::Stmt> Parser::parse_stmt() {
  if (expect(TokenType::exit) && expect(TokenType::open_paren, 1)) {
    consume();
    consume();

    Node::StmtExit stmt_exit;

    if (auto node_expr = parse_expr())
      stmt_exit = Node::StmtExit{.expr = node_expr.value()};
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
    auto stmt_let = Node::StmtLet{.ident = consume()};
    consume();

    if (auto expr = parse_expr()) {
      stmt_let.expr = expr.value();
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
