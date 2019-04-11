/* Example code for Exercises in C.

Copyright 2016 Allen B. Downey
License: MIT License https://opensource.org/licenses/MIT

Further edited by Cassandra Overney

Results from first compilation:
  Creating child 0.
  Hello from the parent.
  Hello from child 0.
  Child 25023 exited with error code 0.
  Elapsed time = 0.000843 seconds.

Results after commenting out the exit statement after calling child_code
  Creating child 0.
  Hello from the parent.
  Hello from child 0.
  Hello from the parent.
  wait failed: No child processes
  ./fork: No child processes
  Child 25160 exited with error code 1.
  Elapsed time = 0.000789 seconds.

  wait is failing and maybe more children are being created?

Results for 1 child:
  Creating child 0.
  Hello from the parent.
  Hello from child 0.
  Child 25252 exited with error code 0.
  Elapsed time = 0.000745 seconds.

Results for 2 children:
  Creating child 0.
  Creating child 1.
  Hello from the parent.
  Hello from child 0.
  Child 25269 exited with error code 0.
  Hello from child 1.
  Child 25270 exited with error code 1.
  Elapsed time = 1.000808 seconds.

Results for 3 children:
  Creating child 0.
  Creating child 1.
  Creating child 2.
  Hello from child 0.
  Hello from the parent.
  Child 25281 exited with error code 0.
  Hello from child 1.
  Child 25282 exited with error code 1.
  Hello from child 2.
  Child 25283 exited with error code 2.
  Elapsed time = 2.001202 seconds.

Parent and child processes don't share same global segment:
  Creating child 0.
  Hello from the parent.
  Hello from child 0.
  Global var of child: 1
  Global var of parent: 0 (NOT 1!!)
  Child 3843 exited with error code 0.
  Elapsed time = 0.000320 seconds.

Parent and child processes don't share some stack segment:
  Creating child 0.
  Hello from the parent.
  Hello from child 0.
  Local var of child: 1
  Local var of parent: 0 (NOT 1!!)
  Child 6757 exited with error code 0.
  Elapsed time = 0.000343 seconds.

Parent and child processes don't share some heap segment:
  Creating child 0.
  Hello from the parent.
  Hello from child 0.
  Heap var of child: 1
  Heap var of parent: 0 (NOT 1!!)
  Child 7239 exited with error code 0.
  Elapsed time = 0.000357 seconds.

I am not sure how to test whether the child processes share the same code
  and static segments, because fork makes a copy of the address space, so
  they will have the same addresses. Since the code and static segments are
  constant, the parent and child processes tecnically share those segments.
  I can't use the same method as I did for the heap, stack, and global sections. 

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <wait.h>


// errno is an external global variable that contains
// error information
extern int errno;

// create global var to see if the parent and child shares a global segment
int var1 = 0;


// get_seconds returns the number of seconds since the
// beginning of the day, with microsecond precision
double get_seconds() {
    struct timeval tv[1];

    gettimeofday(tv, NULL);
    return tv->tv_sec + tv->tv_usec / 1e6;
}

void child_code(int i)
{
    sleep(i);
    printf("Hello from child %d.\n", i);
    var1 = 1;
    printf("%s", "Global var of child: ");
    printf("%i\n", var1);
}

// main takes two parameters: argc is the number of command-line
// arguments; argv is an array of strings containing the command
// line arguments
int main(int argc, char *argv[])
{
    int status;
    pid_t pid;
    double start, stop;
    int i, num_children;

    // the first command-line argument is the name of the executable.
    // if there is a second, it is the number of children to create.
    if (argc == 2) {
        num_children = atoi(argv[1]);
    } else {
        num_children = 1;
    }

    // create local variable
    int var2 = 0;

    // create a variable in the heap
    int *var3 = malloc(sizeof(int));
    *var3 = 0;

    // get the start time
    start = get_seconds();

    for (i=0; i<num_children; i++) {

        // create a child process
        printf("Creating child %d.\n", i);
        pid = fork();

        /* check for an error */
        if (pid == -1) {
            fprintf(stderr, "fork failed: %s\n", strerror(errno));
            perror(argv[0]);
            exit(1);
        }

        /* see if we're the parent or the child */
        if (pid == 0) {
            var2 = 1;
            *var3 = 1;
            child_code(i);

            printf("%s", "Local var of child: ");
            printf("%i\n", var2);

            printf("%s", "Heap var of child: ");
            printf("%i\n", *var3);

            exit(i);
        }
    }

    /* parent continues */
    printf("Hello from the parent.\n");

    for (i=0; i<num_children; i++) {
        pid = wait(&status);

        if (pid == -1) {
            fprintf(stderr, "wait failed: %s\n", strerror(errno));
            perror(argv[0]);
            exit(1);
        }

        // check the exit status of the child
        status = WEXITSTATUS(status);
        // print value of global var
        printf("%s", "Global var of parent: ");
        printf("%i\n", var1);

        // print value of local var
        printf("%s", "Local var of parent: ");
        printf("%i\n", var2);

        // print value of heap var
        printf("%s", "Heap var of parent: ");
        printf("%i\n", *var3);

        printf("Child %d exited with error code %d.\n", pid, status);
    }
    // compute the elapsed time
    stop = get_seconds();
    printf("Elapsed time = %f seconds.\n", stop - start);

    exit(0);
}
