#ifndef __REORDER_H__
#define __REORDER_H__

#include <string.h>
#include <stdlib.h>

#define rodr_convert(T, ...) (T){ (__VA_ARGS__), }

[[__maybe_unused__]]
static inline
void* rodr_memcpy(size_t n,
                  unsigned char s1[restrict static n],
                  unsigned char const s2[restrict static n])
  [[__unsequenced__]];

[[__maybe_unused__]]
static inline
void* rodr_memccpy(size_t n,
                   unsigned char s1[restrict static n],
                   unsigned char const s2[restrict static n],
                   int c)
  [[__unsequenced__]];

[[__maybe_unused__]]
static inline
void* rodr_memmove(size_t n,
                   unsigned char s1[static n],
                   unsigned char const s2[static n]);

[[__maybe_unused__]]
static inline
int rodr_memcmp(size_t n,
                unsigned char s1[restrict static n],
                unsigned char const s2[restrict static n])
  [[__unsequenced__]];

[[__maybe_unused__]]
static inline
void* rodr_memchr(size_t n, unsigned char const s[static n], int c)
  [[__unsequenced__]];

[[__maybe_unused__]]
static inline
void* rodr_memset(size_t n, unsigned char s[static n], int c)
  [[__unsequenced__]];

[[__maybe_unused__]]
static inline
void rodr_qsort(size_t nmemb, size_t size,
                unsigned char base[static nmemb*size],
                int (*compar)(const void *, const void *));

[[__maybe_unused__]]
static inline
void* rodr_bsearch(size_t nmemb, size_t size,
                   unsigned char const key[static 1],
                   unsigned char const base[static nmemb*size],
                   int (*compar)(const void *, const void *));

/**
 ** @brief Similar to `memcpy` but checks the buffer argument for the
 ** size.
 **
 ** To be able to do that check, the function has the size argument
 ** first and then the buffer, so the buffer can be specified as array
 ** parameter that depends on the that size.
 **
 ** Here there is an additional complication, namely that the original
 ** functions has `void` pointers. We go around this by using
 ** `unsigned char` (which then can be the base of an array). Then, in
 ** the macro interface we have to convert the arguments to `void`
 ** pointers such that this function here can be called without
 ** troubles.
 **/
static inline
void* rodr_memcpy(size_t n,
                  unsigned char s1[restrict static n],
                  unsigned char const s2[restrict static n]) {
  // This captures a possible pre-existing macro for memcpy
  return memcpy(s1, s2, n);
}

static inline
void* rodr_memccpy(size_t n,
                   unsigned char s1[restrict static n],
                   unsigned char const s2[restrict static n],
                   int c) {
  // This captures a possible pre-existing macro for memccpy
  return memccpy(s1, s2, c, n);
}

static inline
void* rodr_memmove(size_t n,
                   unsigned char s1[static n],
                   unsigned char const s2[static n]) {
  // This captures a possible pre-existing macro for memmove
  return memmove(s1, s2, n);
}

static inline
int rodr_memcmp(size_t n,
                unsigned char s1[restrict static n],
                unsigned char const s2[restrict static n]) {
  // This captures a possible pre-existing macro for memcmp
  return memcmp(s1, s2, n);
}


static inline
void* rodr_memchr(size_t n, unsigned char const s[static n], int c) {
  // This avoids a possible pre-existing macro for memchr
  return (memchr)(s, c, n);
}

static inline
void* rodr_memset(size_t n, unsigned char s[static n], int c) {
  // This captures a possible pre-existing macro for memset
  return memset(s, c, n);
}


static inline
void rodr_qsort(size_t nmemb, size_t size,
                unsigned char base[static nmemb*size],
                int (*compar)(const void *, const void *)) {
  // This captures a possible pre-existing macro for qsort
  qsort(base, nmemb, size, compar);
}

static inline
void* rodr_bsearch(size_t nmemb, size_t size,
                   unsigned char const key[static 1],
                   unsigned char const base[static nmemb*size],
                   int (*compar)(const void *, const void *)) {
  // This avoids a possible pre-existing macro for bsearch
  return (bsearch)(key, base, nmemb, size, compar);
}

#undef memcpy
/**
 ** @brief A macro replacement for `memcpy` that also propagates the
 ** requirements for the size of the buffer arguments.
 **
 ** There is a complication, namely that the original functions has
 ** `void` pointers. We go around this by using `unsigned char` (which
 ** then can be the base of an array) in the `rodr_memcpy`
 ** interface. So here we have to convert the arguments to `void`
 ** pointers such that this function can be called without troubles.
 **
 ** We don't want to use simple casts to `void*`, for example, because
 ** that would cast away all other type checks that we still want to
 ** maintain. Therefore we use compound literals where the arguments
 ** are initializers. By that, only an argument type that has an
 ** implicit conversion to the corresponding `void` pointer type can
 ** be used, all others will be diagnosed.
 **/
#define memcpy(S1, S2, N)                               \
rodr_memcpy((N),                                        \
            /* Make sure no qualification gets lost */  \
            rodr_convert(void*, S1),                    \
            /* Make sure no volatile gets lost */       \
            rodr_convert(void const*, S2))

#undef memccpy
#define memccpy(S1, S2, C, N)                                   \
  rodr_memccpy((N),                                             \
               /* Make sure no qualification gets lost */       \
               rodr_convert(void*, S1),                         \
               /* Make sure no volatile gets lost */            \
               rodr_convert(void const*, S2),                   \
               (C))

#define memmove(S1, S2, N)                                      \
  rodr_memmove((N),                                             \
               /* Make sure no qualification gets lost */       \
               rodr_convert(void*, S1),                         \
               /* Make sure no volatile gets lost */            \
                 rodr_convert(void const*, S2))

#define memcmp(S1, S2, N)                                       \
  rodr_memcmp((N),                                              \
              /* Make sure no qualification gets lost */        \
              rodr_convert(void*, S1),                          \
              /* Make sure no volatile gets lost */             \
              rodr_convert(void const*, S2))


#undef memchr
#define memchr(S, C, N)                                 \
  ((typeof(1 ? (S) : (void*)1))                         \
   rodr_memchr((N),                                     \
               /* Make sure no volatile gets lost */    \
               rodr_convert(void const*, S),            \
               (C)))

#undef memset
#define memset(S, C, N)                                         \
  rodr_memset((N),                                              \
              /* Make sure no qualification gets lost */        \
              rodr_convert(void*, S),                           \
              (C))

#undef qsort
#define qsort(BASE, NMEMB, SIZE, COMPAR)                \
  rodr_qsort((NMEMB),                                   \
             (SIZE),                                    \
             /* Make sure no qualification gets lost */ \
             rodr_convert(void*, BASE),                 \
             (COMPAR))

#undef bsearch
#define bsearch(KEY, BASE, NMEMB, SIZE, COMPAR)         \
  ((typeof(1 ? (BASE) : (void*)1))                      \
  rodr_bsearch((NMEMB),                                 \
               (SIZE),                                  \
               /* Make sure no volatile gets lost */    \
               rodr_convert(void const*, KEY),          \
               /* Make sure no volatile gets lost */    \
               rodr_convert(void const*, BASE),         \
               (COMPAR)))

#if __STDC_VERSION__ >= 202311L

[[__maybe_unused__]]
static inline
void* rodr_memset_explicit(size_t n, unsigned char s[static n], int c)
  [[__unsequenced__]];

static inline
void* rodr_memset_explicit(size_t n, unsigned char s[static n], int c) {
  // This captures a possible pre-existing macro for memset_explicit
  return memset_explicit(s, c, n);
}

#undef memset_explicit
#define memset_explicit(S, C, N)                                        \
  rodr_memset_explicit((N),                                             \
                       /* Make sure no qualification gets lost */       \
                       rodr_convert(void*, S),                          \
                       (C))

#endif


#endif
