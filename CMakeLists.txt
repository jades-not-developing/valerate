cmake_minimum_required(VERSION 3.20)

project(CCompiler)


add_executable(c_compiler 
  src/main.cpp
  src/fs.cpp
  src/token.cpp
  src/tokenizer.cpp)

target_include_directories(c_compiler PRIVATE include)

if (WIN32)
  target_compile_options(c_compiler PRIVATE /we4062)
endif (WIN32)

if (MSVC)
  target_compile_options(c_compiler PRIVATE /we4062)
else()
  target_compile_options(c_compiler PRIVATE -Wswitch-enum)
endif()

set_property(TARGET c_compiler PROPERTY CXX_STANDARD 23)
