/*  Implementations of several methods for generating random floating-point.

Copyright 2016 Allen B. Downey
License: MIT License https://opensource.org/licenses/MIT
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>


typedef union box{
  double d;
  int64_t i;
} Box;

// generate a random float using the algorithm described
// at http://allendowney.com/research/rand
float my_random_float()
{
    int x, exp, mant;
    float f;

    // this union is for assembling the float.
    union {
        float f;
        int i;
    } b;

    // generate 31 random bits (assuming that RAND_MAX is 2^31 - 1
    x = random();

    // use bit-scan-forward to find the first set bit and
    // compute the exponent
    asm ("bsfl %1, %0"
    :"=r"(exp)
    :"r"(x)
    );
    exp = 126 - exp;

    // use the other 23 bits for the mantissa (for small numbers
    // this means we are re-using some bits)
    mant = x >> 8;
    b.i = (exp << 23) | mant;

    return b.f;
}

// alternative implementation of my algorithm that doesn't use
// embedded assembly
float my_random_float2()
{
    int x;
    int mant;
    int exp = 126;
    int mask = 1;

    union {
        float f;
        int i;
    } b;

    // generate random bits until we see the first set bit
    while (1) {
        x = random();
        if (x == 0) {
            exp -= 31;
        } else {
            break;
        }
    }

    // find the location of the first set bit and compute the exponent
    while (x & mask) {
        mask <<= 1;
        exp--;
    }

    // use the remaining bit as the mantissa
    mant = x >> 8;
    b.i = (exp << 23) | mant;

    return b.f;
}

// compute a random double using my algorithm
double my_random_double()
{
  int64_t x;
  int64_t mant;
  //int exp = 126;
  // long int exp = 1022;
  int64_t exp = 1022;
  int64_t mask = 1;

  union {
      double d;
      int64_t i;
  } b;

  // generate random bits until we see the ffirst set bit
  while (1) {
      //x = random();
      x = (random() << 32) | random();
      if (x == 0) {
          exp -= 31;
      } else {
          break;
      }
  }

  // find the location of the first set bit and compute the exponent
  while (x & mask) {
      mask <<= 1;
      exp--;
  }

  // use the remaining bit as the mantissa
  mant = x >> 11; //mant should be 52
  b.i = (exp << 52) | mant;
  return b.d;
}

int get_bit(){
  int bit;
  static int bits = 0;
  static int x;

  if (bits == 0) {
    x = random();
    bits = 31;
  }

  // if (bits == 31){
  //   x = random();
  // }

  bit = x & 1;
  x = x >> 1;
  bits--;
  return bit;
}

// compute a random double using my algorithm
double my_random_double2()
{
  int x;
  int64_t mant, exp, high_exp, low_exp;
  Box low, high, ans;
  low.d = 0.0;
  high.d = 1.0;
  low_exp = (low.i >> 52) & 0x7FF;
  high_exp = (high.i >> 52) & 0x7FF;
  for(exp = high_exp-1; exp > low_exp; exp--){
    if (get_bit()) {
      break;
    }
  }

  mant = (random()<< 31) | random();

  //printf("%li\n", mant);

  mant = mant & 0xFFFFFFFFFFFFF; // find way to choose 52 bits
  //printf("%li\n", mant);
  if (mant == 0 && get_bit()) {
    exp++;
  }

  ans.i = (exp << 52) | mant;

  //printf("%li\n", ans.i);

  return ans.d;
}

double my_random_double3()
{
  int64_t mant, exp, mant_mask, one, sign;
  one = 1;
  Box ref, ans;
  ref.d = 0.75;

  sign = (ref.i >> 63) & 1;

  exp = (ref.i >> 52) & 0x7FF;
  //exp = exp ^ 1;

  mant_mask = (one << 52) - 1;
  mant = ref.i & mant_mask;

  printf("%d\n", sign );
  printf("%d\n", exp );
  printf("%0x%x\n", mant );

  ans.i = (exp << 52) | mant;

  //printf("%li\n", ans.i);

  return ans.d;
}

// return a constant (this is a dummy function for time trials)
float dummy()
{
    float f = 0.5;
    return f;
}

// generate a random integer and convert to float (dummy function)
float dummy2()
{
    int x;
    float f;

    x = random();
    f = (float) x;

    return f;
}

// generate a random float using the standard algorithm
float random_float()
{
    int x;
    float f;

    x = random();
    f = (float) x / (float) RAND_MAX;

    return f;
}


// generate a random double using the standard algorithm
float random_double()
{
    int x;
    double f;

    x = random();
    f = (double) x / (double) RAND_MAX;

    return f;
}
