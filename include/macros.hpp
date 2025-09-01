#pragma once

#include <iostream>

#define ERROR_RED "\033[31;40m"

#define COLOR_RESET "\033[39;49m"


//#define PANIC(code, x) fprintf(stderr, ERROR_RED"Error : %s\n", COLOR_RESET); \
//                       exit(code);

#define PANIC(x) {std::cerr << ERROR_RED << "Error : " << x << COLOR_RESET << std::endl; \
                       exit(EXIT_FAILURE);}
