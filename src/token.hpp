#pragma once

#include <string>
#include <optional>

#define TOKEN_RETURN() { .type = token::TokenType::_return }
#define TOKEN_INT_LIT(x) { .type = token::TokenType::int_lit, .value = x }
#define TOKEN_SEMI() { .type = token::TokenType::semi }

namespace token {
  enum class TokenType {
    _return,
    int_lit,
    semi,
  };

  struct Token {
    TokenType type;
    std::optional<std::string> value;
    friend std::ostream& operator<<(std::ostream& os, const Token& t);
  };

  std::ostream& operator<<(std::ostream& os, const Token& t);
}
