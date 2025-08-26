#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#include "fs.hpp"
#include "macros.hpp"
#include "token.hpp"
#include "types.h"

using namespace token;

std::vector<Token> tokenize(const std::string &str) {
  std::vector<Token> tokens;
  std::string buf;
  for (i32 i = 0; i < str.length(); ++i) {
    char c = str.at(i);

    // Identifier
    if (std::isalpha(c)) {
      buf.push_back(c);
      i++;
      while (std::isalnum(str.at(i))) {
        buf.push_back(str.at(i));
        i++;
      }
      i--;

      if (buf == "return") {
        tokens.push_back(TOKEN_RETURN());
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
      while (std::isdigit(str.at(i))) {
        buf.push_back(str.at(i));
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

std::string assemble(const std::vector<Token> &tokens) {
  std::stringstream output;
  output << "global _start\n_start:\n";
  for (i32 i = 0; i < tokens.size(); ++i) {
    const Token &t = tokens.at(i);
    if (t.type == TokenType::_return) {
      if (i + 1 < tokens.size() &&
          tokens.at(i + 1).type == TokenType::int_lit) {
        if (i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::semi) {
          output << "    mov eax, " << tokens.at(i+1).value.value() << "\n";
          output << "    ret\n";
        }
      }
    }
  }

  return output.str();
}

i32 main(i32 argc, char *argv[]) {
  if (argc != 2) {
    PANIC("Incorrect Usage" << std::endl
                            << argv[0] << " \033[40;37m<input.diol>");
  }

  std::string buf = fs::read_file(argv[1]);
  auto tokens = tokenize(buf);

  {
    std::fstream file("out.asm", std::ios::out);
    file << assemble(tokens);
  }

  system("nasm -felf64 out.asm");
  system("ld -o out.exe out.o");

  return EXIT_SUCCESS;
}
