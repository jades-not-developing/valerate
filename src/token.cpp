#include "token.hpp"

#include <iostream>
#include <cassert>

std::ostream &token::operator<<(std::ostream &os, const Token &t) {
  switch (t.type) {
  case token::TokenType::_return: {
    os << "Return";
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
