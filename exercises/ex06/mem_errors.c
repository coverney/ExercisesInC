/* Example code for Exercises in C.

Copyright 2014 Allen Downey
License: Creative Commons Attribution-ShareAlike 3.0

Memory errors fixed by Cassandra Overney

*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


void free_anything(int *p) {
    free(p);
}

int read_element(int *array, int index) {
    int x = array[index];
    return x;
}

int main()
{
    //int never_allocated;
    int *free_twice = malloc(sizeof (int));
    int *use_after_free = malloc(sizeof (int));
    int *never_free = malloc(sizeof (int));
    int array1[100];
    int *array2 = malloc(100 * sizeof (int));

    // valgrind does not bounds-check static arrays FIXED
    read_element(array1, 0);
    read_element(array1, 99);

    // but it does bounds-check dynamic arrays FIXED
    read_element(array2, 0);
    read_element(array2, 99);

    // and it catches use after free
    *use_after_free = 17;
    free(use_after_free); // free after use!!

    // never_free is definitely lost
    *never_free = 17;
    free(never_free); // freed it!!

    free(array2);

    // the following line would generate a warning
    // I just commented it out because there is no use in freeing something
    // that isn't allocated
    // free(&never_allocated);

    // but this one doesn't
    // Also commented out since it doesn't make any sense
    //free_anything(&never_allocated);

    free(free_twice);
    // Commented out so it isn't freed twice
    // free(free_twice);

    return 0;
}
