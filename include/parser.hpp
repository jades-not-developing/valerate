#pragma once

#include <vector>

#include "token.hpp"
#include "types.h"

using namespace token;

namespace Node {
  struct Expr {
    Token int_lit;
  };

  struct Exit {
    Expr expr;
  };
}

class Parser {
public:
  explicit Parser(std::vector<Token> tokens);

  std::optional<Node::Exit> parse();

private:
  [[nodiscard]] std::optional<Token> peek(i32 offset = 0) const;
  Token consume();

  std::optional<Node::Expr> parse_expr();

  const std::vector<Token> m_Tokens;
  usize m_Index = 0;
};
