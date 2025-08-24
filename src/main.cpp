#include <cctype>
#include <iostream>
#include <vector>

#include "types.h"
#include "fs.hpp"
#include "macros.hpp"
#include "token.hpp"

std::vector<token::Token> tokenize(const std::string& str) {
  std::vector<token::Token> tokens;
  std::string buf {};
  for (i32 i = 0; i < str.length(); ++i) {
    char c = str.at(i);
    if (std::isalpha(c)) {
      buf.push_back(c);
      i++;
      while (std::isalnum(str.at(i))) {
        buf.push_back(str.at(i));
        i++;
      }
      i--;

      if (buf == "return") {
        tokens.push_back(TOKEN_RETURN);
      } else {
        PANIC("Unrecognized token '" << buf << "'");
      }
    }
  }

  return std::vector<token::Token>();
}

int main(i32 argc, char* argv[]) {
  if (argc != 2) {
    PANIC("Incorrect Usage" << std::endl << argv[0] << " \033[40;37m<input.diol>");
  }

  std::string buf = fs::read_file(argv[1]);
  tokenize(buf);

  return EXIT_SUCCESS;
}
