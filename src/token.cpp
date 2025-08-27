#include "token.hpp"

#include "types.h"
#include "macros.hpp"

#include <cctype>
#include <iostream>
#include <cassert>

using namespace token;

std::ostream &token::operator<<(std::ostream &os, const Token &t) {
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

  while (peek().has_value()) {
    
    // Identifier/Keyword
    if (std::isalpha(peek().value())) {
      buf.push_back(consume());
      while (peek().has_value() && std::isalnum(peek().value())) {
        buf.push_back(consume());
      }
      if (buf == "exit") {
        tokens.push_back(TOKEN_EXIT());
        buf.clear();
      }
    }

    // Numeric
    else if (std::isdigit(peek().value())) {
      buf.push_back(consume());

      while (peek().has_value() && std::isdigit(peek().value())) {
        buf.push_back(consume());
      }

      tokens.push_back(TOKEN_INT_LIT(buf));
      buf.clear();
    }

    // Operators
    else if (peek().value() == ';') 
      tokens.push_back(TOKEN_SEMI());

    // Whitespace
    else if (std::isspace(peek().value()))
      continue;

    // Who knows what
    else
      PANIC("Unrecognized character '" << peek().value() << "'");
  }

  m_Index = 0;
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
