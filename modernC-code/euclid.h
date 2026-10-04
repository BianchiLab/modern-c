#ifndef EUCLID_H
# define EUCLID_H 1

# include "c23-fallback.h"
# include <stdio.h>
# include <assert.h>

inline size_t gcd2(size_t a, size_t b) [[__unsequenced__]] {
  assert(a <= b);                 /*@\label{gcd2-precondition}*/
  if (!a) return b;               /*@\label{gcd2-bottom}*/
  size_t rem = b % a;             /*@\label{gcd2-remainder}*/
  return gcd2(rem, a);            /*@\label{gcd2-recurse}*/
}

inline size_t gcd(size_t a, size_t b) [[__unsequenced__]] {
  assert(a);
  assert(b);
  if (a < b)
    return gcd2(a, b);
  else
    return gcd2(b, a);
}

#endif
