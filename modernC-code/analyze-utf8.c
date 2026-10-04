#include "mbstrings.h"
#include <uchar.h>
#include <stdio.h>
#include <locale.h>

int main(void) {
  // Make sure to have the platform's mb encoding on input.
  setlocale(LC_CTYPE, "");
  // Holds the state of input/output buffering.
  mbstate_t st = { };
  // collects the input mb sequence
  char ib[23];
  // collects the current UTF-8 mb sequence
  char8_t ob[5] = { };
  // the number of input characters for the current code point
  size_t in = 0;
  while (fgets(ib, sizeof(ib), stdin)) {
    // Run through the current line. The last character is
    // always reserved for the string terminator.
    for (char* p = ib; (p-ib) < sizeof(ib)-1;) {
      size_t const n = sizeof(ib)-1-(p-ib);
      size_t const r = mbrtoc8(ob, p, n, &st);
      switch (r) {       // Handle the special cases.
      case mbincomplete: p += n; in += n;    continue;
      case 0: case mbstored: case mbinvalid: goto INVAL;
      }
      p += r; in += r;
      char8_t* cont = ob+1;  // first character is already stored
      while (mbrtoc8(cont, "", 1, &st) == mbstored) {
        cont++;
      }
      // Now we have the whole UTF-8. Analyze the result.
      printf("%s", ((cont-ob) == 1) ? "ASCII\t" : "UTF-8\t");
      for (char8_t* o = ob; o < cont; ++o) {
        printf("|%02hhx", *o);
      }
      // fgets stopped at an end of a line
      if (*ob == u8'\n') {
        puts("|\t~ eol");
        in = 0;
        break;
      } else if (in == (cont-ob)) {
        printf("|\t~ '%s'\n", ob);
      } else {
        printf("|\t%zu→%tu\n", in, (cont-ob));
      }
      in = 0;
    }
    if (*ob != u8'\n') {
      fputs("incomplete line\n", stderr);
    }
  }
  return EXIT_SUCCESS;
 INVAL:
  fputs("input error, exiting\n", stderr);
  return EXIT_FAILURE;
}
