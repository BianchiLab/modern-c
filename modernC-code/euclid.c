#include "euclid.h"

size_t gcd(size_t a, size_t b) [[__unsequenced__]];

int main(int argc, char* argv[argc+1]) {
  size_t prev = 0;
  for (int i = 1; i < argc; ++i) {        // Processes args
    size_t act = strtoull(argv[i], nullptr, 0); // arg -> unsigned long long
    if (prev)
      printf("gcd(%zu, %zu) is %zu\n",
             prev, act, gcd(prev, act));
    prev = act;
  }
  return EXIT_SUCCESS;
}
