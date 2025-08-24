#include "token.hpp"

#include <iostream>
#include <cassert>

std::ostream &token::operator<<(std::ostream &os, const Token &t) {
  switch (t.type) {
  case token::Type::_return: {
    os << "Return";
  } break;

  case token::Type::int_lit: {
    if (t.value) {
      os << "IntLit(Some(" << t.value.value() << "))";
    } else {
      os << "IntLit(None)";
    }
  } break;

  case token::Type::semi: {
    os << "Semi";
  } break;
  }

  return os;
}
