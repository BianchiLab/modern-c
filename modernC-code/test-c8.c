#include <uchar.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>

void print_string(char const*s) {
  char const*str = s;
  size_t n = strlen(str)+1;
  char8_t buffer[n+1];
  printf("mb→utf8, source: '%s', sliced: ", str);
  mbstate_t mb = { 0 };
  for (char8_t *p = buffer; ; p++) {
    size_t res = mbrtoc8(p, str, n, &mb);
    if (res > -3) {
      printf("!error:%zu!\n", -res);
      return;
    } else if (res == -3) {
      printf(",%hhx", *p);
    } else if (res) {
      printf("|%hhx", *p);
      str += res;
    } else {
      printf("(%hhx)", *p);
      str++;
      break;
    }
  }
  printf("; target: %s\n", buffer);
  memset(buffer, 0, sizeof buffer);
  char *p = (char*)buffer;
  printf("utf8→mb, source: '%s', ", s);
  for (char const *q = s; ; q++) {
    size_t res = c8rtomb(p, *q, &mb);
    if (res) {
      switch (res) {
      default: printf("∞"); res = -1; break;
      case 4: printf("<-->"); break;
      case 3: printf("<->"); break;
      case 2: printf("<>"); break;
      case 1: printf("x"); break;
      }
      if (!*p || (res == -1)) break;
      else p += res;
    }
  }
  printf("; target: %s\n", buffer);
}

int main(int argc, char* argv[static argc+1]) {
  if (argc > 1)
    setlocale(LC_ALL, "");
  print_string(u8"abcdäα⌫");
  print_string(u8"→😇");
}
