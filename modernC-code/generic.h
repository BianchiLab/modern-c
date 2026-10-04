#ifndef GENERIC_H
#define GENERIC_H 1

#include <float.h>
#include <stdio.h>
#include "c23-fallback.h"

#if !__has_va_opt
# error "no __VA_OPT__ support found, aborting compilation"
// shut off some subsequent error messages
# include "no __VA_OPT__ support found, aborting compilation"
#endif

/**
 ** @brief Type-generic minimum for floating-point values
 **/
#define min(A, B)                               \
_Generic((A)+(B),                               \
         float: minf,                           \
         long double: minl,                     \
         default: mind)((A), (B))

static inline
double mind(double a, double b) [[__unsequenced__]] {
  return a < b ? a : b;
}

static inline
long double minl(long double a, long double b) [[__unsequenced__]] {
  return a < b ? a : b;
}

static inline
float minf(float a, float b) [[__unsequenced__]] {
  return a < b ? a : b;
}

/**
 ** @brief The maximum value for the type of @a X
 **/
#define MAXVAL(X)                                       \
_Generic((X),                                           \
         bool: (bool)+1,                                \
         char: (char)+CHAR_MAX,                         \
         signed char: (signed char)+SCHAR_MAX,          \
         unsigned char: (unsigned char)+UCHAR_MAX,      \
         signed short: (signed short)+SHRT_MAX,         \
         unsigned short: (unsigned short)+USHRT_MAX,    \
         signed: INT_MAX,                               \
         unsigned: UINT_MAX,                            \
         signed long: LONG_MAX,                         \
         unsigned long: ULONG_MAX,                      \
         signed long long: LLONG_MAX,                   \
         unsigned long long: ULLONG_MAX,                \
         float: FLT_MAX,                                \
         double: DBL_MAX,                               \
         long double: LDBL_MAX)

/**
 ** @brief The maximum promoted value for @a XT, where @a XT
 ** can be an expression or a type name
 **
 ** So this is the maximum value when fed to an arithmetic
 ** operation such as @c +.
 **
 ** @remark Narrow types are promoted, usually to @c signed,
 ** or maybe to @c unsigned on rare architectures.
 **/
#define maxof(XT)                               \
_Generic(0+(XT)+0,                              \
         signed: INT_MAX,                       \
         unsigned: UINT_MAX,                    \
         signed long: LONG_MAX,                 \
         unsigned long: ULONG_MAX,              \
         signed long long: LLONG_MAX,           \
         unsigned long long: ULLONG_MAX,        \
         float: FLT_MAX,                        \
         double: DBL_MAX,                       \
         long double: LDBL_MAX)

#define MINVAL(X)                               \
_Generic((X),                                   \
         bool: (bool)+0,                        \
         char: (char)+CHAR_MIN,                 \
         signed char: (signed char)+SCHAR_MIN,  \
         unsigned char: (unsigned char)+0,      \
         signed short: (signed short)+SHRT_MIN, \
         unsigned short: (unsigned short)+0,    \
         signed: INT_MIN,                       \
         unsigned: 0U,                          \
         signed long: LONG_MIN,                 \
         unsigned long: 0UL,                    \
         signed long long: LLONG_MIN,           \
         unsigned long long: 0ULL,              \
         float: -FLT_MAX,                       \
         double: -DBL_MAX,                      \
         long double: -LDBL_MAX)

#define minof(XT)                               \
_Generic(0+(XT)+0,                              \
         signed: INT_MIN,                       \
         unsigned: 0U,                          \
         signed long: LONG_MIN,                 \
         unsigned long: 0UL,                    \
         signed long long: LLONG_MIN,           \
         unsigned long long: 0ULL,              \
         float: -FLT_MAX,                       \
         double: -DBL_MAX,                      \
         long double: -LDBL_MAX)

#define strtod(NPTR,  ...)                                      \
  STRTOD_I __VA_OPT__(I) (NPTR __VA_OPT__(,) __VA_ARGS__)
#define STRTOD_I(NPTR)          strtod(NPTR, nullptr)
#define STRTOD_II(NPTR, ENDPTR) strtod(NPTR, ENDPTR)
#define strtof(NPTR,  ...)                                      \
  STRTOF_I __VA_OPT__(I) (NPTR __VA_OPT__(,) __VA_ARGS__)
#define STRTOF_I(NPTR)          strtof(NPTR, nullptr)
#define STRTOF_II(NPTR, ENDPTR) strtof(NPTR, ENDPTR)
#define strtold(NPTR,  ...)                                     \
  STRTOLD_I __VA_OPT__(I) (NPTR __VA_OPT__(,) __VA_ARGS__)
#define STRTOLD_I(NPTR)          strtold(NPTR, nullptr)
#define STRTOLD_II(NPTR, ENDPTR) strtold(NPTR, ENDPTR)

/** @brief Output the second argument, if any, or `DEF0` as a default.
 **
 ** Error if more than two arguments are given.
 **/
#define DEFAULT1(DEF0, ...)                     \
  ID_I  ## __VA_OPT__(Iplus_DEFAULT)            \
  (DEF0 __VA_OPT__(,) __VA_ARGS__)
#define ID_IIplus_DEFAULT(_01, ...) ID_I(__VA_ARGS__)

#define ID_()
#define ID_I(_01)                     _01
#define ID_II(_01, _2)                _01, _2
#define ID_III(_01, _2, _3)           _01, _2, _3

/** @brief Output comma-separated arguments from the third position
 ** onward, fill with defaults from the first two.
 **
 ** Error if more than four arguments are given.
 **/
#define DEFAULT2(DEF0, DEF1, ...)                  \
  ID_II  ## __VA_OPT__(Iplus_DEFAULT)              \
  (DEF0, DEF1 __VA_OPT__(,) __VA_ARGS__)
#define ID_IIIplus_DEFAULT(DEF0, DEF1, _01, ...)   \
  DEFAULT1(DEF0, _01), DEFAULT1(DEF1, __VA_ARGS__)

/** @brief Output comma-separated arguments from the fourth position
 ** onward, fill with defaults from the first three.
 **
 ** Error if more than six arguments are given.
 **/
#define DEFAULT3(DEF0, DEF1, DEF2, ...)         \
  ID_III ## __VA_OPT__(Iplus_DEFAULT)           \
  (DEF0, DEF1, DEF2 __VA_OPT__(,) __VA_ARGS__)
#define ID_IIIIplus_DEFAULT(DEF0, DEF1, DEF2, _01, ...) \
  DEFAULT1(DEF0, _01), DEFAULT2(DEF1, DEF2, __VA_ARGS__)

#define CALL1(FUNC, DEF0, ...)                  \
  FUNC(DEFAULT1(DEF0, __VA_ARGS__))
#define CALL2(FUNC, DEF0, DEF1, ...)            \
  FUNC(DEFAULT2(DEF0, DEF1, __VA_ARGS__))
#define CALL3(FUNC, DEF0, DEF1, DEF2, ...)      \
  FUNC(DEFAULT3(DEF0, DEF1, DEF2, __VA_ARGS__))

#define strtoul(...)                            \
  CALL3(strtoul,  "0", nullptr, 0, __VA_ARGS__)
#define strtoull(...)                           \
  CALL3(strtoull, "0", nullptr, 0, __VA_ARGS__)
#define strtol(...)                             \
  CALL3(strtol,   "0", nullptr, 0, __VA_ARGS__)
#define strtoll(...)                            \
  CALL3(strtoll,  "0", nullptr, 0, __VA_ARGS__)

/**
 ** @brief Calls a three-parameter function with default arguments set to
 ** specific values
 **
 ** @see strtoull10 for an example
 **/
#define strtoul10(NPTR, ...)  CALL3((strtoul),  "0", nullptr, 10, NPTR __VA_OPT__(,) __VA_ARGS__)
#define strtoull10(NPTR, ...) CALL3((strtoull), "0", nullptr, 10, NPTR __VA_OPT__(,) __VA_ARGS__)
#define strtol10(NPTR, ...)   CALL3((strtol),   "0", nullptr, 10, NPTR __VA_OPT__(,) __VA_ARGS__)
#define strtoll10(NPTR, ...)  CALL3((strtoll),  "0", nullptr, 10, NPTR __VA_OPT__(,) __VA_ARGS__)

#define PROMOTE(XT, A)                                  \
  _Generic(0+(XT)+0,                                    \
           signed: (signed)(A),                         \
           unsigned: (unsigned)(A),                     \
           signed long: (signed long)(A),               \
           unsigned long: (unsigned long)(A),           \
           signed long long: (signed long long)(A),     \
           unsigned long long: (unsigned long long)(A), \
           float: (float)(A),                           \
           double: (double)(A),                         \
           long double: (long double)(A))

#define SIGNEDNESS(XT) (PROMOTE(XT, -1) < PROMOTE(XT, 0))

inline
signed maxs(signed a, signed b) [[__unsequenced__]] {
  return a < b ? b : a;
}

inline
unsigned maxu(unsigned a, unsigned b) [[__unsequenced__]] {
  return a < b ? b : a;
}

inline
unsigned maxus(unsigned a, signed b) [[__unsequenced__]] {
  return b < 0 ? a : maxu(a, b);
}

inline
unsigned maxsu(signed a, unsigned b) [[__unsequenced__]] {
  return a < 0 ? b : maxu(a, b);
}

inline
signed long maxsl(signed long a, signed long b) [[__unsequenced__]] {
  return a < b ? b : a;
}

inline
unsigned long maxul(unsigned long a, unsigned long b) [[__unsequenced__]] {
  return a < b ? b : a;
}

inline
unsigned long maxusl(unsigned long a, signed long b) [[__unsequenced__]] {
  return b < 0 ? a : maxul(a, b);
}

inline
unsigned long maxsul(signed long a, unsigned long b) [[__unsequenced__]] {
  return a < 0 ? b : maxul(a, b);
}

inline
signed long long maxsll(signed long long a, signed long long b) [[__unsequenced__]] {
  return a < b ? b : a;
}

inline
unsigned long long maxull(unsigned long long a, unsigned long long b) [[__unsequenced__]] {
  return a < b ? b : a;
}

inline
unsigned long long maxusll(unsigned long long a, signed long long b) [[__unsequenced__]] {
  return b < 0 ? a : maxull(a, b);
}

inline
unsigned long long maxsull(signed long long a, unsigned long long b) [[__unsequenced__]] {
  return a < 0 ? b : maxull(a, b);
}

#define  MAX(X, Y)                              \
_Generic((X)+(Y),                               \
         signed: maxs,                          \
         unsigned: maxu,                        \
         signed long: maxsl,                    \
         unsigned long: maxul,                  \
         signed long long: maxsll,              \
         unsigned long long: maxull)((X), (Y))

#define MAXUS(X, Y)                             \
_Generic((X)+(Y),                               \
         signed: maxus,                         \
         unsigned: maxus,                       \
         signed long: maxusl,                   \
         unsigned long: maxusl,                 \
         signed long long: maxusll,             \
         unsigned long long: maxusll)((X), (Y))

#define MAXSU(X, Y)                             \
_Generic((X)+(Y),                               \
         signed: maxsu,                         \
         unsigned: maxsu,                       \
         signed long: maxsul,                   \
         unsigned long: maxsul,                 \
         signed long long: maxsull,             \
         unsigned long long: maxsull)((X), (Y))

#define mix(X, Y)                                       \
_Generic(&(char[SIGNEDNESS(X)-SIGNEDNESS(Y)+2]){ },     \
         char(*)[1]: MAXUS((X), (Y)),                   \
         char(*)[2]: MAX((X), (Y)),                     \
         char(*)[3]: MAXSU((X), (Y)))

/**
 ** @brief Similar to `snprintf` but checks the buffer argument for
 ** the size.
 **
 ** To be able to do that check, the function has the size argument
 ** first and then the buffer, so the buffer can be specified as array
 ** parameter that depends on the that size.
 **/
[[maybe_unused, __gnu__::__format__(__printf__, 3, 4)]]
static inline
int snprintf_swapped(size_t n, char s[restrict static n],
                     char const* restrict form, ...) {
  va_list ap;
  va_start(ap);
  int ret = vsnprintf(s, n, form, ap);
  va_end(ap);
  return ret;
}

/**
 ** @brief A type-generic interface to `snprintf` that checks arguments.
 **
 ** We distinguish two cases. First if the first argument is `nullptr`
 ** the second argument is forced to `0` and the function `snprintf`
 ** is called. If it is not `nullptr` a function `snprintf_swapped`
 ** that just swaps the first two arguments is called. Because
 ** that function has the size argument first and then the buffer, the
 ** buffer can be specified as array parameter that depends on the
 ** that size.
 **/
#define snprintf(S, N, F, ...)                          \
_Generic                                                \
  ((S),                                                 \
   nullptr_t:                                           \
   (snprintf)(nullptr, GENERIC_IF(isice(N), (N), 0),    \
              F __VA_OPT__(,) __VA_ARGS__),             \
   default:                                             \
   snprintf_swapped(_Generic((S),                       \
                             nullptr_t: 1,              \
                             default: (N)),             \
                    _Generic((S),                       \
                             nullptr_t:                 \
                             (char[1]){ 0 },            \
                             default: (S)),             \
                    (F) __VA_OPT__(,) __VA_ARGS__))

#define pow(X, Y)                                               \
  _Generic                                                      \
  (                                                             \
   (void (*)(typeof((X)+(Y)+0ULL), typeof((Y)+0ULL)))nullptr,   \
   /* Second argument is an integer. */                         \
   void (*)(float,              unsigned long long): pownf,     \
   void (*)(double,             unsigned long long): pown,      \
   void (*)(unsigned long long, unsigned long long): pown,      \
   void (*)(long double,        unsigned long long): pownl,     \
   /* Second argument is a floating point. */                   \
   void (*)(float,                           float): powf,      \
   void (*)(long double,                     float): powl,      \
   void (*)(long double,                    double): powl,      \
   void (*)(long double,               long double): powl,      \
   /* Second argument is a floating-point, and first */         \
   /* is double or integer.                          */         \
   default:                                          pow)       \
  ((X), (Y))


#endif
