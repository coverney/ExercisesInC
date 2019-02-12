/* Example code for Exercises in C.

Given keyword, print out possible tracks.

Modified version of an example from Chapter 2.5 of Head First C.
Further modified by Cassandra Overney

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define NUM_TRACKS 5

char tracks[][80] = {
    "So What",
    "Freddie Freeloader",
    "Blue in Green",
    "All Blues",
    "Flamenco Sketches"
};


// Finds all tracks that contain the given string.
//
// Prints track number and title.
void find_track(char search_for[])
{
    int i;
    for (i=0; i<NUM_TRACKS; i++) {
        if (strstr(tracks[i], search_for)) {
            printf("Track %i: '%s'\n", i, tracks[i]);
        }
    }
}

/* Finds all tracks that match the given pattern.

    Prints track number and title.

    pattern: user input, supposedly in regex form
*/
void find_track_regex(char pattern[])
{
    regex_t pattern_regex;
    int result;
    int i;
    /* Variable for possible error messages*/
    char msgbuf[100];

    /* Compile regular expression*/
    result = regcomp(&pattern_regex, pattern, REG_EXTENDED);
    if (result) {
    /* If result isn't 0, then compilation failed*/
      fprintf(stderr, "Could not compile regex.\n");
      exit(EXIT_FAILURE);
    }

    /* Actually compare regular expression with each track name*/
    for (i=0; i<NUM_TRACKS; i++) {
        /* Compared regex with each track*/
        result = regexec(&pattern_regex, tracks[i], 0, NULL, 0);
        /* If return is 0, then found a match and print track*/
        if (!result) {
          printf("Track %i: '%s'\n", i, tracks[i]);
        }
        /* If return is REG_NOMATCH, then just continue to the next track*/
        else if (result == REG_NOMATCH) {
          continue;
        }
        /* Else there is an error and print it*/
        else{
          regerror(result, &pattern_regex, msgbuf, sizeof(msgbuf));
          fprintf(stderr, "Regex match failed: %s\n", msgbuf);
          exit(EXIT_FAILURE);
        }
    }
    /* Frees any memory allocated by regcomp() */
    regfree(&pattern_regex);
}

// Truncates the string at the first newline, if there is one.
void rstrip(char s[])
{
    char *ptr = strchr(s, '\n');
    if (ptr) {
        *ptr = '\0';
    }
}

/* Test find_track_regex
    With input [A-F], I got the following output:
      Track 1: 'Freddie Freeloader'
      Track 2: 'Blue in Green'
      Track 3: 'All Blues'
      Track 4: 'Flamenco Sketches'
*/
int main (int argc, char *argv[])
{
    char search_for[80];

    /* take input from the user and search */
    printf("Search for: ");
    fgets(search_for, 80, stdin);
    rstrip(search_for);

    //find_track(search_for);
    find_track_regex(search_for);

    return 0;
}
