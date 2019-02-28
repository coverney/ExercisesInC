/* test_util uses MinUnit to test at least one function in util.c

 * Author: Cassandra Overney
 * Released under the TestLover Inc.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "util.h"
#include "minunit.h"

int tests_run = 0;

/*
  Runs first MinUnit test for the util icmpcode_v4 function. In this test,
  I am checking if the 0 code returns the right error message,
  'network unreachable'

  Returns a static char that is null if the test passed and the message if
  the test failed.
*/
static char *test1() {
    char *result = "network unreachable";
    char *res = icmpcode_v4(0);
    char *message = "test1 failed: icmpcode_v4(\"0\") should return network unreachable";
    mu_assert(message, strcmp(res,result)==0);
    return NULL;
}

/*
  Runs second MinUnit test for the util icmpcode_v4 function. In this test,
  I am checking if the 11 code returns the right error message,
  'network unreachable for TOS'

  Returns a static char that is null if the test passed and the message if
  the test failed.
*/
static char *test2() {
    char *result = "network unreachable for TOS";
    char *res = icmpcode_v4(11);
    char *message = "test2 failed: icmpcode_v4(\"0\") should return network unreachable for TOS";
    mu_assert(message, strcmp(res,result)==0);
    return NULL;
}

/*
  Runs test1 and test2
*/
static char * all_tests() {
    mu_run_test(test1);
    mu_run_test(test2);
    return NULL;
}

/*
  Runs all_tests and prints the error message or "ALL TESTS PASSED"
  It also prints the total number of tests ran.

  argc: number of command line arguments
  argv: array of strings consisting of command line arguments
  Return: 0 if no error and 1 if errored
*/
int main(int argc, char **argv) {
    char *result = all_tests();
    if (result != NULL) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
