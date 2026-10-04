#include "c23-fallback.h"
#include <stdio.h>

typedef char const* key;


int comp_key(key const*a, key const*b) {
  key A = *a;
  key B = *b;
  return strcmp(A, B);
}

int comp(void const*a, void const*b) {
  key const*A = a;
  key const*B = b;
  return comp_key(A, B);
}

int main() {
  static key data[] = {
    "Tina",
    "Albert",
    "Xaver",
    "Albertina",
  };

  qsort(data, (sizeof data)/(sizeof data[0]), sizeof data[0], comp);
  for (size_t i = 0; i < (sizeof data)/(sizeof data[0]); ++i)
    printf("%zu: \"%s\"\n", i, data[i]);
  key k = "Tina";
  key* n1 = bsearch(&k, data, (sizeof data)/(sizeof data[0]), sizeof data[0], comp);
  printf("found: \"%s\"\n", n1 ? *n1 : "unknown");
  key const* d = data;
  key const* n2 = bsearch(&k, d, (sizeof data)/(sizeof data[0]), sizeof data[0], comp);
  printf("found: \"%s\"\n", n2 ? *n2 : "unknown");
  char buf[25] = { 0 };
  strfroml(buf, 25, "%.10g", 3744.0083383L);
  printf("strfroml %.10LG → \"%s\"\n", 3744.0083383L, buf);
  strfroml(buf, 25, "%.10Lg", 3744.0083383L);
  printf("invalid format leads to \"%s\"\n", buf);
  for (char const* p = buf; p < buf+16; ++p)
    printf("alignment %p: %zu, suitable for int*: %d\n", p, memalignment(p), memalignment(p)>=alignof(int));
  struct two {int a; int b; };
  assert((struct two){ .a = 0, .b = 1, }.b);
  assert((struct two){ .a = 0, .b = 1, }.a);
}
