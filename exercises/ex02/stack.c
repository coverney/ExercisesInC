/* Example code for Think OS.

Copyright 2014 Allen Downey
License: GNU GPLv3

1) According to stack.c, there are three functions that involve initializing
arrays and setting each elm in the array of size 5. In foo, each elm is set
to 42 and supposedly a pointer to the array is returned. In bar, array is set
to {0, 1, 2, 3, 4}, but nothing is returned. In main, foo is called, bar is
called, and each elm is printed. I would expect the array from foo to print out.

2) After compiling stack, I got the following error from foo:
function returns address of local variable. This error means that foo
is returning the address of a local variable that will be removed from the
stack diagram after foo's return statement.

3) After running it, I got the following error:
"Segmentation fault (core dumped)" However, the print statements did execute.
This error means that the program attempted to access an area of memory
that it was not allowed to access. After calling foo, when main tried to print
array it couldn't find the local array variable since it disappeared
from the stack. The stack diagram can be found in "diagrams.PDF".

4) After commenting out the print statements, I get the same errors. The only
difference is the fact that the print statements didn't happen.

Moral: don't return pointers to stack allocated data!

*/

#include <stdio.h>
#include <stdlib.h>

#define SIZE 5

int *foo() {
    int i;
    int array[SIZE];
    //printf("%p\n", array); //I get 0x7fff1ef3a3e0
    for (i=0; i<SIZE; i++) {
        array[i] = 42;
    }
    return array;
}

/*Working foo: make array static or dynamically allocate array
int *foo() {
    int i;
    static int array[SIZE];
    // printf("%p\n", array);
    for (i=0; i<SIZE; i++) {
        array[i] = 42;
    }
    return array;
}*/

void bar() {
    int i;
    int array[SIZE];
    //printf("%p\n", array); //I get 0x7fff1ef3a3e0
    for (i=0; i<SIZE; i++) {
        array[i] = i;
    }
}

/*Working bar: input pointer to array
void bar(int *array) {
    int i;
    // printf("%p\n", array);
    for (i=0; i<SIZE; i++) {
        array[i] = i;
    }
}*/

int main()
{
    int i;
    int *array = foo();

    /*Code snippet to use working bar()
    int array[SIZE];
    bar(array);*/

    bar();

    for (i=0; i<SIZE; i++) {
        printf("%d\n", array[i]);
    }

    return 0;
}
