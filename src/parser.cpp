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
  if (expect(TokenType::int_lit)) {
    return Node::Expr{
      .v = Node::ExprIntLit{
        .int_lit = consume()
      }
    };
  } 
  else if (expect(TokenType::ident)) {
    return Node::Expr {
      .v = Node::ExprIdent{
        .ident = consume()
      }
    };
  }
  else {
    return {};
  }
}

std::optional<Node::Exit> Parser::parse() {
  std::optional<Node::Exit> exit_node;

  while (peek().has_value()) {
    if (expect(TokenType::exit) && expect(TokenType::open_paren, 1)) {
      consume();
      consume();

      if (auto node_expr = parse_expr())
        exit_node = Node::Exit{.expr = node_expr.value()};
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
    }
  }

  m_Index = 0;
  return exit_node;
}
