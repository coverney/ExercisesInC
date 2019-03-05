/*
  Generates 10000 random doubles. Used to pipe to CDF plotting function
  Resulting CDF can be seen in test3_output.png
*/
#include <stdio.h>
#include <stdlib.h>

#include "rand.h"

int main (int argc, char *argv[])
{
    int i;
    double x;

    srandom (time (NULL));

    for (i=0; i<10000; i++) {
        x = my_random_double();
        printf ("%lf\n", x);
    }

    return 0;
}
