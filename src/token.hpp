#pragma once

#include <string>
#include <optional>

#define TOKEN_RETURN { .type = token::Type::_return }
#define TOKEN_INT_LIT(x) { .type = token::Type::int_lit, .value = x }
#define TOKEN_SEMI(x) { .type = token::Type::semi }

namespace token {
  enum class Type {
    _return,
    int_lit,
    semi,
  };

  struct Token {
    Type type;
    std::optional<std::string> value;
    friend std::ostream& operator<<(std::ostream& os, const Token& t);
  };

  std::ostream& operator<<(std::ostream& os, const Token& t);
}
