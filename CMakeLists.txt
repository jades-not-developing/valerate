cmake_minimum_required(VERSION 3.20)

project(Valerate)

add_executable(vcc 
  src/main.cpp
  src/fs.cpp
  src/token.cpp
  src/parser.cpp
  src/generation.cpp
)

target_include_directories(vcc PRIVATE include)

if (WIN32)
  target_compile_options(vcc PRIVATE /we4062)
endif (WIN32)

if (MSVC)
  target_compile_options(vcc PRIVATE /we4062)
else()
  target_compile_options(vcc PRIVATE -Wswitch-enum)
endif()

set_property(TARGET vcc PROPERTY CXX_STANDARD 23)
