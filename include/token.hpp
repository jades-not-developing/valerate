#pragma once

#include <optional>
#include <string>
#include <vector>


#define TOKEN_EXIT() {.type = token::TokenType::exit}
#define TOKEN_INT_LIT(x) {.type = token::TokenType::int_lit, .value = x}
#define TOKEN_SEMI() {.type = token::TokenType::semi}

namespace token {
  #include "types.h"

  enum class TokenType {
    exit,
    int_lit,
    semi,
  };

  struct Token {
    TokenType type;
    std::optional<std::string> value;
    friend std::ostream &operator<<(std::ostream &os, const Token &t);
  };

  std::ostream &operator<<(std::ostream &os, const Token &t);

  class Tokenizer {
  private:
    std::optional<char> peek(i32 offset = 0) const;
    char consume();

    i32 m_Index = 0;
    const std::string m_Src;

  public:
    [[nodiscard]] inline Tokenizer(const std::string src) : m_Src(std::move(src)) {}

    std::vector<token::Token> tokenize();
  };
} // namespace token
