#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#include "fs.hpp"
#include "macros.hpp"
#include "token.hpp"
#include "parser.hpp"
#include "generation.hpp"
#include "types.h"


using namespace token;

//std::string assemble(const std::vector<Token> &tokens) {
//  std::stringstream output;
//  output << "global _start\n_start:\n";
//  for (i32 i = 0; i < tokens.size(); ++i) {
//    const Token &t = tokens.at(i);
//    if (t.type == TokenType::exit) {
//      if (i + 1 < tokens.size() &&
//          tokens.at(i + 1).type == TokenType::int_lit) {
//        if (i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::semi) {
//          output << "    mov eax, " << tokens.at(i + 1).value.value() << "\n";
//          output << "    ret\n";
//        }
//      }
//    }
//  }
//
//  return output.str();
//}

i32 main(i32 argc, char *argv[]) {
  if (argc != 2) {
    PANIC("Incorrect Usage" << std::endl
                            << argv[0] << " \033[40;37m<input.diol>");
  }

  std::string buf = fs::read_file(argv[1]);
  Tokenizer tokenizer = Tokenizer(buf);

  auto tokens = tokenizer.tokenize();
  Parser parser(tokens);

  std::optional<Node::Exit> tree = parser.parse();
  if (!tree.has_value()) {
    PANIC("No exit statement found");
  }

  Generator generator(tree.value());

  {
    std::fstream file("out.asm", std::ios::out);
    file << generator.generate();
  }

  system("nasm -felf64 out.asm");
  system("ld -o out.exe out.o");

  return EXIT_SUCCESS;
}
