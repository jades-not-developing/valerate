#pragma once

#include <fstream>

#include "fs.hpp"

std::string fs::read_file(const char* path) {
  std::stringstream buf;
  {
    std::fstream input(path, std::ios::in);
    buf << input.rdbuf();
  }

  std::string str = buf.str();

  return str;
}
