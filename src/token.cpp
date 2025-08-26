#include "token.hpp"

#include "types.h"
#include "macros.hpp"

#include <iostream>
#include <cassert>

using namespace token;

std::ostream &operator<<(std::ostream &os, const Token &t) {
  switch (t.type) {
  case token::TokenType::exit: {
    os << "Exit";
  } break;

  case token::TokenType::int_lit: {
    if (t.value) {
      os << "IntLit(Some(\"" << t.value.value() << "\"))";
    } else {
      os << "IntLit(None)";
    }
  } break;

  case token::TokenType::semi: {
    os << "Semi";
  } break;
  }

  return os;
}

Tokenizer::Tokenizer(const std::string src) : m_Src(std::move(src)) {}

std::vector<token::Token> Tokenizer::tokenize() {
  std::vector<Token> tokens;
  std::string buf;
  for (i32 i = 0; i < m_Src.length(); ++i) {
    char c = m_Src.at(i);

    // Identifier
    if (std::isalpha(c)) {
      buf.push_back(c);
      i++;
      while (std::isalnum(m_Src.at(i))) {
        buf.push_back(m_Src.at(i));
        i++;
      }
      i--;

      if (buf == "exit") {
        tokens.push_back(TOKEN_EXIT());
        buf.clear();
        continue;
      } else {
        PANIC("Unrecognized keyword '" << buf << "'");
      }
    }

    // Numerics
    else if (std::isdigit(c)) {
      buf.push_back(c);
      i++;
      while (std::isdigit(m_Src.at(i))) {
        buf.push_back(m_Src.at(i));
        i++;
      }
      i--;

      tokens.push_back(TOKEN_INT_LIT(buf));
      buf.clear();
    }

    // Operators
    else if (c == ';')
      tokens.push_back(TOKEN_SEMI());

    // Whitespace
    else if (std::isspace(c))
      continue;

    // Who knows what
    else
      PANIC("Unrecognized token '" << c << "'");
  }

  return tokens;
}

std::optional<char> Tokenizer::peek(i32 ahead) const {
  if (m_Index + ahead >= m_Src.length()) {
    return {};
  } 

  return m_Src.at(m_Index);
}



char Tokenizer::consume() {
  return m_Src.at(m_Index++);
}
