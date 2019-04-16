/* Example code for Think OS.

Copyright 2014 Allen Downey
License: GNU GPLv3

Run with:
1) gcc counter_array.c -o counter_array -lpthread
2) ./counter_array

OR

1) make counter_array
2) ./counter_array

The idea is that each element of the array represents a unit of work.
We want to run multiple threads to do the work, but we have to make sure each
unit of work gets done exactly once. So we use a counter to keep track of the
next unallocated unit of work. Each thread should check the counter to see
which unit to work on, do the work, and then increment one element of the array
to indicate that the unit is done. If everything works correctly, each element
of the array should be incremented exactly once. If there are synchronization
errors, some elements might be 0, or greater than 1.

Results:
Checking...
138040 errors. (syncronization errors occur!!)

Time:
time ./counter_array
real	0m0.051s
user	0m0.087s
sys	0m0.004s


*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_CHILDREN 2

void perror_exit(char *s)
{
    perror(s);
    exit(-1);
}

void *check_malloc(int size)
{
    void *p = malloc(size);
    if (p == NULL) {
        perror_exit("malloc failed");
    }
    return p;
}

typedef struct {
    int counter;
    int end;
    int *array;
} Shared;

Shared *make_shared(int end)
{
    int i;
    Shared *shared = check_malloc(sizeof(Shared));

    shared->counter = 0;
    shared->end = end;

    shared->array = check_malloc(shared->end * sizeof(int));
    // make an array of size end and set every value to 0
    for (i=0; i<shared->end; i++) {
        shared->array[i] = 0;
    }
    return shared;
}

// just like the make_thread in counter.c
pthread_t make_thread(void *(*entry)(void *), Shared *shared)
{
    int ret;
    pthread_t thread;

    ret = pthread_create(&thread, NULL, entry, (void *) shared);
    if (ret != 0) {
        perror_exit("pthread_create failed");
    }
    return thread;
}

// just like the join_thread in counter.c
void join_thread(pthread_t thread)
{
    int ret = pthread_join(thread, NULL);
    if (ret == -1) {
        perror_exit("pthread_join failed");
    }
}

void child_code(Shared *shared)
{
    //printf("Starting child at counter %d\n", shared->counter);

    // loop until counter is greater than or equal to the size of array
    while (1) {
        if (shared->counter >= shared->end) {
            return;
        }
        // increment array value of counter by 1 (from 0 to 1)
        shared->array[shared->counter]++;
        // increment counter
        shared->counter++;
        // print if counter is multiple of 10000
        // if (shared->counter % 10000 == 0) {
        //     printf("%d\n", shared->counter);
        // }
    }
}

void *entry(void *arg)
{
    Shared *shared = (Shared *) arg;
    child_code(shared);
    //printf("Child done.\n");
    pthread_exit(NULL);
}

// Check for synchronization errors by counting how many elements of array are not 1
void check_array(Shared *shared)
{
    int i, errors=0;

    //printf("Checking...\n");

    for (i=0; i<shared->end; i++) {
        if (shared->array[i] != 1) errors++;
    }
    //printf("%d errors.\n", errors);
}

int main()
{
    int i;
    pthread_t child[NUM_CHILDREN];

    Shared *shared = make_shared(1000000);

    for (i=0; i<NUM_CHILDREN; i++) {
        child[i] = make_thread(entry, shared);
    }

    for (i=0; i<NUM_CHILDREN; i++) {
        join_thread(child[i]);
    }

    check_array(shared);
    return 0;
}
