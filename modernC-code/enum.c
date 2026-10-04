#include "c23-fallback.h"
#include <stdio.h>

typedef enum echar echar;
enum echar {
  mchar = UCHAR_MAX,
};

typedef enum eshort eshort;
enum eshort {
  mshort = USHRT_MAX,
};

typedef enum eint eint;
enum eint {
  mint = UINT_MAX,
};

typedef enum elong elong;
enum elong {
  mlong = ULONG_MAX,
};

typedef enum ellong ellong;
enum ellong {
  mllong = ULLONG_MAX,
};

#ifdef UINT128_MAX
typedef enum e128 e128;
enum e128 {
  m128 = UINT128_MAX,
};
#endif

#if __has_feature(c_fixed_enum) || (__clang_major__ > 7)

enum fchar : unsigned char {
  nchar = UCHAR_MAX,
};
typedef enum fchar fchar;

enum fshort : unsigned short {
  nshort = USHRT_MAX,
};
typedef enum fshort fshort;

enum fint : unsigned {
  nint = UINT_MAX,
};
typedef enum fint fint;

enum flong : unsigned long {
  nlong = ULONG_MAX,
};
typedef enum flong flong;

enum fllong : unsigned long long {
  nllong = ULLONG_MAX,
};
typedef enum fllong fllong;

#ifdef UINT128_MAX
enum f128 : uint128_t {
  n128 = UINT128_MAX,
};
typedef enum f128 f128;
#endif
#else
# warning "test for fixed underlying integer type skipped"
#endif

int main(void) {
  printf("echar:\t%zu\t%zu\n", sizeof(echar), sizeof(mchar));
  printf("eshort:\t%zu\t%zu\n", sizeof(eshort), sizeof(mshort));
  printf("eint:\t%zu\t%zu\n", sizeof(eint), sizeof(mint));
  printf("elong:\t%zu\t%zu\n", sizeof(elong), sizeof(mlong));
  printf("ellong:\t%zu\t%zu\n", sizeof(ellong), sizeof(mllong));
#ifdef UINT128_MAX
  printf("e128:\t%zu\t%zu\n", sizeof(e128), sizeof(m128));
#endif

#if __has_feature(c_fixed_enum) || (__clang_major__ > 7)
  printf("fchar:\t%zu\t%zu\n", sizeof(fchar), sizeof(nchar));
  printf("fshort:\t%zu\t%zu\n", sizeof(fshort), sizeof(nshort));
  printf("fint:\t%zu\t%zu\n", sizeof(fint), sizeof(nint));
  printf("flong:\t%zu\t%zu\n", sizeof(flong), sizeof(nlong));
  printf("fllong:\t%zu\t%zu\n", sizeof(fllong), sizeof(nllong));
# ifdef UINT128_MAX
  printf("f128:\t%zu\t%zu\n", sizeof(f128), sizeof(n128));
# endif
#endif
}
