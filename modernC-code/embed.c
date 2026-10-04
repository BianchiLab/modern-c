/**
 ** @brief A toy example for a usage of the #embed directive.
 **
 ** We just embed the source of this program into the executable and
 ** print it.
 **
 ** If the #embed directive is entirely supported, the preprocessor
 ** should do macro expansion of `__FILE__`. If the directory of the
 ** source file is in the include path for embed.c, the source should
 ** just be included like that.
 **
 ** If the directive is not supported you could try to compile this
 ** anyhow by using https://sentido-labs.com/en/library/cedro/. Cedro
 ** does not allow to do macro expansion for the argument of the
 ** directive, so we have to give this explicitly.
 **
 ** For the example itself to output properly, the source and
 ** execution encoding should not be too different.
 **/

#include "c23-fallback.h"
#include <stdio.h>
#include <string.h>

// define a character array that will contain the entire
// source file
static char const here[] = {

// Cedro does not work with blanks between the # and any directive
#pragma Cedro 1.0 embed
#embed "embed.c"

};

// define another character array that has the same size
static char there[sizeof here];

int main(int argc, char* argv[static argc+1]) {
  size_t ibytes = 0;
  int cmp = 1000;
  // Open the file in binary mode.
  FILE* inp = fopen(__FILE__, "rb");
  if (inp) {
    // read the file as binary
    ibytes = fread(there, 1, sizeof there, inp);
    cmp = memcmp(here, there, sizeof here);
    fclose(inp);
  } else {
    printf("could not open %s\n", __FILE__);
    printf("+++++++++++++++++++++++++++++++++++++++++++++++\n");
  }
  size_t obytes = fwrite(here, 1, sizeof here, stdout);
  printf("+++++++++++++++++++++++++++++++++++++++++++++++\n");
  printf("in %zu, out %zu, bytes are %s\n",
         ibytes, obytes,
         cmp < 0 ? "smaller" : (cmp > 0 ? "greater" : "equal"));
}
