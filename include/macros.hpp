#pragma once

#include <iostream>

#define ERROR_RED "\033[31;40m"

#define COLOR_RESET "\033[39;49m"

#ifdef VALERATE_DEBUG
#define PANIC(x)                                                               \
  {                                                                            \
    std::cerr << ERROR_RED << "Panic (" << __FILE__ << ":" << __LINE__         \
              << ") : " << COLOR_RESET << "\r\n"                               \
              << ERROR_RED << x << COLOR_RESET << std::endl;                 \
    exit(EXIT_FAILURE);                                                        \
  }
#else
#define PANIC(x)                                                               \
  {                                                                            \
    std::cerr << ERROR_RED << "Panic : " << x << COLOR_RESET << std::endl;     \
    exit(EXIT_FAILURE);                                                        \
  }
#endif
