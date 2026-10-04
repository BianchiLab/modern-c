#include "c23-fallback.h"
#include <stdarg.h>
#include <stdio.h>
//#include <stdlib.h>

/**
 ** @brief A small, useless function to show how variadic
 ** functions work
 **/
double sumIt(size_t n, ...) {
  double ret = 0.0;
  va_list va;
  va_start(va);
  for (size_t i = 0; i < n; ++i)
    ret += va_arg(va, double);
  va_end(va);
  return ret;
}

/**
 ** @brief A simple debug stream
 **
 ** Per convention no output is produced when this is null.
 **
 ** This can be set local to the current thread, such that threads may
 ** print their debug messages to different files or streams.
 **/
thread_local FILE* iodebug = nullptr;

/**
 ** @brief Prints to the debug stream @c iodebug
 **/
[[gnu::format(printf, 1, 2)]]
int printf_debug(const char *format, ...) {
  int ret = 0;
  if (iodebug) {
    va_list va;
    va_start(va);
    ret = vfprintf(iodebug, format, va);
    va_end(va);
  }
  return ret;
}



int main(int argc, char* argv[argc+1]) {
  if (argc < 4) return EXIT_FAILURE;
  iodebug = stderr;
  printf_debug("%g\n", sumIt(3, strtod(argv[1], nullptr), strtod(argv[2], nullptr), strtod(argv[3], nullptr)));
  return EXIT_SUCCESS;
}
