#include <stdio.h>
#include <float.h>

int main(int argc, [[maybe_unused]] char* argv[argc+1]) {
  // This 5²¹
  unsigned long long m10 = 476837158203125;
  _Decimal64 f10 = m10;
  // Because many of the 5's can be canceled by the 10⁻¹⁵, this is
  // exact
  _Decimal64 a10 = 2.097'152E-15DD;
  _Decimal64 p10 = a10*f10;
  double f2 = m10;
  // This is not an exact representation in binary floating types
  double a2u = 2.097'152E-15;
  double p2 = a2u*f2;
  printf("value of (double)5²¹ as binary\t\t\t\t%a\n", f2);
  printf("value of (double)5⁻²¹ as binary\t\t\t\t%a\n", a2u);
  printf("value of (double)5²¹⋅(double)5⁻²¹ as binary\t\t%a\n", p2);
  printf("value of 5²¹ as decimal integer\t\t\t\t%llu\n", m10);
  printf("value of (double)5²¹ as decimal\t\t\t\t%.26f\n", f2);
  printf("value of (_Decimal64)5²¹ as decimal\t\t\t476837158203125.00000000000000000000000000\n");
  printf("value of (double)5⁻²¹ as decimal\t\t\t%.40e\n", a2u);
  printf("value of (_Decimal64)5⁻²¹ as decimal\t\t\t2.0971520000000000000000000000000000000000e-15\n");
  printf("value of (double)5²¹⋅(double)5⁻²¹ as decimal\t\t%.40e\n", p2);
  printf("value of (_Decimal64)5²¹⋅(_Decimal64)5⁻²¹ as decimal\t%.40e\n", (double)p10);
  printf("equality of roundtrip (_Decimal64)5⁻²¹ through binary:\t\t\t\t\t%s\n", ((_Decimal64)(double)a10) == a10 ? "yes" : "no");
  printf("equality of roundtrip (double)5⁻²¹ through decimal:\t\t\t\t\t%s\n", ((double)(_Decimal64)a2u) == a2u ? "yes" : "no");
  printf("equality of roundtrip (_Decimal64)5²¹ through binary:\t\t\t\t\t%s\n", ((_Decimal64)(double)f10) == f10 ? "yes" : "no");
  printf("equality of roundtrip (double)5²¹ through decimal:\t\t\t\t\t%s\n", ((double)(_Decimal64)f2) == f2 ? "yes" : "no");
  printf("equality of roundtrip (_Decimal64)5²¹⋅(_Decimal64)5⁻²¹ through binary:\t\t\t%s\n", ((_Decimal64)(double)p10) == p10 ? "yes" : "no");
  printf("equality of roundtrip (double)5²¹⋅(double)5⁻²¹ through decimal:\t\t\t\t%s\n", ((double)(_Decimal64)p2) == p2 ? "yes" : "no");
  printf("equality (_Decimal64)5⁻²¹ and (double)5⁻²¹ in binary:\t\t\t\t\t%s\n", ((double)a10) == a2u ? "yes" : "no");
  printf("equality (_Decimal64)5⁻²¹ and (double)5⁻²¹ in decimal:\t\t\t\t\t%s\n", a10 == ((_Decimal64)a2u) ? "yes" : "no");
  printf("equality (_Decimal64)5²¹ and (double)5²¹ in binary:\t\t\t\t\t%s\n", ((double)f10) == f2 ? "yes" : "no");
  printf("equality (_Decimal64)5²¹ and (double)5²¹ in decimal:\t\t\t\t\t%s\n", f10 == ((_Decimal64)f2) ? "yes" : "no");
  printf("equality (_Decimal64)5²¹⋅(_Decimal64)5⁻²¹ and (double)5²¹⋅(double)5⁻²¹ in binary:\t%s\n", ((double)p10) == p2 ? "yes" : "no");
  printf("equality (_Decimal64)5²¹⋅(_Decimal64)5⁻²¹ and (double)5²¹⋅(double)5⁻²¹ in decimal:\t%s\n", p10 == ((_Decimal64)p2) ? "yes" : "no");
}
