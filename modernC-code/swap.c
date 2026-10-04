#include "c23-fallback.h"
#include <stdio.h>

#define static_assert_compatible(A, B, REASON)  \
  static_assert(_Generic((typeof(A)*)nullptr,   \
                         typeof(B)*: true,      \
                         default: false),       \
                "expected compatible types: " REASON ", have " #A " and " #B "")

#ifndef __GNUC__
#define SWAP(X, Y)                                              \
  do {                                                          \
    /* These two variables have the role of function            \
       parameters. They ensure to evaluate the                  \
       expression X and Y once, since these could be            \
       complicated lvalue expressions with evaluated            \
       subexpressions that have side effects. */                \
    auto const swap_p1 = &(X);                                  \
    auto const swap_p2 = &(Y);                                  \
    static_assert_compatible(*swap_p1, *swap_p2,                \
                             "to exchange values, '"            \
                             #X "' and '" #Y                    \
                             "' must have compatible types");   \
    auto swap_tmp = *swap_p1;                                   \
    *swap_p1 = *swap_p2;                                        \
    *swap_p2 = swap_tmp;                                        \
  } while (false)
#else
#define SWAP(X, Y)                                              \
  /* This starts the compound expression construct. */          \
  ({                                                            \
    /* These two variables have the role of function            \
       parameters. They ensure to evaluate the                  \
       expression X and Y once, since these could be            \
       complicated lvalue expressions with evaluated            \
       subexpressions that have side effects. */                \
    auto const swap_p1 = &(X);                                  \
    auto const swap_p2 = &(Y);                                  \
    static_assert_compatible(*swap_p1, *swap_p2,                \
                             "to exchange values, '"            \
                             #X "' and '" #Y                    \
                             "' must have compatible types");   \
    auto swap_tmp = *swap_p1;                                   \
    *swap_p1 = *swap_p2;                                        \
    *swap_p2 = swap_tmp;                                        \
    /* ensure that the type of the expression is void */        \
    (void)0;                                                    \
  })
#endif


#define isnegative(X) ((X) < 0)

#define MAX_EQSIGN(X, Y)                                \
  /* This starts the compound expression construct. */  \
  ({                                                    \
    /* These two captures play the role of function     \
       parameters and read X and Y ... */               \
    auto const max_x = (X);                             \
    auto const max_y = (Y);                             \
    /* now the body starts */                           \
    /* types need to have the same signedness */        \
    (max_x < max_y) ? max_y : max_x;                    \
  })

#define MAX(X, Y)                                       \
  /* This starts the compound expression construct. */  \
  ({                                                    \
    auto const max_x = (X);                             \
    auto const max_y = (Y);                             \
    /* now the body starts */                           \
    ((isnegative(max_x) && !isnegative(max_y))          \
     ? max_y                                            \
     : ((isnegative(max_y) && !isnegative(max_x))       \
        ? max_x                                         \
        : /* both have the same sign  */                \
        ((max_x < max_y) ? max_y : max_x)));            \
  })



#define mincharacteristic(X, Y) ((issigned(X)<<2)|(issigned(Y)<<1)|1)

#define minunsigned(X, Y) (_Generic(tozero(X)+tozero(Y), typeof(X): tozero(Y), default: tozero(X)))

#define minreturn(X, Y)                                 \
  _Generic(                                             \
           (char(*)[mincharacteristic(X, Y)]){ 0 },     \
           /* both signed, arithmetic conversion */     \
           char(*)[4|2|1]: tozero(X)+tozero(Y),         \
           /* only one signed, use it */                \
           char(*)[0|2|1]: tozero(Y),                   \
           char(*)[4|0|1]: tozero(X),                   \
           /* both unsigned, use narrower */            \
           char(*)[0|0|1]: minunsigned(X, Y)            \
)

#define MIN(X, Y)                                                       \
  /* This starts the compound expression construct. */                  \
  ({                                                                    \
    /* These two captures play the role of function                     \
       parameters and read X and Y ... */                               \
    auto const min_x = (X);                                             \
    auto const min_y = (Y);                                             \
    /* now the body starts */                                           \
    typedef typeof(minreturn(min_x, min_y)) min_type;                   \
    ((isnegative(min_x) && !issigned(min_y))                            \
     ? (min_type)min_x                                                  \
     : ((isnegative(min_y) && !issigned(min_x))                         \
        ? (min_type)min_y                                               \
        : (                                                             \
           /* both have the same signedness or are both positive */     \
           (min_x < min_y)                                              \
           ? (min_type)min_x                                            \
           : (min_type)min_y)));                                        \
  })


int main (int argc, char* argv[static argc + 1]) {
  size_t i = argc;
  size_t j = argc+1;
  printf("%s, %s\n", argv[1], argv[2]);
  // Works even with side effects, but                     don't do that
  SWAP(argv[1+(i++)%2], argv[1+(j++)%2]);               // don't do that
  printf("%s, %s\n", argv[1], argv[2]);
  // Works even with side effects, but                     don't do that
  SWAP(argv[1+(i++)%2], argv[1+(j++)%2]);               // don't do that
  printf("%s, %s\n", argv[1], argv[2]);
  printf("%d\t%d\t→\t%d\n", -1, -2, MIN(-1, -2));
  printf("%d\t%ldl\t→\t%ldl\n", -2, -1L, MIN(-2, -1L));
  printf("%d\t%d\t→\t%d\n", -1, 2, MIN(-1, 2));
  printf("%d\t%ldl\t→\t%ldl\n", -1, 2L, MIN(-1, 2L));
  printf("%d\t%uu\t→\t%d\n", -1, 2u, MIN(-1, 2u));
  printf("%d\t%luul\t→\t%d\n", -1, 2UL, MIN(-1, 2UL));
  printf("%uu\t%uu\t→\t%uu\n", -1u, -2u, MIN(-1u, -2u));
  printf("%uu\t%luul\t→\t%uu\n", -1u, -2UL, MIN(-1u, -2UL));
  static_assert_compatible(1, 0, "both int");
  //static_assert_compatible(1, 0L, "both int");
  //static_assert_compatible(1L, 0, "both int");
  static_assert_compatible(1L, 0L, "both int");
  enum A : unsigned long { A = -1UL, };
  static_assert_compatible(A, 0UL, "both int");
  static_assert_compatible(int[1], int[true], "both int");
}
