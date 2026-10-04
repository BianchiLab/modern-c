#include "generic.h"

/* "Instantiate" the inline functions of the "generic.h" header. */
signed (maxs)(signed a, signed b) [[__unsequenced__]];
unsigned (maxu)(unsigned a, unsigned b) [[__unsequenced__]];
unsigned (maxus)(unsigned a, signed b) [[__unsequenced__]];
unsigned (maxsu)(signed a, unsigned b) [[__unsequenced__]];
signed long (maxsl)(signed long a, signed long b) [[__unsequenced__]];
unsigned long (maxul)(unsigned long a, unsigned long b) [[__unsequenced__]];
unsigned long (maxusl)(unsigned long a, signed long b) [[__unsequenced__]];
unsigned long (maxsul)(signed long a, unsigned long b) [[__unsequenced__]];
signed long long (maxsll)(signed long long a, signed long long b) [[__unsequenced__]];
unsigned long long (maxull)(unsigned long long a, unsigned long long b) [[__unsequenced__]];
unsigned long long (maxusll)(unsigned long long a, signed long long b) [[__unsequenced__]];
unsigned long long (maxsull)(signed long long a, unsigned long long b) [[__unsequenced__]];

int main(int argc, char* argv[argc+1]){
  if (argc > 1) {
    printf("testing strto functions: base 10 = %lu, general base = %llu\n",
           strtoul10(argv[2]), strtoull(argv[2], nullptr));
    long double a = strtold(argv[1]);
    printf("minimum of %Lg and 1.0f is %Lg\n",
           a, min(a, 1.0F));
    printf("minimum of %g and argc is %g\n",
           3.0, min(3.0, argc));
  }
  printf("maxof(1) = %d\n", maxof(1));
  printf("maxof(1.0) = %g\n", maxof(1.0));
  printf("maxof(long double) = %Lg\n", maxof(long double));
  printf("minof(1ul) = %lu\n", minof(1ul));
  printf("minof(1.0) = %g\n", minof(1.0));
  printf("minof(long double) = %Lg\n", minof(long double));
  printf("mix(1u, -1l) = %luul\n", mix(1u, -1l));
  printf("mix(-1, 1ul) = %luul\n", mix(-1, 1ul));
  printf("mix(-1, 1l) = %ldl\n", mix(-1, 1l));
  printf("mix(1u, 2ul) = %luul\n", mix(1u, 2ul));
  puts((memcpy)((char[6]){ 0 }, "hello", 6));
  // Erroneous target buffer, should not be qualified. diagnostic?
  puts((memcpy)((char volatile[6]){ 0 }, "he1lo", 6));
  // Erroneous use of target buffer, using 6 where there are only 5. diagnostic?
  puts((memcpy)((char[5]){ 0 }, "he2lo", 6));
  // Erroneous use of source buffer, using 7 where there are only 6. diagnostic?
  puts((memcpy)((char[7]){ 0 }, "he3lo", 7));
  puts(memcpy((char[6]){ 0 }, "hello", 6));
  // Erroneous target buffer, should not be qualified. diagnostic?
  puts(memcpy((char volatile[6]){ 0 }, "he1lo", 6));
  // Erroneous use of target buffer, using 6 where there are only 5. diagnostic?
  puts(memcpy((char[5]){ 0 }, "he2lo", 6));
  // Erroneous use of source buffer, using 7 where there are only 6. diagnostic?
  puts(memcpy((char[7]){ 0 }, "he3lo", 7));
  printf("snprintf(nullptr, 0, \"%%g\", 6.555): %d\n", snprintf(nullptr, 0, "%g", 6.555));
  printf("snprintf(nullptr, 0, \"hoho\"): %d\n", snprintf(nullptr, 0, "hoho"));
  // Erroneous use of nullptr when size is not zero. diagnostic?
  printf("snprintf(nullptr, 0, \"%%g\", 6.555): %d\n", snprintf(nullptr, 20, "%g", 6.555));
  printf("snprintf(nullptr, 0, \"hoho\"): %d\n", snprintf(nullptr, 20, "hoho"));
  // Erroneous use of buffer, claiming 20 only providing 16. diagnostic?
  printf("snprintf(something, 20, \"%%g\", 6.555): %d\n", snprintf((char[16]){ 0 }, 20, "%g", 6.555));
  // Erroneous use of buffer, claiming 20 only providing 16. diagnostic?
  printf("snprintf(something, 20, \"hoho\"): %d\n", snprintf((char[16]){ 0 }, 20, "hoho"));
  return EXIT_SUCCESS;
  SIZE_WIDTH;
}
