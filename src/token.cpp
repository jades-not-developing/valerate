#include "token.hpp"

#include "macros.hpp"
#include "types.h"

#include <cassert>
#include <cctype>
#include <iostream>

using namespace token;

std::ostream &token::operator<<(std::ostream &os, const Token &t) {
  switch (t.type) {
  case token::TokenType::exit: {
    os << "Exit";
  } break;

  case token::TokenType::let: {
    os << "Let";
  } break;

  case token::TokenType::eq: {
    os << "Eq";
  } break;

  case token::TokenType::int_lit: {
    if (t.value) {
      os << "IntLit(Some(\"" << t.value.value() << "\"))";
    } else {
      os << "IntLit(None)";
    }
  } break;

  case token::TokenType::ident: {
    if (t.value) {
      os << "Ident(Some(\"" << t.value.value() << "\"))";
    } else {
      os << "Ident(None)";
    }
  } break;

  case token::TokenType::semi: {
    os << "Semi";
  } break;
  case token::TokenType::open_paren: {
    os << "OpenParen";
  } break;
  case token::TokenType::close_paren: {
    os << "CloseParen";
  } break;
  }

  return os;
}

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
        continue;
      } else if (buf == "let") {
        tokens.push_back(TOKEN_LET());
        buf.clear();
        continue;
      } else {
        tokens.push_back(TOKEN_IDENT(buf));
        buf.clear();
        continue;
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
    else if (peek().value() == '(') {
      consume();
      tokens.push_back(TOKEN_OPEN_PAREN());
      continue;
    } else if (peek().value() == '=') {
      consume();
      tokens.push_back(TOKEN_EQ());
      continue;
    } else if (peek().value() == ')') {
      consume();
      tokens.push_back(TOKEN_CLOSE_PAREN());
      continue;
    } else if (peek().value() == ';') {
      consume();
      tokens.push_back(TOKEN_SEMI());
      continue;
    }

    // Whitespace
    else if (std::isspace(peek().value())) {
      consume();
      continue;
    }

    // Who knows what
    else
      PANIC("Unrecognized character '" << peek().value() << "'");
  }

  m_Index = 0;
  return tokens;
}

std::optional<char> Tokenizer::peek(i32 offset) const {
  if (m_Index + offset >= m_Src.length()) {
    return {};
  }

  return m_Src.at(m_Index + offset);
}

char Tokenizer::consume() { return m_Src.at(m_Index++); }
