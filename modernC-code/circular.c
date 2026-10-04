#include <stdlib.h>
#include <string.h>
#include "circular.h"

circular* circular_new(size_t len);
void circular_delete(circular* c);

#if __GNUC__ > 4 && __GNUC__ <= 14
# pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

[[deprecated("implementation")]]
circular* circular_init(circular* c, size_t cap) {
  if (c) {
    if (cap) {
      *c = (circular){
        .cap = cap,
        .tab = malloc(sizeof(double[cap])),
      };
        // Allocation failed.
      if (!c->tab) c->cap = 0;
    } else {
      *c = (circular){ };
    }
  }
  return c;
}

[[deprecated("implementation")]]
void circular_destroy(circular* c) {
  if (c) {
    free(c->tab);
    circular_init(c, 0);
  }
}

[[deprecated("implementation")]]
size_t circular_getlength(circular const* c) {
  return c ? c->len : 0;
}

[[deprecated("implementation")]]
static size_t circular_getpos(circular const c[static 1], size_t pos) {
  if (c->cap) {
    pos += c->start;
    pos %= c->cap;
  }
  return pos;
}

[[deprecated("implementation")]]
circular* circular_append(circular* c, double value) {
  if (c) {
    double* where = circular_element(c, c->len);
    if (where) {
      *where = value;
      ++c->len;
      return c;
    }
  }
  return nullptr;
}

[[deprecated("implementation")]]
double* circular_element(circular const* c, size_t pos) {
  double* ret = nullptr;
  if (c) {
    if (pos < c->cap) {
      pos = circular_getpos(c, pos);
      ret = &c->tab[pos];
    }
  }
  return ret;
}

[[deprecated("implementation")]]
double circular_pop(circular* c) {
  double ret = 0.0;
  if (c && c->len) {
    double* p = circular_element(c, 0);
    if (p) ret = *p;
    ++c->start;
    --c->len;
  }
  return ret;
}

[[nodiscard("returned pointer replaces function argument")]]
[[deprecated("implementation")]]
circular* circular_resize(circular* c, size_t nlen) {
  if (c) {
    size_t len = c->len;
    if (len > nlen) return nullptr;
    size_t olen = c->cap;
    if (nlen != olen) {
      size_t ostart = circular_getpos(c, 0);
      size_t nstart = ostart;
      double* otab = c->tab;
      double* ntab;
      if (nlen > olen) {
        ntab = realloc(c->tab, sizeof(double[nlen]));
        if (!ntab) return nullptr;
        // non-empty, and there were already two separate chunks
        if (ostart+len > olen) {
          size_t ulen = olen - ostart;
          size_t llen = len - ulen;
          if (llen <= (nlen - olen)) {
            /* Copy the lower one up after the old end. */ /*@\label{cpy-lowchunk}*/
            memcpy(ntab + olen, ntab,
                   llen*sizeof(double));
          } else {
            /* Move the upper one up to the new end. */ /*@\label{cpy-hichunk}*/
            nstart = nlen - ulen;
            memmove(ntab + nstart, ntab + ostart,
                    ulen*sizeof(double));
          }
        }
      } else {
        if (ostart+len > olen) {
          // Two separate chunks; mv the upper one down to the new end.
          size_t ulen = olen - ostart;
          nstart = nlen - ulen;
          memmove(otab + nstart, otab + ostart, ulen*sizeof(double));
        } else {
          // A single chunk
          if (ostart + len > nlen) {
            // Reallocation cuts the existing chunk in two.
            memmove(otab, otab + ostart, len*sizeof(double));
            nstart = 0;
          }
        }
        if (nlen) {
          // Now all data is saved in the conserved part of the array.
          ntab = realloc(c->tab, sizeof(double[nlen]));
          // If realloc fails in this case (would be weird), just overrule it.
          if (!ntab) ntab = otab;
        } else {
          // There had been no elements and we were asked to reduce
          // the buffer to 0 elements.
          free(c->tab);
          ntab = nullptr;
          nstart = 0;
        }
      }
      *c = (circular){
        .cap = nlen,
        .start = nstart,
        .len = len,
        .tab = ntab,
      };
    }
  }
  return c;
}

[[deprecated("implementation")]]
void circular_fput(circular* c, FILE* s) {
  if (c) {
    size_t len = circular_getlength(c);
    double* tab = c->tab;
    if (tab) {
      fprintf(s, "%p+%zu (%zu+%zu):", (void*)tab, c->cap, c->start, len);
      for (size_t i = 0; i < len; ++i) {
        double* elp = circular_element(c, i);
        // We know that c is not null, and so elp never will be.
        if (!elp) unreachable();
        fprintf(s, "\t%g", *elp);
      }
      fputc('\n', s);
      return;
    }
  }
  fputs("invalid circular\n", s);
}
