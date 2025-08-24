cmake_minimum_required(VERSION 3.20)

project(CCompiler)


add_executable(c_compiler 
  src/main.cpp
  src/fs.cpp
  src/token.cpp)

target_compile_options(c_compiler PRIVATE /we4062)

set_property(TARGET c_compiler PROPERTY CXX_STANDARD 23)
