/**
 ** @file Test handling of different Unicode letters in identifiers.
 **/

#include "c23-fallback.h"

double Å = 1;                  // error, Ångström not normalized
double Å = 2;                  // ok, A with circle
double A\u030A = 3;            // error, combination not normalized
double Ω = 1;                  // error, Ohm not normalized
double Ω = 2;                  // ok, Greek capital omega
double K = 1;                  // error, Kelvin not normalized
double K = 2;
double ﬁ  = 1;                 // valid, ﬁ does not decompose into f and i
double fi = 2;                 // valid, different identifier
typedef double ℝ;              // valid
typedef unsigned long long ℕ;  // valid
#define ℜ(Z) real(Z)          // valid
#define ℑ(Z) imag(Z)           // valid
ℕ №;                           // error: Numero sign is not a letter
ℕ ℓ;                           // valid, ell sign is a letter
double ʃ(double a, double b, double φ(double)); // IPA is also acceptable
typedef double 각;             // valid, Hangul generally maps to itself
각 α = 1;
enum : bool { 응 = true, wɛ = true, };
char const 얜읐[] = "얜읐";
int a١ = 1;                    // valid, Arabic digits are considered different
int a1 = 2;
int a⁰ = 1;                    // non-portable, gcc and clang extension
int a𝟬 = 1;                    // valid, but not a good idea
int a0 = 2;
unsigned Ⅾ;                    // valid, roman digit 500, really a bad idea
