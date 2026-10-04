#ifndef C23_FALLBACK_H
#define C23_FALLBACK_H

/** @file
 ** @brief A header file to augment compatibility of older compilers with C23.
 **
 ** This has only been tested with the open source compilers
 **
 ** - gcc
 ** - clang
 **
 ** and the C libraries
 **
 ** - glibc
 ** - musl
 **
 ** The newer the version of these compilers or C libraries you can
 ** get, the better. If you use another compiler or C library and have
 ** adjustments to make to this file, please share. I will try to
 ** include such changes in the future.
 **
 ** Commonly supported C23 features that are required are
 ** - digit separators such as in `0xAB'CD`
 ** - binary integer literals such as `0b1010` or `0B0101`
 ** - the new attribute syntax such as `[[deprecated]]`
 **
 ** The first two are not easy to circumvent, so probably you should
 ** not even try to use C23 enabled code without having support for
 ** them.
 **
 ** For the attribute syntax there is a feature test
 ** __has_c_attribute. It can be used with define (or #ifdef) to test
 ** for the syntax itself and with #if and an argument to test for an
 ** individual attribute, see below.
 **
 ** Another feature test is __has_include, which similarly can be used
 ** to test for the preprocessor feature itself and if that is
 ** available to test for the availability of a specific header. This
 ** can be used to
 **
 ** - test for optional headers such as <complex.h>, <threads.h>, or
 **   <stdatomic.h>. This now allows to use the corresponding feature
 **   test macro (__STDC_NO_COMPLEX__, __STDC_NO_ATOMICS__,
 **   __STDC_NO_THREADS__) to test for compiler support of these
 **   features, separately from the availability of library
 **   interfaces.
 **
 ** - test for new headers that come with C23, namely <stdckdint.h>
 **   and <stdbit.h>. Even if your C library does not support these,
 **   their functionality should mostly be available through
 **   type-generic macros that we emulate. Prefer them over function
 **   symbols, those may depend on linker symbols that are not yet
 **   available.
 **
 ** This header here unconditionally includes a bunch of C library
 ** headers (see below) and augments them with C23 features if
 ** possible. You should not include them yourself, such that we are
 ** sure in what we get. This also concerns the new C23 header
 ** <stdckdint.h>.
 **
 ** Other headers also may be included conditionally, namely
 ** <threads.h> and <stdatomic.h>.
 **
 ** When emulating the new features, there may be spurious warnings
 ** that some attributes are misplaced or ignored. This obviously
 ** means that they are not completely taken into account and that the
 ** analysis they are expected to provide is not yet fully implemented
 ** by your compiler. If you are flooded by these, you might switch
 ** some of the diagnostics off by defining the macro
 ** `C23_FALLBACK_SILENT` for example by providing the command line
 ** argument `-DC23_FALLBACK_SILENT`.
 **
 ** But note also, that some errors that are found are actually real
 ** errors. These are meant as bad code examples and also as tests if
 ** compilers are able to detect these kind of things. If you stumble
 ** into these, there should be a code comment close by that explains
 ** the particular example. Compilers vary widely in what they are
 ** able to detect, so here the more you see, the better.
 **
 ** And then, ..., there are the bugs. Currently, the code triggers
 ** one particular bug of gcc-13 in `endianness.c`.
 **
 ** As of March 2024, gcc-14 and clang-18 work with most of the new
 ** C23 features. Current restrictions are
 **
 ** - clang-18: `constexpr` is not yet implemented
 ** - both: `#embed` is not yet implemented
 **
 ** As indicated above, some short comings of your C library
 ** implementation can be worked around, but unfortunately not all. As
 ** of this writing
 **
 ** - glibc: `printf` and `scanf` support only includes the new %b
 **   specifier for binary representation, but not the %w length
 **   modifiers.
 **
 ** - musl: in the current distribution there is no support for C23
 **   features, so in particular `printf` etc will not work with the
 **   new formats, yet.
 **
 ** We are currently working to have patches included to musl whence
 ** the new edition of Modern C and the new standard appear. In the
 ** mean time I will try to post a patch set and build recipe such
 ** that you may easily build such modified version of musl with
 ** complete C23 support on your own. Stay tuned.
 **/

#include <limits.h>

/* Needed for musl */
/* #ifdef __has_include */
/* # if __has_include(<stdc-predef.h>) */
/* #  include <stdc-predef.h> */
/* # endif */
/* #endif */


#if (__STDC_UTF_16__ != 1) || (__STDC_UTF_32__ != 1)
# ifndef C23_FALLBACK_SILENT
#  warning "C23 imposes that char16_t and char32_t use UTF-16 and UTF-32 encodings"
# endif
#endif

/**
 ** @brief A preprocessor test for the `__VA_OPT__` feature.
 **
 ** Compilers before C23 might not implement `__VA_OPT__`.
 ** @return This macro should always evaluate to `0` or `1`.
 **/
#define __has_va_opt C23_HAS_VA_OPT_I(C23)
#define C23_HAS_VA_OPT_I(...)                   \
  C23_HAS_VA_OPT_IIplus(__VA_OPT__(,) 1, 0,)
#define C23_HAS_VA_OPT_IIplus(_0, _1, ...) _1

#if !__has_va_opt
# ifndef C23_FALLBACK_SILENT
#  warning "no support of C23 __VA_OPT__ found"
# endif
#endif

#ifndef BITINT_MAXWIDTH
# ifdef __BITINT_MAXWIDTH__
#  define BITINT_MAXWIDTH __BITINT_MAXWIDTH__
# else
#  ifndef C23_FALLBACK_SILENT
#   warning "no support for _BitInt types found"
#  endif
# endif
#endif

/* A simple type-generic conditional. */
#define GENERIC_IF(COND, A, B)                  \
  _Generic((char(*)[1+!!(COND)])0,              \
           char(*)[2]: (A),                     \
           default: (B))

/* clang has a set of feature tests that are quite comfortable,

   __has_feature, to test if a specific feature is implemented

   __has_extension, to test if a specific feature is implemented

   __is_identifier, to test if a word is an identifier or a keyword.

   In the following we construct similar tests for gcc such that we
   may use these features independently from the C standard version
   that is supported. */
# define __tester2(_0, _1, ...) _1
# define __tester1(...) __tester2(__VA_ARGS__)
#ifndef __has_feature
# define __has_feature(X) __tester1(__has_feature_ ## X 1, 0, )
#endif
#ifndef __has_extension
# define __has_extension(X) __tester1(__has_extension_ ## X 1, 0, )
#endif

#ifndef __is_identifier
# define __is_identifier(X) __tester1(__is_special_ ## X 0, 1, )
# define __is_special_auto ,
# define __is_special_break ,
# define __is_special_case ,
# define __is_special_char ,
# define __is_special_const ,
# define __is_special_continue ,
# define __is_special_default ,
# define __is_special_do ,
# define __is_special_double ,
# define __is_special_else ,
# define __is_special_enum ,
# define __is_special_extern ,
# define __is_special_float ,
# define __is_special_for ,
# define __is_special_goto ,
# define __is_special_if ,
# define __is_special_inline ,
# define __is_special_int ,
# define __is_special_long ,
# define __is_special_register ,
# define __is_special_restrict ,
# define __is_special_return ,
# define __is_special_short ,
# define __is_special_signed ,
# define __is_special_sizeof ,
# define __is_special_static ,
# define __is_special_struct ,
# define __is_special_switch ,
# define __is_special_typedef ,
# define __is_special_union ,
# define __is_special_unsigned ,
# define __is_special_void ,
# define __is_special_volatile ,
# define __is_special_while ,
# define __is_special__Atomic ,
# define __is_special__Complex ,
# define __is_special__Decimal128 ,
# define __is_special__Decimal32 ,
# define __is_special__Decimal64 ,
# define __is_special__Generic ,
# define __is_special__Imaginary ,
# define __is_special__Noreturn ,
# if __GNUC__ > 12 || __STDC_VERSION__ >= 202311L
#  define __is_special_nullptr ,
#  define __is_special_static_assert ,
#  define __is_special_thread_local ,
#  define __is_special_constexpr ,
#  define __is_special_typeof ,
#  define __is_special_typeof_unqual ,
# endif
# if __GNUC__ > 13 || __STDC_VERSION__ >= 202311L
#  define __is_special__BitInt ,
#  define __is_special_alignas ,
#  define __is_special_alignof ,
#  define __is_special_bool ,
#  define __is_special_false ,
#  define __is_special_true ,
# endif
#endif
#if __GNUC__ > 12 || __STDC_VERSION__ >= 202311L
# define __has_feature_c_fixed_enum ,
#endif

// The feature for fixed underlying types for enums has been so long
// in clang, that they don't even seem to have a feature test for it.
#if !__has_feature(c_fixed_enum) && defined(__clang_major__) && (__clang_major__ < 8)
# ifndef C23_FALLBACK_SILENT
#  warning "syntax for fixed underlying integer type of enumerations is not supported"
# endif
#endif

#if __is_identifier(constexpr)
# ifndef C23_FALLBACK_SILENT
#  warning "constexpr keyword is not supported, emulating as static const"
# endif
# define constexpr static const
#endif

#if __is_identifier(bool)
# ifndef C23_FALLBACK_SILENT
#  warning "bool keyword is not supported, emulating as macro"
# endif
# define bool _Bool
#endif
#if __is_identifier(false)
# ifndef C23_FALLBACK_SILENT
#  warning "false keyword is not supported, emulating as macro"
# endif
# undef false
# define false ((bool)+0U)
#endif
#if __is_identifier(true)
# ifndef C23_FALLBACK_SILENT
#  warning "true keyword is not supported, emulating as macro"
# endif
# undef true
# define true ((bool)+1U)
#endif

#if __is_identifier(thread_local)
// Maybe this is also provided by the C library implementation, but a
// redefinition makes no harm.
#define thread_local _Thread_local
#endif

#if __is_identifier(alignof)
#undef alignof
#define alignof _Alignof
#endif

#if __is_identifier(alignas)
#undef alignas
#define alignas _Alignas
#endif

#if __is_identifier(static_assert)
#undef static_assert
#define static_assert(...) __static_assert(__VA_ARGS__,)
#define __static_assert(E, ...) _Static_assert(E)
#endif

#if __is_identifier(typeof)
# if __GNUC__
#  undef typeof
#  define typeof __typeof__
# else
#  warning "typeof operator only comes with C23"
# endif
#endif

#if __STDC_VERSION__ < 202311L
# if __GNUC__
#  if __GNUC__ < 13
#   define auto __auto_type
#  endif
# else
#  warning "auto type inference only comes with C23"
# endif
#endif

#ifndef __has_c_attribute
# define __has_c_attribute(X) 0
#endif

// C23 has nullptr, use a fallback if that is not available. Here we
// use an enumeration type and constant to do that. The idea is that
// such an enumeration may then be used in _Generic as long the cases
// otherwise only have pointer types. The first choice is to use a
// _BitInt type for that, if that is already available, in an effort
// to not clash with standard integer types. If that is not found, we
// use a standard integer type that has the right width.
//
// If possible, we also use an attribute that forces warnings if any
// other value than a null or 0 is assigned to a variable of type
// nullptr_t.
#if !__is_identifier(nullptr)
// Maybe this is also provided by the C library implementation, but a
// redefinition makes no harm.
typedef typeof(nullptr) nullptr_t;
#elif (__STC_VERSION__ < 202311L)
# ifdef BITINT_MAXWIDTH

#  ifndef C23_FALLBACK_SILENT
#   warning "using fallback for nullptr to enumeration constant based on _BitInt type"
#  else
#   pragma GCC diagnostic ignored "-Wnon-literal-null-conversion"
#  endif

enum
#  if __has_c_attribute(clang::enum_extensibility)
[[__clang__::__enum_extensibility__(closed)]]
#  endif
nullptr_t :
__typeof__((unsigned _BitInt(sizeof(void*)*CHAR_BIT))0)
{
  nullptr,
};

# else

#  ifndef C23_FALLBACK_SILENT
#   warning "using fallback for nullptr to enumeration constant based on standard integer type"
#  endif

enum
#  if __has_c_attribute(clang::enum_extensibility)
[[__clang__::__enum_extensibility__(closed)]]
#  endif
nullptr_t {
  nullptr,
  __nullptr_max =
  GENERIC_IF((sizeof(void*)==sizeof(int)),
             -1U,
             GENERIC_IF((sizeof(void*)==sizeof(long)),
                        -1UL,
                        -1ULL)),
};

// Unfortunately such an enumeration type is not yet good enough for
// all compilers. In particular some gcc version has nullptr and
// __nullptr_max as different types. Force an expression that is an
// ICE of value 0 and of the enumeration type.
#  define nullptr (1 ? nullptr : __nullptr_max)
# endif
#endif
typedef typeof(nullptr) nullptr_t;

static_assert(_Generic(nullptr, nullptr_t: true));
static_assert(sizeof(void*)==sizeof(nullptr));
static_assert(sizeof(void*)==sizeof(nullptr_t));

/**********************************************************************************************/

#include <inttypes.h>
#include <setjmp.h>
#include <signal.h>
#include <stdalign.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

/**********************************************************************************************/

// for call_once
#ifndef ONCE_FLAG_INIT
# ifdef __has_include
#  if __has_include(<threads.h>)
#   ifndef C23_FALLBACK_SILENT
#    warning "including call_once function from <threads.h> instead of <stdlib.h>"
#   endif
#   include <threads.h>
#  endif
# endif
#endif

// If call_once has not been found we construct a function that is
// based on atomics.
#ifndef ONCE_FLAG_INIT
# ifdef __has_include
#  if __has_include(<stdatomic.h>)
#   include <stdatomic.h>
#   ifndef C23_FALLBACK_SILENT
#    warning "emulating call_once function by using atomics"
#   endif
enum { once_initial, once_inter, once_final, };
#   define ONCE_FLAG_INIT once_initial

typedef _Atomic(unsigned) once_flag;

# if __has_c_attribute(__gnu__::__noinline__)
[[__gnu__::__noinline__]]
# endif
static inline
void call_once(once_flag* flag, void callback(void)) {
  if (*flag != once_final) {
    unsigned expected = once_initial;
    if (atomic_compare_exchange_strong(flag, &expected, once_inter)) {
      callback();
      *flag = once_final;
    } else {
      while (*flag != once_final) {
        //
      }
    }
  } else {
    // We know already that the implementation does not support threads,
    // so the only available fence is this:
    atomic_signal_fence(memory_order_seq_cst);
  }
}
#  endif
# endif
#endif

/**********************************************************************************************/

// We assume that the compiler is already able to handle the new
// attribute syntax such as [[deprecated]]. Nevertheless, maybe
// certain attributes themselves might not yet be implemented. We
// provide fallback macros to corresponding gnu attributes. Here we
// use the "safe" names for these features with double underscores, so
// they don't interfere with application names.

#if !__has_c_attribute(__unsequenced__)
# ifndef C23_FALLBACK_SILENT
#  warning "unsequenced attribute not yet implemented"
# endif
# define __unsequenced__
# if __has_c_attribute(__gnu__::__const__)
#  ifndef C23_FALLBACK_SILENT
#   warning "unsequenced attribute could be emulated via gnu attribute by using c23_unsequenced"
#  endif
#  define c23_unsequenced __attribute__((__const__))
# endif
#endif

#if !__has_c_attribute(__reproducible__)
# ifndef C23_FALLBACK_SILENT
#  warning "reproducible attribute not yet implemented"
# endif
# define __reproducible__
# if __has_c_attribute(__gnu__::__pure__)
#  ifndef C23_FALLBACK_SILENT
#   warning "reproducible attribute could be emulated via gnu attribute by using c23_reproducible"
#  endif
#  define c23_reproducible __attribute__((__pure__))
# endif
#endif

#if !__has_c_attribute(__deprecated__) && __has_c_attribute(__gnu__::__deprecated__)
# ifndef C23_FALLBACK_SILENT
#  warning "emulating deprecated attribute via gnu attribute"
# endif
# define __deprecated__ __gnu__::__deprecated__
#endif

#if !__has_c_attribute(__nodiscard__) && __has_c_attribute(__gnu__::__warn_unused_result__)
# ifndef C23_FALLBACK_SILENT
#  warning "emulating nodiscard attribute via gnu attribute"
# endif
# define __nodiscard__ __gnu__::__warn_unused_result__
#endif

#if !__has_c_attribute(__maybe_unused__) && __has_c_attribute(__gnu__::__unused__)
# ifndef C23_FALLBACK_SILENT
#  warning "emulating maybe_unused attribute via gnu attribute"
# endif
# define __maybe_unused__ __gnu__::__unused__
#endif

#if !__has_c_attribute(noreturn) && __has_c_attribute(__gnu__::__noreturn__)
# ifndef C23_FALLBACK_SILENT
#  warning "emulating noreturn attribute via gnu attribute"
# endif
# define noreturn __gnu__::__noreturn__
#endif

#if !__has_c_attribute(__fallthrough__) && __has_c_attribute(__gnu__::__fallthrough__)
# warning "emulating fallthrough attribute via gnu attribute"
# define __fallthrough__ __gnu__::__fallthrough__
#endif

#if __has_c_attribute(__deprecated__) || defined(__deprecated__)
// Deprecate two interfaces in <time.h>
[[__deprecated__]] char *asctime(const struct tm*);
[[__deprecated__]] char *ctime(const time_t*);
#endif

// Gnu compilers have a malloc attribute that comes in two different
// forms. Without argument it just certifies that a function returns a
// pointer value that has not been seen before, and thus the pointer
// target will not alias with anything known. The second form receives
// one or two arguments and specifies a function that should be used
// analogous to free. Unfortunately, clang didn't follow that second
// development, yet. But we suppose that there will be at least a
// partial solution in clang-18 final.
#if __has_c_attribute(__gnu__::__malloc__) && !(defined(__clang_major__) && __clang_major__ <= 19)
# define __gnu_free__(...) __gnu__::__malloc__(__VA_ARGS__)
#else
# define __gnu_free__(...)
#endif

/**********************************************************************************************/

/* C23 allows to omit the second argument of va_start */
#if __STDC_VERSION_STDARG_H__ < 202311L
#undef va_start
#define va_start(...) __va_start1(__VA_ARGS__ , 0, )
#define __va_start1(...) __va_start2(__VA_ARGS__)
#define __va_start2(v, l, ...) __builtin_va_start(v, l)
#endif

/**********************************************************************************************/

/* C23 has three new tg interfaces in the new <stdkdint.h> header.
They are modeled after similar gcc features. They are meant to do
arithmetic with overflow check by using everything the compiler can
get from instruction flags that already exist on most CPU.
*/

#ifdef __has_include
# if __has_include(<stdckdint.h>)
#  include <stdckdint.h>
# endif
#endif

/* If we don't have the header, yet, we may easily emulate it if we
are on a compiler claiming compatibility with gcc. */
#ifndef ckd_add
# ifdef __GNUC__
#  ifndef C23_FALLBACK_SILENT
#   warning "emulating stdckdint.h interfaces via gnu builtins"
#  endif
#  define ckd_add(R, A, B) __builtin_add_overflow ((A), (B), (R))
#  define ckd_sub(R, A, B) __builtin_sub_overflow ((A), (B), (R))
#  define ckd_mul(R, A, B) __builtin_mul_overflow ((A), (B), (R))
# else
#  warning "checked integer arithmetic as of C23 is not yet implemented"
# endif
#endif

/**********************************************************************************************/

// C23 has the unreachable macro in <stdlib.h>, use a fallback if that
// is not available
#ifndef unreachable
# ifndef C23_FALLBACK_SILENT
#  warning "using fallback for unreachable macro"
# endif
# define unreachable() __builtin_unreachable()
#endif

/**********************************************************************************************/

// C23 provides macros for the width of integer types. Provide a
// fallback if they are not available

#ifndef BOOL_WIDTH
# define BOOL_WIDTH 1
#endif

#ifndef CHAR_WIDTH
# define CHAR_WIDTH CHAR_BIT
#endif

#ifndef UCHAR_WIDTH
# define UCHAR_WIDTH CHAR_BIT
#endif

#ifndef SCHAR_WIDTH
# define SCHAR_WIDTH CHAR_BIT
#endif

#ifndef USHRT_WIDTH
# if USHRT_MAX <= 0xFFFFU
#  define USHRT_WIDTH 16
#  define SHRT_WIDTH 16
# else
#  if USHRT_MAX <= 0xFFFFFFFFU
#   define USHRT_WIDTH 32
#   define SHRT_WIDTH 32
#  else
#   if USHRT_MAX <= 0xFFFFFFFFFFFFFFFFU
#    define USHRT_WIDTH 64
#    define SHRT_WIDTH 64
#   else
// This doesn't use #elif but nested #if/#else because the value might
// be too large for the preprocessor
#    if USHRT_MAX <= 0xFFFFFFFFFFFFFFFFFFFFFFFFU
#     define USHRT_WIDTH 96
#     define SHRT_WIDTH 96
#    else
// This doesn't use #elif but nested #if/#else because the value might
// be too large for the preprocessor
#     if USHRT_MAX <= 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFU
#      define USHRT_WIDTH 128
#      define SHRT_WIDTH 128
#     endif
#    endif
#   endif
#  endif
# endif
#endif

// unsigned int has a width of at least 16.
#ifndef UINT_WIDTH
# if UINT_MAX <= 0xFFFFU
#  define UINT_WIDTH 16
#  define INT_WIDTH 16
# else
#  if UINT_MAX <= 0xFFFFFFFFU
#   define UINT_WIDTH 32
#   define INT_WIDTH 32
#  else
#   if UINT_MAX <= 0xFFFFFFFFFFFFFFFFU
#    define UINT_WIDTH 64
#    define INT_WIDTH 64
#   else
// This doesn't use #elif but nested #if/#else because the value might
// be too large for the preprocessor
#    if UINT_MAX <= 0xFFFFFFFFFFFFFFFFFFFFFFFFU
#     define UINT_WIDTH 96
#     define INT_WIDTH 96
#    else
// This doesn't use #elif but nested #if/#else because the value might
// be too large for the preprocessor
#     if UINT_MAX <= 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFU
#      define UINT_WIDTH 128
#      define INT_WIDTH 128
#     endif
#    endif
#   endif
#  endif
# endif
#endif

// size_t has a width of at least 16.
#ifndef SIZE_WIDTH
# if SIZE_MAX <= 0xFFFFU
#  define SIZE_WIDTH 16
# else
#  if SIZE_MAX <= 0xFFFFFFFFU
#   define SIZE_WIDTH 32
#  else
#   if SIZE_MAX <= 0xFFFFFFFFFFFFFFFFU
#    define SIZE_WIDTH 64
#   else
// This doesn't use #elif but nested #if/#else because the value might
// be too large for the preprocessor
#    if SIZE_MAX <= 0xFFFFFFFFFFFFFFFFFFFFFFFFU
#     define SIZE_WIDTH 96
#    else
// This doesn't use #elif but nested #if/#else because the value might
// be too large for the preprocessor
#     if SIZE_MAX <= 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFU
#      define SIZE_WIDTH 128
#     endif
#    endif
#   endif
#  endif
# endif
#endif

// unsigned long has a width of at least 32
#ifndef ULONG_WIDTH
# if ULONG_MAX <= 0xFFFFFFFFUL
#  define ULONG_WIDTH 32
#  define LONG_WIDTH 32
# else
#  if ULONG_MAX <= 0xFFFFFFFFFFFFFFFFUL
#   define ULONG_WIDTH 64
#   define LONG_WIDTH 64
#  else
// This doesn't use #elif but nested #if/#else because the value might
// be too large for the preprocessor
#   if ULONG_MAX <= 0xFFFFFFFFFFFFFFFFFFFFFFFFUL
#    define ULONG_WIDTH 96
#    define LONG_WIDTH 96
#   else
// This doesn't use #elif but nested #if/#else because the value might
// be too large for the preprocessor
#    if ULONG_MAX <= 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFUL
#     define ULONG_WIDTH 128
#     define LONG_WIDTH 128
#    endif
#   endif
#  endif
# endif
#endif

// unsigned long long has a width of at least 64.
#ifndef ULLONG_WIDTH
# if ULLONG_MAX <= 0xFFFFFFFFFFFFFFFFULL
#  define ULLONG_WIDTH 64
#  define LLONG_WIDTH 64
# else
// This doesn't use #elif but nested #if/#else because the value might
// be too large for the preprocessor
#  if ULLONG_MAX <= 0xFFFFFFFFFFFFFFFFFFFFFFFFULL
#   define ULLONG_WIDTH 96
#   define LLONG_WIDTH 96
#  else
// This doesn't use #elif but nested #if/#else because the value might
// be too large for the preprocessor
#   if ULLONG_MAX <= 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFULL
#    define ULLONG_WIDTH 128
#    define LLONG_WIDTH 128
#   endif
#  endif
# endif
#endif

// In most cases the [u]intmax_t types will just be the long long
// types.
#ifndef UINTMAX_WIDTH
# if UINTMAX_MAX == ULLONG_MAX
#  define UINTMAX_WIDTH ULLONG_WIDTH
#  define INTMAX_WIDTH  LLONG_WIDTH
# endif
#endif

#ifndef UINT128_MAX
# ifdef __SIZEOF_INT128__
#  if defined(__clang_major__) && (__clang_major__ < 18)
#   ifndef C23_FALLBACK_SILENT
#    warning "not using compiler intrinsic for int128_t, please upgrade to clang version ≥ 18"
#   endif
#  else
#   define __has_int128
#  endif
# endif
#endif

#ifndef UINT256_MAX
# ifdef __SIZEOF_INT256__
#  if defined(__clang_major__) && (__clang_major__ < 18)
#   ifndef C23_FALLBACK_SILENT
#    warning "not using compiler intrinsic for int256_t, please upgrade to clang version ≥ 18"
#   endif
#  else
#   define __has_int256
#  endif
# endif
#endif

// In C23 uint128_t may exist, even if it is wider than
// uintmax_t. Provide it if the compiler supports it.
#ifndef UINT128_MAX
# ifdef __has_int128
typedef signed __int128 int128_t;
typedef unsigned __int128 uint128_t;
#  define UINT128_WIDTH 128
#  define INT128_WIDTH  128
#  define UINT128_MAX   ((uint128_t)-1)
#  define INT128_MAX    ((int128_t)(((uint128_t)-1)>>1))
#  define INT128_MIN    (-INT128_MAX-1)
#  if BITINT_MAXWIDTH >= 128
#   define UINT128_C(X) ((uint128_t)+(X ## wbu))
#   define INT128_C(X)  ((int128_t)+(X ## wb))
#  endif
# endif
#endif

// In C23 uint256_t may exist, even if it is wider than
// uintmax_t. Provide it if the compiler supports it.
#ifndef UINT256_MAX
# ifdef __has_int256
typedef signed __int256 int256_t;
typedef unsigned __int256 uint256_t;
#  define UINT256_WIDTH 256
#  define INT256_WIDTH  256
#  define UINT256_MAX   ((uint256_t)-1)
#  define INT256_MAX    ((int256_t)(((uint256_t)-1)>>1))
#  define INT256_MIN    (-INT256_MAX-1)
#  if BITINT_MAXWIDTH >= 256
#   define UINT256_C(X) ((uint256_t)+(X ## wbu))
#   define INT256_C(X)  ((int256_t)+(X ## wb))
#  endif
# endif
#endif

/**********************************************************************************************/

#if __STDC_VERSION_INTTYPES_H__ < 202311L
# ifndef C23_FALLBACK_SILENT
#  warning "native C23 library support for %b printf or scanf formats may be missing"
# endif
# ifndef PRIb32
#  if __GLIBC__ > 1 && defined(__PRI64_PREFIX)
#   ifndef C23_FALLBACK_SILENT
#    warning "no C23 support for %b printf formats found, emulating with glibc features"
#   endif
#   define PRIB16      "B"
#   define PRIB32      "B"
#   define PRIB64      __PRI64_PREFIX "B"
#   define PRIB8       "B"
#   define PRIBFAST16  __PRIPTR_PREFIX "B"
#   define PRIBFAST32  __PRIPTR_PREFIX "B"
#   define PRIBFAST64  __PRI64_PREFIX "B"
#   define PRIBFAST8   "B"
#   define PRIBLEAST16 "B"
#   define PRIBLEAST32 "B"
#   define PRIBLEAST64 __PRI64_PREFIX "B"
#   define PRIBLEAST8  "B"
#   define PRIBMAX     __PRI64_PREFIX "B"
#   define PRIBPTR     __PRIPTR_PREFIX "B"
#   define PRIb16      "b"
#   define PRIb32      "b"
#   define PRIb64      __PRI64_PREFIX "b"
#   define PRIb8       "b"
#   define PRIbFAST16  __PRIPTR_PREFIX "b"
#   define PRIbFAST32  __PRIPTR_PREFIX "b"
#   define PRIbFAST64  __PRI64_PREFIX "b"
#   define PRIbFAST8   "b"
#   define PRIbLEAST16 "b"
#   define PRIbLEAST32 "b"
#   define PRIbLEAST64 __PRI64_PREFIX "b"
#   define PRIbLEAST8  "b"
#   define PRIbMAX     __PRI64_PREFIX "b"
#   define PRIbPTR     __PRIPTR_PREFIX "b"
#   define SCNb16      "hb"
#   define SCNb32      "b"
#   define SCNb64      __PRI64_PREFIX "b"
#   define SCNb8       "hhb"
#   define SCNbFAST16  __PRIPTR_PREFIX "b"
#   define SCNbFAST32  __PRIPTR_PREFIX "b"
#   define SCNbFAST64  __PRI64_PREFIX "b"
#   define SCNbFAST8   "hhb"
#   define SCNbLEAST16 "hb"
#   define SCNbLEAST32 "b"
#   define SCNbLEAST64 __PRI64_PREFIX "b"
#   define SCNbLEAST8  "hhb"
#   define SCNbMAX     __PRI64_PREFIX "b"
#   define SCNbPTR     __PRIPTR_PREFIX "b"
#  else
#   ifndef C23_FALLBACK_SILENT
#    warning "no C23 support for %b printf formats found"
#   endif
#  endif
# endif
#endif

/**********************************************************************************************/

// Complex arithmetic without <complex.h> header
#ifndef __STDC_NO_COMPLEX__

// gcc and related compilers implement an "I" or "i" suffix for
// floating point constants since ages.
# ifdef __GNUC__
#  define I (__extension__ 1.0IF)
#  define _COMPLEX_I (0.0F + I)

// Otherwise use the new constexpr construct to produce a named
// constant of complex type.
# else
typedef union _C23_complex _C23_complex;
union _C23_complex {
  // These two are guaranteed to have the same representation.
  float _C23_v[2];
  _Complex float _C23_c;
};
// Use static const. Best would be to have constexpr, but using a
// different member than was initialized is not defined for them,
// unfortunately.
static _C23_complex const _C23_I = { { 0.0F, 1.0F, }, };
#  define _COMPLEX_I (_C23_I._C23_c)
# endif

#define iscomplex(X) (sizeof(1.0F*(X)) == sizeof(1.0F*(X)+_COMPLEX_I))
#define isimaginary(X) (sizeof(1.0F*(X)+0.0F) == 2*sizeof(1.0F*(X)))

# define _C23_float_Complex_CASE float _Complex: true,
# define _C23_double_Complex_CASE double _Complex: true,
# define _C23_long_double_Complex_CASE long double _Complex: true,
#else
# define _C23_float_Complex_CASE
# define _C23_double_Complex_CASE
# define _C23_long_double_Complex_CASE
#endif

/**********************************************************************************************/

// Testing for presence of decimal floating types

#if defined(__DEC32_MAX__)
# define _C23_Decimal32_CASE _Decimal32: true,
#else
# define _C23_Decimal32_CASE
#endif

#if defined(__DEC64_MAX__)
# define _C23_Decimal64_CASE _Decimal64: true,
#else
# define _C23_Decimal64_CASE
#endif

#if defined(__DEC128_MAX__)
# define _C23_Decimal128_CASE _Decimal128: true,
#else
# define _C23_Decimal128_CASE
#endif

#define isdecimalfloating(...) (_Generic((__VA_ARGS__)+0, _C23_Decimal32_CASE _C23_Decimal64_CASE _C23_Decimal128_CASE default: false))
#ifndef iscomplex
# define iscomplex(...) (_Generic((__VA_ARGS__)+0, _C23_float_Complex_CASE _C23_double_Complex_CASE _C23_long_double_Complex_CASE default: false))
#endif
#define isstandardrealfloating(...) (_Generic((__VA_ARGS__)+0, float: true, double: true, long double: true, default: false))
#define isstandardfloating(...) ((bool)(isstandardrealfloating(__VA_ARGS__)||iscomplex(__VA_ARGS__)))
#ifndef isfloating
# define isfloating(...) ((bool)(isstandardfloating(__VA_ARGS__)||isdecimalfloating(__VA_ARGS__)))
#endif

/**********************************************************************************************/

// various tg-conversions and type traits

#ifndef totype
# define totype(Y, ...) ((typeof(__VA_ARGS__))Y)
#endif
#ifndef tominusone
# define tominusone(...) totype(-1, __VA_ARGS__)
#endif
#ifndef tozero
# define tozero(...) totype(0, __VA_ARGS__)
#endif
#ifndef toone
# define toone(...) totype(1, __VA_ARGS__)
#endif
#ifndef iscompatible
# define iscompatible(X, Y) (_Generic((X), typeof(Y): true, default: false))
#endif
#ifndef is_potentially_negative
# define is_potentially_negative(...) (tominusone(__VA_ARGS__) < 0)
#endif
#ifndef is_const_target
# define is_const_target(...) (_Generic((1 ? (__VA_ARGS__) : (void*)1), void const*: true, void const volatile*: true, default: false))
#endif
#ifndef is_volatile_target
# define is_volatile_target(...) (_Generic((1 ? (__VA_ARGS__) : (void*)1), void volatile*: true, void const volatile*: true, default: false))
#endif
#ifndef is_const
# define is_const(...) is_const_target(&(typeof(__VA_ARGS__)){ 0 })
#endif
#ifndef is_volatile
# define is_volatile(...) is_volatile_target(&(typeof(__VA_ARGS__)){ 0 })
#endif
#ifndef is_null_pointer_constant
struct do_not_use_this_otherwise;
# define is_null_pointer_constant(...)                                    \
  (_Generic((1 ? (struct do_not_use_this_otherwise*)nullptr : (__VA_ARGS__)),     \
            struct do_not_use_this_otherwise*: true,                    \
            default: false))
#endif
#ifndef is_zero_ice
# define is_zero_ice(...) is_null_pointer_constant((void*)(uintptr_t)(!!(__VA_ARGS__)))
#endif
#ifndef isinteger
# define isinteger(...) is_zero_ice(tozero(__VA_ARGS__))
#endif
#ifndef issigned
# define issigned(...) ((bool)(is_potentially_negative(__VA_ARGS__) && isinteger(__VA_ARGS__) && !iscompatible(__VA_ARGS__, char)))
#endif
#ifndef isunsigned
# define isunsigned(...) ((bool)(!is_potentially_negative(__VA_ARGS__) && isinteger(__VA_ARGS__) && !iscompatible(__VA_ARGS__, char)))
#endif
#ifndef isice
# define isice(...) is_zero_ice(!((__VA_ARGS__) || 1))
#endif
#ifndef isvla
# define isvla(...) ((bool)!isice(sizeof(__VA_ARGS__)))
#endif
#ifndef isxwide
# define isxwide(...)                           \
  ((bool)(                                      \
          isinteger(__VA_ARGS__)                \
          &&_Generic((__VA_ARGS__)+0ULL,        \
                     unsigned long long: false, \
                     default: true)))
#endif

#ifndef is_pointer
struct do_not_use_this_otherwise { char c; };
# define get_fla(...)                                                   \
  GENERIC_IF(isvla(__VA_ARGS__),                                        \
             (struct do_not_use_this_otherwise[1]){ 0 },                \
             (__VA_ARGS__))
# define is_pointer_nvla(...)                           \
  (_Generic((typeof(__VA_ARGS__)*)0,                    \
            typeof(get_fla(*(__VA_ARGS__)))**: true,    \
            default: false))
# define is_pointer_vla(...)                                        \
  (_Generic((typeof(get_fla(*(__VA_ARGS__)))*)0,                    \
            typeof(struct do_not_use_this_otherwise[1])*: true,     \
            default: false))
# define is_pointer(...) ((bool)(is_pointer_nvla(__VA_ARGS__)||is_pointer_vla(__VA_ARGS__)))
#endif
#ifndef is_array
# define is_array(...) ((bool)!is_pointer(__VA_ARGS__))
#endif
#ifndef is_fla
# define is_fla(...) ((bool)(is_array(__VA_ARGS__)&&!isvla(__VA_ARGS__)))
#endif

#ifndef is_void_pointer
# define is_void_pointer(...)                                   \
  _Generic((typeof(*(__VA_ARGS__))const volatile*)nullptr,      \
           void const volatile*: true,                          \
           default: false)
#endif

/**********************************************************************************************/

/* Const preserving functions as of C23. */

/* These are not exactly according to the book, because the return
   type of the string functions could sometimes be a pointer to a void
   type and not a character type.*/

#if __STDC_VERSION_STRING_H__ < 202311L

//QVoid *memchr(QVoid *s, int c, size_t n);
# ifndef memchr
#  define memchr(S, C, N) ((typeof(1 ? S : (void*)1))memchr((S), (C), (N)))
# endif

// QChar *strchr(QChar *s, int c);
# ifndef strchr
#  define strchr(S, C) ((typeof(S))strchr((S), (C)))
# endif

// QChar *strpbrk(QChar *s1, const char *s2);
# ifndef strpbrk
#  define strpbrk(S1, S2) ((typeof(S1))strpbrk((S1), (S2)))
# endif

// QChar *strrchr(QChar *s, int c);
# ifndef strrchr
#  define strrchr(S, C) ((typeof(S))strrchr((S), (C)))
# endif

// QChar *strstr(QChar *s1, const char *s2);
# ifndef strstr
#  define strstr(S1, S2) ((typeof(S1))strstr((S1), (S2)))
# endif

#endif

#if __STDC_VERSION_WCHAR_H__ < 202311L

//QWchar *wmemchr(QWchar *s, int c, size_t n);
# ifndef wmemchr
#  define wmemchr(S, C, N) ((typeof(S))wmemchr((S), (C), (N)))
# endif

// QWchar *wcschr(QWchar *s, wchar_t c);
# ifndef wcschr
#  define wcschr(S, C) ((typeof(S))wcschr((S), (C)))
# endif

// QWchar *wcspbrk(QWchar *s1, const wchar_t *s2);
# ifndef wcspbrk
#  define wcspbrk(S1, S2) ((typeof(S1))wcspbrk((S1), (S2)))
# endif

// QWchar *wcsrchr(QWchar *s, wchar_t c);
# ifndef wcsrchr
#  define wcsrchr(S, C) ((typeof(S))wcsrchr((S), (C)))
# endif

// QWchar *wcswcs(QWchar *s1, const wchar_t *s2);
# ifndef wcsstr
#  define wcsstr(S1, S2) ((typeof(S1))wcsstr((S1), (S2)))
# endif

#endif

#if __STDC_VERSION_STDLIB_H__ < 202311L

// QVoid *bsearch(const void *key, QVoid *base, size_t nmemb, size_t size,
//               int (*compar)(const void *, const void *));
# ifndef bsearch
#  define bsearch(KEY, BASE, NMEMB, SIZE, COMPAR) ((typeof(1 ? (BASE) : (void*)1))bsearch((KEY), (BASE), (NMEMB), (SIZE), (COMPAR)))
# endif

#endif

#endif

/**********************************************************************************************/

#ifdef __has_include
# if __has_include(<stdbit.h>)
#  include <stdbit.h>
#  define __has_feature_c_stdbit_h ,
# endif
#endif

#ifndef __STDC_ENDIAN_LITTLE__
# ifdef __GNUC__
#  define __STDC_ENDIAN_LITTLE__ __ORDER_LITTLE_ENDIAN__
# endif
#endif

#ifndef __STDC_ENDIAN_BIG__
# ifdef __GNUC__
#  define __STDC_ENDIAN_BIG__ __ORDER_BIG_ENDIAN__
# endif
#endif

#ifndef __STDC_ENDIAN_NATIVE__
# ifdef __GNUC__
#  define __STDC_ENDIAN_NATIVE__ __BYTE_ORDER__
# else
#  ifndef C23_FALLBACK_SILENT
#   warning "native endian could not be determined"
#  endif
# endif
#endif

/**********************************************************************************************/

/* This adds the 14 type-generic bit interfaces that are added by
   C23. The use of the other type-specific interfaces is of less
   interest.

generic_return_type stdc_leading_zeros(generic_value_type value);
generic_return_type stdc_leading_ones(generic_value_type value);
generic_return_type stdc_trailing_zeros(generic_value_type value);
generic_return_type stdc_trailing_ones(generic_value_type value);
generic_return_type stdc_first_leading_zero(generic_value_type value);
generic_return_type stdc_first_leading_one(generic_value_type value);
generic_return_type stdc_first_trailing_zero(generic_value_type value);
generic_return_type stdc_count_zeros(generic_value_type value);
generic_return_type stdc_count_ones(generic_value_type value);
bool stdc_has_single_bit(generic_value_type value);
generic_return_type stdc_bit_width(generic_value_type value);
generic_value_type stdc_bit_floor(generic_value_type value);
generic_value_type stdc_bit_ceil(generic_value_type value);

*/

/*
  An auxiliary macro that shifts right by ULLONG_WIDTH, regardless
  whether the type has a width smaller than that. For most types this
  will just be a zero of the same type.
 */

#define shift_xright(...)                                       \
  GENERIC_IF(isxwide(__VA_ARGS__),                              \
             (__VA_ARGS__)>>GENERIC_IF(isxwide(__VA_ARGS__),    \
                                       ULLONG_WIDTH,            \
                                       0),                      \
             tozero(__VA_ARGS__))

#ifndef stdc_count_ones
# ifdef __GNUC__

#  define stdc_trailing_zeros(...)                                      \
  ({                                                                    \
    auto s_t_z_x = (__VA_ARGS__);                                       \
    static_assert(isunsigned(s_t_z_x), "bit operation needs unsigned type"); \
    (s_t_z_x)                                                           \
      ? _Generic((s_t_z_x),                                             \
                 bool:               __builtin_ctz((unsigned)s_t_z_x),  \
                 unsigned char:      __builtin_ctz((unsigned)s_t_z_x),  \
                 unsigned short:     __builtin_ctz((unsigned)s_t_z_x),  \
                 unsigned:           __builtin_ctz(s_t_z_x),            \
                 unsigned long:      __builtin_ctzl(s_t_z_x),           \
                 unsigned long long: __builtin_ctzll(s_t_z_x),          \
                 default: ({                                            \
                     size_t s_t_z_ret = 0;                              \
                     while (s_t_z_x) {                                  \
                       unsigned long long s_t_z_z = s_t_z_x;            \
                       if (s_t_z_z) {                                   \
                         s_t_z_ret += __builtin_ctzll(s_t_z_z);         \
                         s_t_z_x = 0;                                   \
                       } else {                                         \
                         s_t_z_ret += ULLONG_WIDTH;                     \
                         s_t_z_x = shift_xright(s_t_z_x);               \
                       }                                                \
                     }                                                  \
                     s_t_z_ret;                                         \
                   }))                                                  \
      : stdc_count_ones(tominusone(s_t_z_x));                           \
  })

/* This counts the least significant 1-bits of the specific type of the
   argument. Therefor we complement all bits. To take narrow types
   into account, that result has to be cast back to the original
   type. */
#  define stdc_trailing_ones(...) stdc_trailing_zeros((typeof(__VA_ARGS__))~(__VA_ARGS__))

#  define stdc_leading_zeros(...)                                       \
  ({                                                                    \
    auto s_l_o_x = (__VA_ARGS__);                                       \
    static_assert(isunsigned(s_l_o_x), "bit operation needs unsigned type"); \
    (s_l_o_x)                                                           \
      ? _Generic((s_l_o_x),                                             \
                 unsigned char:      __builtin_clz((unsigned)s_l_o_x) - __builtin_clz((unsigned)tominusone(s_l_o_x)), \
                 unsigned short:     __builtin_clz((unsigned)s_l_o_x) - __builtin_clz((unsigned)tominusone(s_l_o_x)), \
                 unsigned:           __builtin_clz(s_l_o_x),            \
                 unsigned long:      __builtin_clzl(s_l_o_x),           \
                 unsigned long long: __builtin_clzll(s_l_o_x),          \
                 default:                                               \
                 ((!isxwide(s_l_o_x))                                   \
                  /* Should only trigger for _BitInt types. */          \
                  ? (__builtin_clzll(s_l_o_x) - __builtin_clzll(tominusone(s_l_o_x))) \
                  /* determine the long long word with highest 1-bit . */ \
                  : ({                                                  \
                      size_t s_l_o_w = stdc_count_ones(tominusone(s_l_o_x)) - ULLONG_WIDTH; \
                      while ((s_l_o_x+0UL) > ULLONG_MAX) {              \
                        s_l_o_x = shift_xright(s_l_o_x);                \
                        s_l_o_w -= ULLONG_WIDTH;                        \
                      }                                                 \
                      __builtin_clzll(s_l_o_x) + s_l_o_w;               \
                    })))                                                \
      /* This should resolve to a compile time constant. */             \
      : stdc_count_ones(tominusone(s_l_o_x));                           \
  })


/* This counts the most significant 1-bits of the specific type of the
   argument. Therefor we complement all bits. To take narrow types
   into account, that result has to be cast back to the original
   type. */
#  define stdc_leading_ones(...) stdc_leading_zeros((typeof(__VA_ARGS__))~(__VA_ARGS__))


#  define stdc_count_ones(...)                                          \
  _Generic((__VA_ARGS__),                                               \
           bool:               __builtin_popcount((unsigned)(__VA_ARGS__)), \
           unsigned char:      __builtin_popcount((unsigned)(__VA_ARGS__)), \
           unsigned short:     __builtin_popcount((unsigned)(__VA_ARGS__)), \
           unsigned:           __builtin_popcount((unsigned)(__VA_ARGS__)), \
           unsigned long:      __builtin_popcountl((unsigned long)(__VA_ARGS__)), \
           unsigned long long: __builtin_popcountll((unsigned long long)(__VA_ARGS__)), \
           default: ({                                                  \
               auto s_c_o_x = (__VA_ARGS__);                            \
               static_assert(isunsigned(s_c_o_x), "bit operation needs unsigned type"); \
               size_t s_c_o_ret = 0;                                    \
               while (s_c_o_x) {                                        \
                 s_c_o_ret += __builtin_popcountll((unsigned long long)s_c_o_x); \
                 s_c_o_x = shift_xright(s_c_o_x);                       \
               }                                                        \
               s_c_o_ret;                                               \
             }))


/* This counts the 0-bits of the specific type of the
   argument. Therefor we complement all bits. To take narrow types
   into account, that result has to be cast back to the original
   type. */
#  define stdc_count_zeros(...) stdc_count_ones((typeof(__VA_ARGS__))~(__VA_ARGS__))

#  define stdc_has_single_bit(...)                                      \
  ({                                                                    \
    auto s_h_s_b_x = (__VA_ARGS__);                                     \
    static_assert(isunsigned(s_h_s_b_x), "bit operation needs unsigned type"); \
    (bool)(s_h_s_b_x && !((s_h_s_b_x != 1) && (s_h_s_b_x & (s_h_s_b_x - 1)))); \
  })

#  define stdc_bit_width(...)                                           \
  ({                                                                    \
  auto s_b_w_x = (__VA_ARGS__);                                         \
  static_assert(isunsigned(s_b_w_x), "bit operation needs unsigned type"); \
  stdc_count_ones(tominusone(s_b_w_x))-stdc_leading_zeros(s_b_w_x);     \
  })

#  define stdc_bit_floor(...)                                           \
  ({                                                                    \
  auto s_b_f_x = (__VA_ARGS__);                                         \
  static_assert(isunsigned(s_b_f_x), "bit operation needs unsigned type"); \
  (!s_b_f_x)                                                            \
    ? 0                                                                 \
    : (toone(s_b_f_x)<<(stdc_bit_width(s_b_f_x)-1));                    \
  })

#  define stdc_bit_ceil(...)                            \
  (toone(__VA_ARGS__)<<stdc_bit_width(__VA_ARGS__))

# endif
#endif
