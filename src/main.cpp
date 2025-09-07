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

i32 main(i32 argc, char *argv[]) {
  if (argc != 2) {
    PANIC("Incorrect Usage" << std::endl
                            << argv[0] << " \033[40;37m<input.diol>");
  }

  std::string buf = fs::read_file(argv[1]);
  Tokenizer tokenizer = Tokenizer(buf);

  auto tokens = tokenizer.tokenize();
  Parser parser(tokens);

  Node::Program tree = parser.parse_program();
  Generator generator(tree);

  {
    std::fstream file("out.asm", std::ios::out);
    file << generator.gen_program();
  }

  system("nasm -felf64 out.asm");
  system("ld -o out.exe out.o");

  return EXIT_SUCCESS;
}
