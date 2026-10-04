#include "c23-fallback.h"
#include <stdio.h>
#include <fenv.h>
#include <float.h>
#ifndef INFINITY
#include <math.h>
#endif

#if !defined(__STDC_IEC_559__) && !defined(__STDC_IEC_60559_BFP__) && !FE_ALL_EXCEPT
#error "floating-point arithmetic is too weird"
#endif

static_assert(FE_ALL_EXCEPT, "floating point exceptions are not supported");

#ifdef FENV_OFF
#warning "switching FENV_ACCESS off"
#pragma STDC FENV_ACCESS OFF
#else
#pragma STDC FENV_ACCESS ON
#endif

bool const has_inf =
#ifdef INFINITY
  (1.0/0.0 == INFINITY)
#else
  false
#endif
  ;

int excepts[] = {
#ifdef FE_DIVBYZERO
  FE_DIVBYZERO,
#endif
#ifdef FE_INEXACT
  FE_INEXACT,
#endif
#ifdef FE_INVALID
  FE_INVALID,
#endif
#ifdef FE_OVERFLOW
  FE_OVERFLOW,
#endif
#ifdef FE_UNDERFLOW
  FE_UNDERFLOW,
#endif
};

void printexcept(void) {
  char const* name[] = {
#ifdef FE_DIVBYZERO
    "divbyzero",
#endif
#ifdef FE_INEXACT
    "inexact",
#endif
#ifdef FE_INVALID
    "invalid",
#endif
#ifdef FE_OVERFLOW
    "overflow",
#endif
#ifdef FE_UNDERFLOW
    "underflow",
#endif
  };
  int except = fetestexcept(FE_ALL_EXCEPT);
  if (except) {
    printf("[");
    for (unsigned j = 0; except; except &= ~excepts[j], ++j)
      if (excepts[j] & except)
        printf("%s ", name[j]);
    printf("]");
  }
}

int main (int argc, char* argv[static argc+1]) {
  printf("division by zero is %sequal to INFINITY\n", has_inf ? "" : "un");
  for (unsigned i = 1; i < argc; i++) {
    feclearexcept(FE_ALL_EXCEPT);
    double x = strtod(argv[i], nullptr);
    printf("%g ", x);
    printexcept();
    feclearexcept(FE_ALL_EXCEPT);
    printf(": %g ", 1.0/x);
    printexcept();
    puts("");
  }
}
