#include "c23-fallback.h"
#include <errno.h>
//#include <limits.h>
#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <assert.h>
/* Supposes that lowercase characters are contiguous. */
static_assert('z'-'a' == 25,
               "alphabetic characters not contiguous");
#include <ctype.h>
/* Converts an alphanumeric digit to an unsigned */
/* '0' ... '9'  =>  0 ..  9u */
/* 'A' ... 'Z'  => 10 .. 35u */
/* 'a' ... 'z'  => 10 .. 35u */
/* Other values =>   Greater */
unsigned hexatridecimal(int a) {
  if (isdigit(a)) {
    /* This is guaranteed to work: decimal digits
       are consecutive, and isdigit is not
       locale dependent. */
    return a - '0';
  } else {
    /* Leaves a unchanged if it is not lowercase */
    a = toupper(a);
    /* Returns value >= 36 if not Latin uppercase */
    return (isupper(a)) ? 10 + (a - 'A') : -1;
  }
}

unsigned long Strtoul_inner(char const s[static 1],
                            size_t i,
                            unsigned base) {
  unsigned long ret = 0;
  while (s[i]) {
    unsigned c = hexatridecimal(s[i]);
    if (c >= base) break;
    /* Maximal representable value for 64 bit is
       3w5e11264sgsf in base 36 */
    if (ULONG_MAX/base < ret) {
      ret = ULONG_MAX;
      errno = ERANGE;
      break;
    }
    ret *= base;
    ret += c;
    ++i;
  }
  return ret;
}

static unsigned find_prefix(char const s[static 1], size_t i, char const t[static 1]) {
  size_t ret = 0;
  while (s[i+ret] && (tolower(s[i+ret]) == tolower(t[ret]))) {
    ++ret;
  }
  return ret;
}

unsigned long Strtoul(char const s[static 1], unsigned base) {
  if (base > 36u) {             /* Tests if base          */
    errno = EINVAL;             /* Extends the specification */
    return ULONG_MAX;
  }
  size_t i = strspn(s, " \f\n\r\t\v"); /* Skips spaces    */
  bool switchsign = false;      /* Looks for a sign       */
  switch (s[i]) {
  case '-' :
    switchsign = true;
    [[fallthrough]];
  case '+' :
    ++i;
  }
  /* Test if there is anything left in the string. */
  if (!s[i]) return 0;
  if (!base || base == 16 || base == 2) { /* use prefix */
    size_t adj = find_prefix(s, i, "0x");
    switch (adj) {
    case 2:                   // There is a 0x or a 0X prefix
      if (!base || base == 16) base = 16;
      // If we are looking for another base, the x is the end
      else return 0;
      break;
    case 1:                    // There is a 0 prefix
      adj = find_prefix(s, i, "0b");
      switch (adj) {
      case 1:                  // There is only a 0 prefix
        if (!base) base = 8;
        break;
      default:                 // There is a 0b or a 0B prefix
        if (!base || base == 2) base = 2;
        // If we are looking for another base, the b is the end
        else return 0;
        break;
      }
      break;
    default:
      if (!base) base = 10;
      break;
    }
    i += adj;
  }
  /* Test again, maybe the prefix 0 was the only digit. */
  if (!s[i]) return 0;
  /* Now, starts the real conversion */
  unsigned long ret = Strtoul_inner(s, i, base);
  return (switchsign) ? -ret : ret;
}

char* char36(unsigned long n, unsigned base, char buf[static 256]) {
  if (base > 36) {
    strcpy(buf, "EINVAL");
  } else {
    unsigned i = 256;
    while (n) {
      --i;
      buf[i] = n % base;
      n /= base;
      if (buf[i] < 10) {
        buf[i] += '0';
      } else {
        buf[i] += 'a' - 10;
      }
    }
    memmove(buf, &buf[i], 256-i);
  }
  return buf;
}

int main(int argc, char* argv[argc+1]) {
  static unsigned const code[] = { 0, 8, 10, 16, 36, 37, };
  for (unsigned j = 0; j < sizeof code/ sizeof code[0]; ++j) {
    printf("%u:", code[j]);
    for (int i = 1; i < argc; ++i) {
      unsigned long res = Strtoul(argv[i], code[j]);
      if (res == ULONG_MAX && errno) {
        puts("");
        fprintf(stderr, "invalid conversion with base %u and \"%s\"\n", code[j], argv[i]);
        perror("error");
        errno = 0;
      } else {
        printf("\t%lu", res);
      }
    }
    puts("");
    if (code[j]) printf("%u:\tULONG_MAX %s\n", code[j], char36(ULONG_MAX, code[j], (char[256]){ }));
  }
  return EXIT_SUCCESS;
}
