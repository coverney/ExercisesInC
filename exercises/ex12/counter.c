/* Example code for Think OS.

Copyright 2014 Allen Downey
License: GNU GPLv3

Further edited by Cassandra Overney

Results:
Address of parent: 0x7ffce50ee184
counter = 0
Address of child: 0x7feb7fd69f14
counter = 1
Address of child: 0x7feb7f568f14
counter = 2
Address of child: 0x7feb7ed67f14
counter = 3
Address of child: 0x7feb7e566f14
counter = 4
Address of child: 0x7feb7dd65f14
Final value of counter is 5


*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_CHILDREN 5

/* Print an error message and exit.
*/
void perror_exit(char *s)
{
    perror(s);
    exit(-1);
}

/* Call malloc and exit if it fails.
*/
void *check_malloc(int size)
{
    void *p = malloc(size);
    if (p == NULL) {
        perror_exit("malloc failed");
    }
    return p;
}

/* Structure that contains variables shared between threads.
*/
typedef struct {
    int counter;
} Shared;

/* Allocate the shared structure.
*/
Shared *make_shared()
{
    Shared *shared = check_malloc(sizeof(Shared));
    shared->counter = 0;
    return shared;
}

/* Create a child thread.
*/
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

/* Wait for a child thread.
*/
void join_thread(pthread_t thread)
{
    int ret = pthread_join(thread, NULL);
    if (ret == -1) {
        perror_exit("pthread_join failed");
    }
}

/* Code run by the child threads.
*/
void child_code(Shared *shared)
{
    printf("counter = %d\n", shared->counter);
    int i;
    printf("Address of child: %p\n", &i);
    shared->counter++;
}

/* Entry point for the child threads.
*/
void *entry(void *arg)
{
    Shared *shared = (Shared *) arg;
    child_code(shared);
    pthread_exit(NULL);
}

int main()
{
    int i;
    printf("Address of parent: %p\n", &i);
    pthread_t child[NUM_CHILDREN];

    Shared *shared = make_shared();

    for (i=0; i<NUM_CHILDREN; i++) {
        child[i] = make_thread(entry, shared);
    }

    for (i=0; i<NUM_CHILDREN; i++) {
        join_thread(child[i]);
    }

    printf("Final value of counter is %d\n", shared->counter);
    return 0;
}
