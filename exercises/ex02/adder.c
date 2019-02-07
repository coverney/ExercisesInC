/*
 * Program to allows users to input integers and adds them up
 * Author: Cassandra Overney
 * Released under the ...
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int buffersize = 3;
static int arraysize = 5;

int read_input(int *array) {
  int index = 0;
  int always = 0;
  do {
    char temp_input[buffersize];
    char *line;
    puts("Enter an integer:");
    line = fgets(temp_input, buffersize, stdin);

    if (line == NULL) {
      return index;
    }

    // -2 considering fact that index starts at 0 and '\0' character
    if (temp_input[buffersize-2] != '\n' && getchar() != '\n') {
      fprintf(stderr, "Entered something that exceeds the length of %i!\n", buffersize-1);
      exit(EXIT_FAILURE);
    }

    if (index >= arraysize) {
      fprintf(stderr, "Entered more numbers than will fit in the array!\n");
      exit(EXIT_FAILURE);
    }

    array[index] = atoi(temp_input);
    index = index + 1;
  } while(always == 0);
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
  int intarray[arraysize];
  int index = read_input(intarray);
  //printf("Index is %i\n", index);
  int sum = compute_sum(intarray, index);
  printf("Sum is %i\n", sum);
  return 0;
}
