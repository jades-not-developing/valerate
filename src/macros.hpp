#pragma once

#define ERROR_RED "\033[31;40m"

#define COLOR_RESET "\033[39;49m"

#define PANIC(x) std::cerr << ERROR_RED << "Error : " << x << COLOR_RESET << std::endl; \
                 exit(1);
