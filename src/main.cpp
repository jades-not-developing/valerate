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
  std::cout << "hi" << std::endl;
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

  system("nasm -f win64 -g out.asm");
  system("link /SUBSYSTEM:WINDOWS msvc/kernel32.lib msvc/user32.lib out.obj");

  return EXIT_SUCCESS;
}
