#include <stdlib.h>
#include <stdio.h>

#define testIt(FUNC, F, V)                      \
  do {                                          \
    char buf[256];                              \
    int ret = FUNC(buf, 256, (F), (V));         \
    printf("%d: %s\n", ret, buf);               \
  } while (0)

int main(void) {
  testIt(strfromd, "%g", 1.0/7);
  testIt(strfromd, "%.6766g", 1.0/7);
  testIt(strfromd, "%.0000000000000000000000000000000000000000000006766g", 1.0/7);
  testIt(strfroml, "%g", 1.0L/7);
  testIt(strfroml, "%.6766g", 1.0L/7);
  testIt(strfroml, "%.0000000000000000000000000000000000000000000006766g", 1.0L/7);
  testIt(strfroml, "%.547996766g", 1.0L/7);
  testIt(strfromd, "%a", 1.0/7);
  testIt(strfromd, "%.6766a", 1.0/7);
  testIt(strfromd, "%.0000000000000000000000000000000000000000000006766a", 1.0/7);
  testIt(strfroml, "%a", 1.0L/7);
  testIt(strfroml, "%.6766a", 1.0L/7);
  testIt(strfroml, "%.0000000000000000000000000000000000000000000006766a", 1.0L/7);
  testIt(strfroml, "%.547996766a", 1.0L/7);
}
