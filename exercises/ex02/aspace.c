/* Example code for Think OS.

Copyright 2014 Allen Downey
License: GNU GPLv3

PDF of virtual address space sketch is included in the ex02 folder. It is
titled "diagrams.PDF".

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int var1;

/* Prints address of a local int variable to test if stack grows down
  With this function, I discovered that the stack grows down towards
  smaller memory addresses. The address of var2 in decimal form was
  140721107744428, while the address of var3 in decimal form was
  140721107744388.
*/
void printLocalAddress(){
  int var3 = 7;
  printf("Address of local variable is %p\n", &var3);
}

int main ()
{
    int var2 = 5;
    void *p = malloc(128);
    /* With second call to malloc, I discovered that heap grows up towards
    larger memory addresses. The address of p in decimal form was
    17834000, while the address of c in decimal form was 17834144.
    */
    void *c = malloc(128);
    char *s = "Hello, World";

    printf ("Address of main is %p\n", main);
    printf ("Address of var1 is %p\n", &var1);
    printf ("Address of var2 is %p\n", &var2);
    printf ("p points to %p\n", p);
    printf ("c points to %p\n", c);
    printf ("s points to %p\n", s);
    printLocalAddress();

    /* Even after generating several different random numbers from 1 to 32,
    the space between the allocated memory m1 and m2 point to 32 in decimal
    form. That means there are consistently 4 bytes between the two
    memory addresses.
    */
    srand ( time(NULL) ); // Set random seed
    int var4 = (rand() % 32) + 1;
    printf("Random number is %i\n", var4);
    void *m1 = malloc(var4);
    void *m2 = malloc(var4);
    printf ("m1 points to %p\n", m1);
    printf ("m2 points to %p\n", m2);

    return 0;
}
