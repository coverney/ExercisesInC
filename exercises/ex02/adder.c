/*
 * Program to allows users to input integers, stores them in an array,
  adds them up once the user hits ^D, and print out the sum.
 * Author: Cassandra Overney
 * Released under the Simple Calculator Inc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Create global variables for buffer and array sizes*/
static int buffersize = 3;
static int arraysize = 5;

/* Reads user input, converts into integer and appends to array.

    If ^D inputted, then return. If user enters something that exceeds
    buffersize, then send an error message. If user enters more numbers than
    arraysize, then send an error message.

    Convert fron string to int using atoi and increment index variable.

    array: pointer to array of integers
    index: return size of array after ^D pressed

*/
int read_input(int *array) {
  int index = 0;
  int always = 1;
  /* Create forever loop since loop automatically ends with return
  statement if user input ^D*/
  while(always) {
    char temp_input[buffersize]; /* user inputted integer*/
    char *line; /* variable used to check for ^D*/
    puts("Enter an integer:");
    line = fgets(temp_input, buffersize, stdin);
    /* If user inputs ^D, then line is Null and return*/
    if (line == NULL) {
      return index;
    }

    /* If the user input exceeds buffersize, then print error message and exit.
    The -2 is considering fact that index starts at 0 and '\0' character*/
    if (temp_input[buffersize-2] != '\n' && getchar() != '\n') {
      fprintf(stderr, "Entered something that exceeds the length of %i!\n", buffersize-1);
      exit(EXIT_FAILURE);
    }

    /* If size of array exceeds the array size then print error message
    and exit*/
    if (index >= arraysize) {
      fprintf(stderr, "Entered more numbers than will fit in the array!\n");
      exit(EXIT_FAILURE);
    }

    /* Convert from string to integer and then increment index*/
    array[index] = atoi(temp_input);
    index = index + 1;
  }
  /* Technically never gets here, but here it is!*/
  return index;
}

/* Go through the integers in array and computes sum

    array: pointer to array of integers
    size: size of array
    sum: return sum
*/
int compute_sum(int *array, int size){
  int sum = 0;
  for (int i = 0; i < size; i++) {
    sum = sum + array[i];
  }
  return sum;
}

/* Create integer array, call read_input, call compute_sum, and then print
    sum.
*/
int main() {
  int intarray[arraysize];
  int index = read_input(intarray);
  int sum = compute_sum(intarray, index);
  printf("Sum is %i\n", sum);
  return 0;
}
