#include "c23-fallback.h"
#include <math.h>
#include <stdio.h>

#if __is_identifier(constexpr)
# error "constexpr is needed for this test"
// really stop it
# include "really stop useless compilation right now"
#endif


#define eatit(P) printf("" #P ": %p\n", &(P))

[[__maybe_unused__]] static constexpr size_t size_max = (size_t)-1;

[[__maybe_unused__]] constexpr long double π = 3.141'592'653'589'793'238'46;
[[__maybe_unused__]] constexpr long double π² = π*π;

[[__maybe_unused__]] constexpr div_t one = { .quot = 1, .rem = 0, };

[[__maybe_unused__]] constexpr double E𝟏[2][2] = { {1, 0,}, {0, 1,}, };

int main(void) {
  eatit(size_max);
  eatit(π);
  eatit(π²);
  eatit(one);
  eatit(E𝟏);
}
