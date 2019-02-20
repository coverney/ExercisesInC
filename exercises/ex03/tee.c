/*
 * Small tool that emulates the UNIX tee command which reads from
   standard input and writes to standard output and other inputted files
 *
 * Author: Cassandra Overney
 * Released under the EasyTee Inc.
 */

#include <stdio.h>
#include <unistd.h>

/* Executes the tee command

    Reads in the command line options and arguments
    Creates an array of File pointers
    Reads in the standard input line by line
    Writes each line to standard output and files
    Close open files

  argc: number of command line arguments
  argv: array of strings consisting of command line arguments
  Return: 0 if no error and 1 if errored

*/
int main(int argc, char *argv[]){
  int append = 0; // Boolean that is true if -a option used
  FILE *outputs[20]; // Array of file pointers
  // Read in command line options
  char ch;
  while ((ch = getopt(argc, argv, "a")) != EOF)
    switch(ch){
      // If a tag is present, then set append to true
      case 'a':
        append = 1;
        break;
      // If option detected that isn't accounted for then throw error
      default:
        fprintf(stderr, "Unkown option: '%s'\n", optarg);
        return 1;
    }
  // Update argc and argv to skip past options and get to other arguments
  argc -= optind;
  argv += optind;
  // Read in other arguments which are files to write
  for (int i = 0; i < argc; i++) {
    // If append is true then open file to append
    if (append) {
      outputs[i] = fopen(argv[i], "a");
    }
    // Else open file to write (overwrite)
    else{
      outputs[i] = fopen(argv[i], "w");
    }
  }

  // Read in standard input line by line
  //int line_num = 0;
  char line[150];
  while (1) {
    if (fgets(line,150, stdin) == NULL) break;
    // line_num++;
    // printf("%3d: %s", line_num, line);

    // Write to Standard Output
    fprintf(stdout, "%s", line);
    // Write to other open files
    for (int i = 0; i < argc; i++) {
      fprintf(outputs[i], "%s", line);
    }
  }
  // Close open files
  for (int i = 0; i < argc; i++) {
    fclose(outputs[i]);
  }
  return 0;
}
