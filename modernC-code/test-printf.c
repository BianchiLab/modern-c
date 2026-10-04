#include "c23-fallback.h"
#include <stdio.h>
#include <inttypes.h>
#include <errno.h>

#ifndef PRIb8
# error "library support of binary number representations for input and output are required for this test"
// really stop it
# include "really stop useless compilation right now"
#endif

#define PS(S, B)                                        \
do {                                                    \
  errno = 0;                                            \
  unsigned long long res = strtoull(S, nullptr, B);     \
  if (errno) {                                          \
    perror("strtoull error " S);                        \
    errno = 0;                                          \
  } else {                                              \
    printf("base %d,\t\"%s\" →\t%llu\n", B, S, res);    \
    if (errno) perror("printf error");                  \
    errno = 0;                                          \
  }                                                     \
  res = wcstoull(L ## S, nullptr, B);                   \
  if (errno) {                                          \
    perror("wcstoull error " S);                        \
    errno = 0;                                          \
  } else {                                              \
    printf("base %d,\tL\"%s\" →\t%llu\n", B, S, res);   \
    if (errno) perror("printf error");                  \
    errno = 0;                                          \
  }                                                     \
 } while (false)

#define PSCAN(F, S, X)                                          \
do {                                                            \
  sscanf(S, F, &(X));                                           \
  if (errno) {                                                  \
    perror("sscanf error \"" S "\" format \"" F "\"");          \
    errno = 0;                                                  \
  } else {                                                      \
    printf("%s: \"%s\" → " F "\n", F, S, X/*+0ULL*/);           \
    if (errno) perror("printf error " F);                       \
    errno = 0;                                                  \
  }                                                             \
  swscanf(L ## S, L ## F, &(X));                                \
  if (errno) {                                                  \
    perror("swscanf error L\"" S "\" format L\"" F "\"");       \
    errno = 0;                                                  \
  } else {                                                      \
    printf("%s: L\"%s\" → " F "\n", F, S, X/*+0ULL*/);          \
    if (errno) perror("printf error " F);                       \
    errno = 0;                                                  \
  }                                                             \
 } while(false)

int main(int argc, char* argv[static argc+1]) {
  printf("%b\n", 35u);
  printf("%#b\n", 35u);
  printf("%B\n", 35u);
  printf("%#B\n", 35u);
  if (argc > 1) {
    unsigned long b = strtoul(argv[1], nullptr, 0);
    printf("%lb\n", b);
    printf("%#lb\n", b);
    printf("%lB\n", b);
    printf("%#lB\n", b);
  }
  perror("printf error"); errno = 0;
  uint8_t u8 = -1;
  uint16_t u16 = -1;
  uint32_t u32 = -1;
  uint64_t u64 = -1;
  printf("PRIb8:\t%" PRIb8 "\n", u8);
  printf("PRIb16:\t%" PRIb16 "\n", u16);
  printf("PRIb32:\t%" PRIb32 "\n", u32);
  printf("PRIb64:\t%" PRIb64 "\n", u64);
  printf("PRIB8:\t%" PRIB8 "\n", u8);
  printf("PRIB16:\t%" PRIB16 "\n", u16);
  printf("PRIB32:\t%" PRIB32 "\n", u32);
  printf("PRIB64:\t%" PRIB64 "\n", u64);
  printf("PRIb8:\t%#" PRIb8 "\n", u8);
  printf("PRIb16:\t%#" PRIb16 "\n", u16);
  printf("PRIb32:\t%#" PRIb32 "\n", u32);
  printf("PRIb64:\t%#" PRIb64 "\n", u64);
  printf("PRIB8:\t%#" PRIB8 "\n", u8);
  printf("PRIB16:\t%#" PRIB16 "\n", u16);
  printf("PRIB32:\t%#" PRIB32 "\n", u32);
  printf("PRIB64:\t%#" PRIB64 "\n", u64);
  perror("printf error"); errno = 0;
#if __STDC_VERSION_INTTYPES_H__ < 201800L
#  ifndef C23_FALLBACK_SILENT
#   warning "no C23 printf support found, skipping tests for %w length modifiers"
#  endif
#else
  printf("11:\t%w11x\n", u8);
  perror("printf error"); errno = 0;
  printf("8:\t%w8x\n", u8);
  printf("16:\t%w16x\n", u16);
  printf("32:\t%w32x\n", u32);
  printf("64:\t%w64x\n", u64);
  perror("printf error"); errno = 0;
  printf("11:\t%w11x\n", u8);
  perror("printf error"); errno = 0;
  printf("8:\t%w8u\n", u8);
  printf("16:\t%w16u\n", u16);
  printf("32:\t%w32u\n", u32);
  printf("64:\t%w64u\n", u64);
  perror("printf error"); errno = 0;
  printf("11:\t%w11x\n", u8);
  perror("printf error"); errno = 0;
  printf("8:\t%w8d\n", u8);
  printf("16:\t%w16d\n", u16);
  printf("32:\t%w32d\n", u32);
  printf("64:\t%w64d\n", u64);
  perror("printf error"); errno = 0;
  int8_t i8 = INT8_MAX;
  int16_t i16 = INT16_MAX;
  int32_t i32 = INT32_MAX;
  int64_t i64 = INT64_MAX;
  printf("11:\t%w11x\n", i8);
  perror("printf error"); errno = 0;
  printf("8:\t%w8x\n", i8);
  printf("16:\t%w16x\n", i16);
  printf("32:\t%w32x\n", i32);
  printf("64:\t%w64x\n", i64);
  perror("printf error"); errno = 0;
  printf("11:\t%w11x\n", i8);
  perror("printf error"); errno = 0;
  printf("8:\t%w8u\n", i8);
  printf("16:\t%w16u\n", i16);
  printf("32:\t%w32u\n", i32);
  printf("64:\t%w64u\n", i64);
  perror("printf error"); errno = 0;
  printf("11:\t%w11x\n", i8);
  perror("printf error"); errno = 0;
  printf("8:\t%w8d\n", i8);
  printf("16:\t%w16d\n", i16);
  printf("32:\t%w32d\n", i32);
  printf("64:\t%w64d\n", i64);
  perror("printf error"); errno = 0;
  uint_fast8_t uf8 = -1;
  uint_fast16_t uf16 = -1;
  uint_fast32_t uf32 = -1;
  uint_fast64_t uf64 = -1;
  printf("11 fast:\t%wf11x\n", uf8);
  perror("printf error"); errno = 0;
  printf("8 fast:\t%wf8x\n", uf8);
  printf("16 fast:\t%wf16x\n", uf16);
  printf("32 fast:\t%wf32x\n", uf32);
  printf("64 fast:\t%wf64x\n", uf64);
  perror("printf error"); errno = 0;
  printf("11 fast:\t%wf11x\n", uf8);
  perror("printf error"); errno = 0;
  printf("8 fast:\t%wf8u\n", uf8);
  printf("16 fast:\t%wf16u\n", uf16);
  printf("32 fast:\t%wf32u\n", uf32);
  printf("64 fast:\t%wf64u\n", uf64);
  perror("printf error"); errno = 0;
  printf("11 fast:\t%wf11x\n", uf8);
  perror("printf error"); errno = 0;
  printf("8 fast:\t%wf8d\n", uf8);
  printf("16 fast:\t%wf16d\n", uf16);
  printf("32 fast:\t%wf32d\n", uf32);
  printf("64 fast:\t%wf64d\n", uf64);
  perror("printf error"); errno = 0;
  int_fast8_t if8 = INT_FAST8_MAX;
  int_fast16_t if16 = INT_FAST16_MAX;
  int_fast32_t if32 = INT_FAST32_MAX;
  int_fast64_t if64 = INT_FAST64_MAX;
  printf("11 fast:\t%wf11x\n", if8);
  perror("printf error"); errno = 0;
  printf("8 fast:\t%wf8x\n", if8);
  printf("16 fast:\t%wf16x\n", if16);
  printf("32 fast:\t%wf32x\n", if32);
  printf("64 fast:\t%wf64x\n", if64);
  perror("printf error"); errno = 0;
  printf("11 fast:\t%wf11x\n", if8);
  perror("printf error"); errno = 0;
  printf("8 fast:\t%wf8u\n", if8);
  printf("16 fast:\t%wf16u\n", if16);
  printf("32 fast:\t%wf32u\n", if32);
  printf("64 fast:\t%wf64u\n", if64);
  perror("printf error"); errno = 0;
  printf("11 fast:\t%wf11x\n", if8);
  perror("printf error"); errno = 0;
  printf("8 fast:\t%wf8d\n", if8);
  printf("16 fast:\t%wf16d\n", if16);
  printf("32 fast:\t%wf32d\n", if32);
  printf("64 fast:\t%wf64d\n", if64);
  perror("printf error"); errno = 0;
#if UINT128_WIDTH || __SIZEOF_INT128__
#if UINT128_WIDTH
  puts("------------ found standard uint128_t -------------");
  uint128_t u128 = -1;
  int128_t i128 = u128/2;
#elif __SIZEOF_INT128__
  puts("------------ found non-standard __uint128_t -------------");
  __uint128_t u128 = -1;
  __int128_t i128 = u128/2;
#endif
  printf("128u:\t%w128u\n", (__uint128_t)0);
  printf("128u:\t%w128u\n", u128);
  printf("128d:\t%w128d\n", u128);
  printf("128d:\t%w128u\n", (__int128_t)0);
  printf("128x:\t%w128x\n", i128);
  printf("128d:\t%w128d\n", i128);
  printf("128b:\t%#w128b\n", i128);
#endif
  perror("printf error"); errno = 0;
#endif
  puts("-------------- scanf functions ------------");
  PS("2147483647", 0);
  PS("2147483647", 2);
  PS("2147483647", 8);
  PS("2147483647", 10);
  PS("2147483647", 16);
  PS("-1", 0);
  PS("-1", 2);
  PS("-1", 8);
  PS("-1", 10);
  PS("-1", 16);
  PS("0b1", 0);
  PS("0b1", 2);
  PS("0b1", 8);
  PS("0b1", 10);
  PS("0b1", 16);
  PS("0B1", 0);
  PS("0B1", 2);
  PS("0B1", 8);
  PS("0B1", 10);
  PS("0B1", 16);
  PS("0x1", 0);
  PS("0x1", 2);
  PS("0x1", 8);
  PS("0x1", 10);
  PS("0x1", 16);
  PS("0b1111111111111111111111111111111111111111111111111111111111111111", 0);
  PS("0b1111111111111111111111111111111111111111111111111111111111111111", 2);
  PS("0b1111111111111111111111111111111111111111111111111111111111111111", 8);
  PS("0b1111111111111111111111111111111111111111111111111111111111111111", 10);
  PS("0b1111111111111111111111111111111111111111111111111111111111111111", 16);
  PS("0B1111111111111111111111111111111111111111111111111111111111111111", 0);
  PS("0B1111111111111111111111111111111111111111111111111111111111111111", 2);
  PS("0B1111111111111111111111111111111111111111111111111111111111111111", 8);
  PS("0B1111111111111111111111111111111111111111111111111111111111111111", 10);
  PS("0B1111111111111111111111111111111111111111111111111111111111111111", 16);
  PS("0xffffffffffffffff", 0);
  PS("0xffffffffffffffff", 2);
  PS("0xffffffffffffffff", 8);
  PS("0xffffffffffffffff", 10);
  PS("0xffffffffffffffff", 16);
  PSCAN("%" SCNb8, "0b1", u8);
  PSCAN("%" SCNu8, "0b1", u8);
  PSCAN("%" SCNx8, "0b1", u8);
  PSCAN("%" SCNb8, "0x1", u8);
  PSCAN("%" SCNu8, "0x1", u8);
  PSCAN("%" SCNx8, "0x1", u8);
  PSCAN("%" SCNb16, "0b1", u16);
  PSCAN("%" SCNu16, "0b1", u16);
  PSCAN("%" SCNx16, "0b1", u16);
  PSCAN("%" SCNb16, "0x1", u16);
  PSCAN("%" SCNu16, "0x1", u16);
  PSCAN("%" SCNx16, "0x1", u16);
  PSCAN("%" SCNb32, "0b1", u32);
  PSCAN("%" SCNu32, "0b1", u32);
  PSCAN("%" SCNx32, "0b1", u32);
  PSCAN("%" SCNb32, "0x1", u32);
  PSCAN("%" SCNu32, "0x1", u32);
  PSCAN("%" SCNx32, "0x1", u32);
  PSCAN("%" SCNb64, "0b1", u64);
  PSCAN("%" SCNu64, "0b1", u64);
  PSCAN("%" SCNx64, "0b1", u64);
  PSCAN("%" SCNb64, "0x1", u64);
  PSCAN("%" SCNu64, "0x1", u64);
  PSCAN("%" SCNx64, "0x1", u64);
#if __STDC_VERSION_INTTYPES_H__ < 201800L
#  ifndef C23_FALLBACK_SILENT
#   warning "no C23 scanf support found, skipping tests for %w length modifiers"
#  endif
#else
  PSCAN("%w8b", "0b1", u8);
  PSCAN("%w8u", "0b1", u8);
  PSCAN("%w8x", "0b1", u8);
  PSCAN("%w8b", "0x1", u8);
  PSCAN("%w8u", "0x1", u8);
  PSCAN("%w8x", "0x1", u8);
  PSCAN("%w16b", "0b1", u16);
  PSCAN("%w16u", "0b1", u16);
  PSCAN("%w16x", "0b1", u16);
  PSCAN("%w16b", "0x1", u16);
  PSCAN("%w16u", "0x1", u16);
  PSCAN("%w16x", "0x1", u16);
  PSCAN("%w32b", "0b1", u32);
  PSCAN("%w32u", "0b1", u32);
  PSCAN("%w32x", "0b1", u32);
  PSCAN("%w32b", "0x1", u32);
  PSCAN("%w32u", "0x1", u32);
  PSCAN("%w32x", "0x1", u32);
  PSCAN("%w64b", "0b1", u64);
  PSCAN("%w64u", "0b1", u64);
  PSCAN("%w64x", "0b1", u64);
  PSCAN("%w64b", "0x1", u64);
  PSCAN("%w64u", "0x1", u64);
  PSCAN("%w64x", "0x1", u64);
#if UINT128_WIDTH || __SIZEOF_INT128__
  PSCAN("%w128b", "0b1", u128);
  PSCAN("%w128u", "0b1", u128);
  PSCAN("%w128x", "0b1", u128);
  PSCAN("%w128b", "0x1", u128);
  PSCAN("%w128u", "0x1", u128);
  PSCAN("%w128x", "0x1", u128);

  PSCAN("%w128b", "0b11111111111111111111111111111111111111111111111111111111111111111", u128);
  PSCAN("%w128u", "0b111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111", u128);
  PSCAN("%w128x", "0b111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111", u128);
  PSCAN("%w128b", "0x7ffffffffffffffffffffffffffffffe", u128);
  PSCAN("%w128u", "0x7ffffffffffffffffffffffffffffffe", u128);
  PSCAN("%w128x", "0x7ffffffffffffffffffffffffffffffe", u128);

  PSCAN("%w128b", "0b1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111", u128);
  PSCAN("%w128u", "0b1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111", u128);
  PSCAN("%w128x", "0b1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111", u128);
  PSCAN("%w128b", "0x7fffffffffffffffffffffffffffffff", u128);
  PSCAN("%w128u", "0x7fffffffffffffffffffffffffffffff", u128);
  PSCAN("%w128x", "0x7fffffffffffffffffffffffffffffff", u128);
  PSCAN("%w128b", "0xffffffffffffffffffffffffffffffff", u128);
  PSCAN("%w128u", "0xffffffffffffffffffffffffffffffff", u128);
  PSCAN("%w128x", "0xffffffffffffffffffffffffffffffff", u128);
#ifdef SCNb128
  puts("-------------- SCN macros ------------");
  PSCAN("%" SCNb128, "0b1", u128);
  PSCAN("%" SCNu128, "0b1", u128);
  PSCAN("%" SCNx128, "0b1", u128);
  PSCAN("%" SCNb128, "0x1", u128);
  PSCAN("%" SCNu128, "0x1", u128);
  PSCAN("%" SCNx128, "0x1", u128);
#endif
#endif
#endif
}
