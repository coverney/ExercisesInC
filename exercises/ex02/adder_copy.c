/*
 * Program to allows users to input integers and adds them up
 * Author: Cassandra Overney
 * Released under the ...
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int buffersize = 10;
static int arraysize = 5;

int read_input(int *array) {
  int index = 0;
  char *temp_input;
  char *line;
  do {
    puts("Enter an integer:");
    line = fgets(temp_input, buffersize, stdin);
    /*since I chose to use fgets, I do not need to output an error message
    if the user enters something that exceeds the buffer size.*/
    if (line == NULL) {
      return index;
    }
    // if (index >= ARRAYSIZE) {
    //   //fprintf(stderr, "Entered more numbers than will fit in the array!\n");
    //   puts("error");
    // }
    else{
      array[index] = atoi(temp_input);
      index = index + 1;
    }
  } while(line != NULL);
  return index;
}

int compute_sum(int *array, int size){
  int sum = 0;
  for (int i = 0; i < size; i++) {
    sum = sum + array[i];
  }
  return sum;
}

int main() {
  int intarray[5];
  int index = read_input(intarray);
  int sum = compute_sum(intarray, index);
  printf("Sum is %i\n", sum);
  return 0;
}
