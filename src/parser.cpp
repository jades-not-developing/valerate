#include "parser.hpp"
#include "macros.hpp"


Parser::Parser(std::vector<Token> tokens)
  : m_Tokens(std::move(tokens))
{
}

std::optional<Token> Parser::peek(i32 ahead) const
{
  if (m_Index + ahead > m_Tokens.size()) {
    return {};
  } else {
    return m_Tokens.at(m_Index);
  }
}

Token Parser::consume()
{
  return m_Tokens.at(m_Index++);
}


std::optional<Node::Expr> Parser::parse_expr()
{
  if (peek().has_value() && peek().value().type == TokenType::int_lit) {
    return Node::Expr { .int_lit = consume() };
  } else {
    return {};
  } 
}

std::optional<Node::Exit> Parser::parse()
{
  std::optional<Node::Exit> exit_node;

  while (peek().has_value()) {
    if (peek().value().type == TokenType::exit) {
      consume();

      if (auto node_expr = parse_expr()) 
        exit_node = Node::Exit { .expr = node_expr.value() };
      else PANIC("Invalid Expression!");
     
      if (peek().has_value() && peek().value().type == TokenType::semi) consume();
      else PANIC("Invalid Expression!");
    }
  }

  m_Index = 0;
  return exit_node;
}
