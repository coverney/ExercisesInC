/*  Implementations of several methods for generating random floating-point.

Copyright 2016 Allen B. Downey
License: MIT License https://opensource.org/licenses/MIT

my_random_double() is written by Cassandra Overney after much crying
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

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
  // Create 64 bit integers since double is 64 bit
  int64_t x;
  int64_t mant;
  int64_t exp = 1022; // binary: 01111111110
  int64_t mask = 1;

  union {
      double d;
      int64_t i;
  } b;

  // generate random bits until we see the ffirst set bit
  while (1) {
      // each random() gives a 32 bit result, so to generate 64 random bits
      // need to call random(), shift to the left by 32 bits and then call
      // random() again. The | is for setting the last 32 bits of x.
      x = (random() << 32) | random();
      if (x == 0) { //coin toss to move to the left of the fulcrum
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
  // mantissa is also known as the coefficient and is found by removing the
  // 11 bits of the exponent and taking whatever is left in x
  mant = x >> 11;
  // create the 64 bit integer of b by shifting the exponent to the left by
  // 52 bits and then combining it with the mantissa
  b.i = (exp << 52) | mant;
  return b.d;
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
double random_double()
{
    int x;
    double f;

    x = random();
    f = (double) x / (double) RAND_MAX;

    return f;
}
