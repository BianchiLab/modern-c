#include <stdio.h>
#include "c23-fallback.h"

#ifndef BITINT_MAXWIDTH
# error "no _BitInt support found, aborting compilation"
// shut off some subsequent error messages
# include "no _BitInt support found, aborting compilation"
#endif

int main (int argc, char* argv[static argc+1]) {
  // this doesn't fit, diagnose?
  _BitInt(15)          s15 = 0b1010101'01010101wb;
  unsigned _BitInt(15) u15 = 0b1010101'01010101wbu;
  // this doesn't fit, diagnose?
  _BitInt(15) su15 = u15;

  auto u65 = 0x1'FFFF'FFFF'FFFF'FFFFwbu;
  // wrap around arithmetic is fine.
  auto oa65 = 0x1'FFFF'FFFF'FFFF'FFFFwbu + 1wbu;
  // constant too wide, diagnostic?
  typeof(u65) o65 = 0x2'FFFF'FFFF'FFFF'FFFFwbu;

  auto s65 = 0xFFFF'FFFF'FFFF'FFFFwb;
  // signed arithmetic overflows, diagnostic?
  auto sa65 = 0xFFFF'FFFF'FFFF'FFFFwb + 1wb;
  // constant too wide, diagnostic?
  typeof(s65) os65 = 0x1'FFFF'FFFF'FFFF'FFFFwb;

  unsigned _BitInt(128) max128s = 0x7FFF'FFFF'FFFF'FFFF'FFFF'FFFF'FFFF'FFFFwb;
  unsigned _BitInt(128) min128s = -1wb -max128s;

  unsigned _BitInt(3) max3u = -1;
  unsigned _BitInt(4) max4u = -1;
  unsigned _BitInt(4) one4u = max4u - max3u;
    signed _BitInt(4) max4s = max3u;
    signed _BitInt(4) min4s = ~max4s;

  printf("15 bits, signed:\t%d\n", (signed)s15);
  printf("15 bits, unsigned:\t%u\n", (unsigned)u15);
  printf("15 bits, u → s:\t%d\n", (signed)su15);
  printf("4 bits:\t%u\n", (unsigned)max4u);
  printf("4 bits:\t%u\n", (unsigned)one4u);
  printf("4 bits:\t%d\n", (signed)max4s);
  printf("4 bits:\t%d\n", (signed)min4s);
  printf("4 bit arithm:\t%u\n", (unsigned)(max4u + 1wbu));
  printf("int arithm:\t%u\n", (unsigned)(max4u + 1));
  return s15 + su15 + u15 + u65 + oa65 + o65 + s65 + sa65 + os65 + !max128s + !min128s;
}
