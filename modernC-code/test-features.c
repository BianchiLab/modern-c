// test for features

// Compiler versions know to work with this

// gcc-??
// clang-??

#include "c23-fallback.h"
#include <stdio.h>

#define test(...) printf("Testing " #__VA_ARGS__ ": %lld\n", (long long)(__VA_ARGS__))

int main(int argc, char* argv[argc+1]) {
  test(iscomplex(0.0));
  test(isimaginary(0.0));
  test(isfloating(0.0));
  test(iscomplex(I));
  test(isimaginary(I));
  test(isfloating(I));
  test(iscomplex(0U));
  test(isimaginary(0U));
  test(isfloating(0U));
  test(isinteger(0.0));
  test(isinteger(0U));
  test(isinteger((char)'A'));
  test(isunsigned((char)'A'));
  test(issigned((char)'A'));
  test(isunsigned((unsigned char)'A'));
  test(issigned((unsigned char)'A'));
  test(tominusone((char)'A'));
  test(tominusone((unsigned char)'A'));
  test(is_const_target((char*)nullptr));
  test(is_const_target((char const*)nullptr));
  test(is_const_target((char volatile*)nullptr));
  test(is_const_target((char const volatile*)nullptr));
  test(is_volatile_target((char*)nullptr));
  test(is_volatile_target((char const*)nullptr));
  test(is_volatile_target((char volatile*)nullptr));
  test(is_volatile_target((char const volatile*)nullptr));
  test(is_const((char){ 0 }));
  test(is_const((char const){ 0 }));
  test(is_const((char volatile){ 0 }));
  test(is_const((char const volatile){ 0 }));
  test(is_volatile((char){ 0 }));
  test(is_volatile((char const){ 0 }));
  test(is_volatile((char volatile){ 0 }));
  test(is_volatile((char const volatile){ 0 }));
  test(is_null_pointer_constant(0));
  test(is_null_pointer_constant(nullptr));
  test(is_null_pointer_constant((void*)0));
  test(is_null_pointer_constant((void*)1));
  test(is_null_pointer_constant((void*)0LL));
  test(is_null_pointer_constant((void*)(long long){ 0 }));
  test(is_zero_ice(0));
  test(is_zero_ice(1));
  test(is_zero_ice((long long){ 0 }));
  test(is_zero_ice(0.0));
  test(is_zero_ice(((void)0, 0)));
  test(isice(0));
  test(isice(1));
  test(isice((long long){ 0 }));
  test(isice(0.0));
  test(isvla(0));
  test(isvla((char[]){ 0 }));
  char fla[1][1];
  test(isvla(fla));
  test(isvla(fla[0]));
  test(isvla(&fla));
  test(isvla(*(&fla)));
  char vla[(int){ 1 }][1];
  test(isvla(vla));
  test(isvla(vla[0]));
  test(isvla(&vla));
  test(isvla(*(&vla)));
  test(isvla(char[1][1]));
  test(isvla(char[(int){ 1 }][1]));
  test(isvla(char(*)[1][1]));
  test(isvla(char(*)[(int){ 1 }][1]));
  test(is_pointer_nvla((char const volatile[]){ 0 }));
  test(is_pointer_nvla((char const volatile*){ 0 }));
  test(is_pointer_nvla(fla));
  test(is_pointer_nvla(fla[0]));
  test(is_pointer_nvla(&fla));
  test(is_pointer_nvla(*(&fla)));
  test(is_pointer_nvla(vla));
  test(is_pointer_nvla(vla[0]));
  test(is_pointer_nvla(&vla));
  test(is_pointer_nvla(*(&vla)));
  test(is_pointer_vla(fla));
  test(is_pointer_vla(fla[0]));
  test(is_pointer_vla(&fla));
  test(is_pointer_vla(*(&fla)));
  test(is_pointer_vla(vla));
  test(is_pointer_vla(vla[0]));
  test(is_pointer_vla(&vla));
  test(is_pointer_vla(*(&vla)));
  test(is_pointer((char const volatile[]){ 0 }));
  test(is_pointer((char const volatile*){ nullptr }));
  test(is_pointer((char*)nullptr));
  test(is_pointer((void*)nullptr));
  test(is_pointer((char const volatile*)nullptr));
  test(is_pointer(fla));
  test(is_pointer(fla[0]));
  test(is_pointer(&fla));
  test(is_pointer(*(&fla)));
  test(is_pointer(vla));
  test(is_pointer(vla[0]));
  test(is_pointer(&vla));
  test(is_pointer(*(&vla)));
  test(is_fla((char const volatile[]){ 0 }));
  test(is_fla((char const volatile*){ 0 }));
  test(is_fla(fla));
  test(is_fla(fla[0]));
  test(is_fla(&fla));
  test(is_fla(*(&fla)));
  test(is_fla(vla));
  test(is_fla(vla[0]));
  test(is_fla(&vla));
  test(is_fla(*(&vla)));
  test(is_array((char const volatile[]){ 0 }));
  test(is_array((char const volatile*){ 0 }));
  test(is_array(fla));
  test(is_array(fla[0]));
  test(is_array(&fla));
  test(is_array(*(&fla)));
  test(is_array(vla));
  test(is_array(vla[0]));
  test(is_array(&vla));
  test(is_array(*(&vla)));
  /* test(is_array(char[1][1])); */
  /* test(is_array(char[(int){ 1 }][1])); */
  /* test(is_array(char(*)[1][1])); */
  /* test(is_array(char(*)[(int){ 1 }][1])); */
#if __has_feature(c_stdbit_h)
  test(stdc_count_ones_uc(0));
  test(stdc_count_ones_us(0));
  test(stdc_count_ones_ui(0));
  test(stdc_count_ones_ul(0));
  test(stdc_count_ones_ull(0));
  test(stdc_count_ones(0U));
  test(stdc_count_ones_uc(-1));
  test(stdc_count_ones_us(-1));
  test(stdc_count_ones_ui(-1));
  test(stdc_count_ones_ul(-1));
  test(stdc_count_ones_ul(-1));
  test(stdc_count_ones_uc(u'a'));
  test(stdc_count_ones_us(u'a'));
  test(stdc_count_ones_ui(u'a'));
  test(stdc_count_ones_ul(u'a'));
  test(stdc_count_ones_ull(u'a'));
#endif
  test(stdc_count_ones(u'a'));
  test(stdc_count_ones(0b01010101U));
  test(stdc_count_ones(0b01010101UL));
  test(stdc_count_ones(0b01010101ULL));
#if __has_feature(c_stdbit_h)
  test(stdc_count_zeros_uc(0));
  test(stdc_count_zeros_us(0));
  test(stdc_count_zeros_ui(0));
  test(stdc_count_zeros_ul(0));
  test(stdc_count_zeros_ull(0));
  test(stdc_count_zeros_uc(-1));
  test(stdc_count_zeros_us(-1));
  test(stdc_count_zeros_ui(-1));
  test(stdc_count_zeros_ul(-1));
  test(stdc_count_zeros_ul(-1));
#endif
  test(stdc_count_zeros(u'a'));
  test(stdc_count_zeros(0b01010101U));
  test(stdc_count_zeros(0b01010101UL));
  test(stdc_count_zeros(0b01010101ULL));
#if __has_feature(c_stdbit_h)
  test(stdc_leading_zeros_uc(0U));
  test(stdc_leading_zeros_us(0U));
  test(stdc_leading_zeros_ui(0U));
  test(stdc_leading_zeros_ul(0U));
  test(stdc_leading_zeros_ull(0U));
#endif
  test(stdc_leading_zeros(0U));
  test(stdc_leading_zeros(1U));
  test(stdc_leading_zeros(0b01010101U));
  test(stdc_leading_zeros(0b01010101UL));
  test(stdc_leading_zeros(0b01010101ULL));
  test(stdc_trailing_zeros(0U));
  test(stdc_trailing_zeros(1U));
  test(stdc_trailing_zeros(0b0101010100U));
  test(stdc_trailing_zeros(0b0101010100UL));
  test(stdc_trailing_zeros(0b0101010100ULL));
  test(stdc_trailing_zeros(u'\0'));
  test(stdc_trailing_zeros(0U));
  test(stdc_trailing_zeros(0UL));
  test(stdc_trailing_zeros(0ULL));
#if __has_feature(c_stdbit_h)
  test(stdc_leading_ones_ui(UINT_MAX));
#endif
  test(stdc_leading_ones(UINT_MAX));
  test(stdc_leading_ones(0U+INT_MIN));
  test(stdc_leading_ones(1U+INT_MIN));
  test(stdc_leading_ones(ULLONG_MAX ^ UINT_MAX));
  test(stdc_trailing_ones(0U));
  test(stdc_trailing_ones(1U));
  test(stdc_trailing_ones(0b01010101U));
  test(stdc_trailing_ones(0b01010101UL));
  test(stdc_trailing_ones(0b01010101ULL));
  test(stdc_trailing_ones(u'\0'));
  test(stdc_trailing_ones(UINT_MAX>>1));
  test(stdc_trailing_ones(ULONG_MAX>>1));
  test(stdc_trailing_ones(ULONG_MAX>>1));
  // argument must be an unsigned type
  // test(stdc_leading_ones(1));
  /* test(stdc_leading_zeros(true)); */
  /* test(stdc_leading_zeros(false)); */
  /* test(stdc_leading_zeros((bool)1)); */
  test(stdc_leading_zeros((unsigned char)1));
#if __has_feature(c_stdbit_h)
  test(stdc_leading_zeros_uc(u'a'));
  test(stdc_leading_zeros_us(u'a'));
  test(stdc_leading_zeros_ui(u'a'));
  test(stdc_leading_zeros_ul(u'a'));
  test(stdc_leading_zeros_ull(u'a'));
#endif
  test(stdc_leading_zeros(u'a'));
  test(stdc_has_single_bit(u'a'));
  test(stdc_has_single_bit(u'\2'));
  /* test(stdc_has_single_bit(true)); */
  /* test(stdc_has_single_bit(false)); */
#if __has_feature(c_stdbit_h)
  test(stdc_has_single_bit_uc(0U));
  test(stdc_has_single_bit_uc(1U));
  test(stdc_has_single_bit_uc(2U));
  test(stdc_has_single_bit_uc(3U));
  test(stdc_has_single_bit_uc(4U));
#endif
  test(stdc_has_single_bit(0U));
  test(stdc_has_single_bit(1U));
  test(stdc_has_single_bit(2U));
  test(stdc_has_single_bit(3U));
  test(stdc_has_single_bit(4U));
  test(stdc_has_single_bit(1UL));
  test(stdc_has_single_bit(2UL));
  test(stdc_has_single_bit(3UL));
  test(stdc_has_single_bit(1ULL));
  test(stdc_has_single_bit(2ULL));
  test(stdc_has_single_bit(3ULL));
#if __has_feature(c_stdbit_h)
  test(stdc_bit_width_uc(u'a'));
  test(stdc_bit_width_us(u'a'));
  test(stdc_bit_width_ui(u'a'));
  test(stdc_bit_width_ul(u'a'));
  test(stdc_bit_width_ull(u'a'));
  test(stdc_bit_width(u'a'));
#endif
  /* test(stdc_bit_width(true)); */
  /* test(stdc_bit_width(false)); */
  test(stdc_bit_width(1U));
  test(stdc_bit_width(2U));
  test(stdc_bit_width(3U));
  test(stdc_bit_width(1UL));
  test(stdc_bit_width(2UL));
  test(stdc_bit_width(3UL));
  test(stdc_bit_width(1ULL));
  test(stdc_bit_width(2ULL));
  test(stdc_bit_width(3ULL));
  /* test(stdc_bit_floor(true)); */
  /* test(stdc_bit_floor(false)); */
  test(stdc_bit_floor(0U));
  test(stdc_bit_floor(1U));
  test(stdc_bit_floor(2U));
  test(stdc_bit_floor(3U));
  test(stdc_bit_floor(4U));
  test(stdc_bit_floor(1UL));
  test(stdc_bit_floor(2UL));
  test(stdc_bit_floor(3UL));
  test(stdc_bit_floor(1ULL));
  test(stdc_bit_floor(2ULL));
  test(stdc_bit_floor(3ULL));
  /* test(stdc_bit_ceil(true)); */
  /* test(stdc_bit_ceil(false)); */
  test(stdc_bit_ceil(0U));
  test(stdc_bit_ceil(1U));
  test(stdc_bit_ceil(2U));
  test(stdc_bit_ceil(3U));
  test(stdc_bit_ceil(4U));
  test(stdc_bit_ceil(1UL));
  test(stdc_bit_ceil(2UL));
  test(stdc_bit_ceil(3UL));
  test(stdc_bit_ceil(1ULL));
  test(stdc_bit_ceil(2ULL));
  test(stdc_bit_ceil(3ULL));
#ifdef UINT128_MAX
  test(stdc_leading_zeros(UINT128_MAX));
  test(stdc_leading_zeros(UINT128_MAX>>63));
  test(stdc_leading_zeros(UINT128_MAX>>64));
  test(stdc_leading_zeros(UINT128_MAX>>65));
  test(stdc_leading_zeros(UINT128_MAX ^ INT128_MIN));
  test(stdc_leading_zeros((UINT128_MAX ^ INT128_MIN)>>3));
  test(stdc_leading_zeros((UINT128_MAX ^ INT128_MIN)>>63));
  test(stdc_count_ones(UINT128_MAX ^ 0b01010101U));
  test(stdc_count_zeros(UINT128_MAX ^ 0b01010101U));
  test(stdc_trailing_zeros(~UINT128_MAX));
  test(stdc_trailing_zeros(UINT128_MAX - 1));
  test(stdc_trailing_zeros(UINT128_MAX<<63));
  test(stdc_trailing_zeros(UINT128_MAX<<64));
  test(stdc_trailing_zeros(UINT128_MAX<<65));
  test(stdc_trailing_zeros(UINT128_MAX & INT128_MIN));
  test(stdc_trailing_zeros((UINT128_MAX & INT128_MIN)>>3));
  test(stdc_trailing_zeros((UINT128_MAX & INT128_MIN)>>63));
  test(stdc_leading_ones(UINT128_MAX));
  test(stdc_leading_ones(UINT128_MAX ^ INT128_MIN));
  test(stdc_leading_ones(UINT128_MAX ^ UINT_MAX));
  test(stdc_leading_ones(UINT128_MAX ^ (UINT_MAX>>3)));
  test(stdc_trailing_ones(UINT128_MAX));
  test(stdc_trailing_ones(UINT128_MAX - 1));
  test(stdc_trailing_ones(UINT128_MAX>>63));
  test(stdc_trailing_ones(UINT128_MAX>>64));
  test(stdc_trailing_ones(UINT128_MAX>>65));
  test(stdc_trailing_ones(UINT128_MAX ^ INT128_MIN));
  test(stdc_trailing_ones((UINT128_MAX ^ INT128_MIN)>>3));
  test(stdc_trailing_ones((UINT128_MAX ^ INT128_MIN)>>63));
  test(stdc_has_single_bit(UINT128_MAX ^ INT128_MIN));
  test(stdc_has_single_bit(UINT128_MAX & INT128_MIN));
  test(stdc_bit_width(UINT128_MAX ^ INT128_MIN));
  test(stdc_bit_width(UINT128_MAX & INT128_MIN));
  test(stdc_bit_floor(UINT128_MAX & INT128_MIN) == INT128_MIN);
  test(stdc_bit_ceil(UINT128_MAX>>1) == INT128_MIN);
  test(stdc_leading_zeros(argc+tozero(UINT128_MAX)));
#endif
#if BITINT_MAXWIDTH >= 128
  unsigned _BitInt(128) const ubitint128_max = -1;
    signed _BitInt(128) const  bitint128_min = ubitint128_max/2;
  test(stdc_leading_zeros(ubitint128_max));
  test(stdc_leading_zeros(ubitint128_max>>63));
  test(stdc_leading_zeros(ubitint128_max>>64));
  test(stdc_leading_zeros(ubitint128_max>>65));
  test(stdc_leading_zeros(ubitint128_max ^ bitint128_min));
  test(stdc_leading_zeros((ubitint128_max ^ bitint128_min)>>3));
  test(stdc_leading_zeros((ubitint128_max ^ bitint128_min)>>63));
  test(stdc_count_ones(ubitint128_max ^ 0b01010101U));
  test(stdc_count_zeros(ubitint128_max ^ 0b01010101U));
  test(stdc_trailing_zeros(~ubitint128_max));
  test(stdc_trailing_zeros(ubitint128_max - 1));
  test(stdc_trailing_zeros(ubitint128_max<<63));
  test(stdc_trailing_zeros(ubitint128_max<<64));
  test(stdc_trailing_zeros(ubitint128_max<<65));
  test(stdc_trailing_zeros(ubitint128_max & bitint128_min));
  test(stdc_trailing_zeros((ubitint128_max & bitint128_min)>>3));
  test(stdc_trailing_zeros((ubitint128_max & bitint128_min)>>63));
  test(stdc_leading_ones(ubitint128_max));
  test(stdc_leading_ones(ubitint128_max ^ bitint128_min));
  test(stdc_leading_ones(ubitint128_max ^ UINT_MAX));
  test(stdc_leading_ones(ubitint128_max ^ (UINT_MAX>>3)));
  test(stdc_trailing_ones(ubitint128_max));
  test(stdc_trailing_ones(ubitint128_max - 1));
  test(stdc_trailing_ones(ubitint128_max>>63));
  test(stdc_trailing_ones(ubitint128_max>>64));
  test(stdc_trailing_ones(ubitint128_max>>65));
  test(stdc_trailing_ones(ubitint128_max ^ bitint128_min));
  test(stdc_trailing_ones((ubitint128_max ^ bitint128_min)>>3));
  test(stdc_trailing_ones((ubitint128_max ^ bitint128_min)>>63));
  test(stdc_has_single_bit(ubitint128_max ^ bitint128_min));
  test(stdc_has_single_bit(ubitint128_max & bitint128_min));
  test(stdc_bit_width(ubitint128_max ^ bitint128_min));
  test(stdc_bit_width(ubitint128_max & bitint128_min));
  test(stdc_bit_floor(ubitint128_max & bitint128_min) == bitint128_min);
  test(stdc_bit_ceil(ubitint128_max>>1) == bitint128_min);
  test(stdc_leading_zeros(argc+tozero(ubitint128_max)));
#endif
}
