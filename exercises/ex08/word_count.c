/*

Reads a text file, splits lines into words, and counts the frequency of each word.

Run with:
export PKG_CONFIG_PATH=/usr/lib/x86_64-linux-gnu/pkgconfig
echo $PKG_CONFIG_PATH

gcc -Wall -o word_count `pkg-config --cflags glib-2.0` word_count.c `pkg-config --libs glib-2.0`
./word_count test1.txt

OR

make clean
make

Author: Cassandra Overney
License: Confused Book Lovers Inc.

*/

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

// Max char length of word
#define BUFFER_MAX_LENGTH 1024

/* Hash map iterator to print every key-value pair

  key: key
  value: value
  user_data: print format
*/
void iterator(gpointer key, gpointer value, gpointer user_data) {
 printf(user_data, key, GPOINTER_TO_INT(value));
}

/* If input does not exist in hash map, then add it
    If input exist then replace the key-value pair with an incremented value

  hash: hash map
  input: word
*/
void update_hash(GHashTable* hash, char* input){
  // first see if key exists in hash
  gpointer res = g_hash_table_lookup(hash, input);
  // if not, then insert key with value of 1
  if(res == NULL){
    g_hash_table_insert(hash, input, GINT_TO_POINTER(1));
  }
  // else, add 1 to value and replace key-value pair
  else{
    int old_val = GPOINTER_TO_INT(res);
    int new_val = old_val + 1;
    g_hash_table_replace(hash, input, GINT_TO_POINTER(new_val));
  }
}

/* Open txt file and call update_hash for every extracted word

  hash: hash map
  input: txt filename
*/
int parse_txt(GHashTable* hash, char* input){
  // declare some vars
  int c;
  unsigned int line_index = 0U;
  FILE *input_file;
  char word[BUFFER_MAX_LENGTH];
  unsigned int word_index = 0U;
  input_file = fopen(input, "r");

  if (input_file == 0) {
    //fopen returns 0, the NULL pointer, on failure
    fprintf(stderr, "Canot open input file\n");
    return EXIT_FAILURE;
  }
  else{
    int found_word = 0;
    // get a character from the file pointer
    while((c = fgetc(input_file)) != EOF){
      // avoid buffer overflow error
      if(line_index == BUFFER_MAX_LENGTH){
        fprintf(stderr, "error: line is too long. increase BUFFER_MAX_LENGTH.\n");
        return EXIT_FAILURE;
      }
      // check if c is line break
      else if(c == '\n'){
        // end word if word is being constructed
        if(found_word){
          word[word_index] = '\0';
          word_index = 0U;
          found_word = 0;
        }
        // reset line index
        line_index = 0U;
        continue;
      }

      // check if c is alpha character.
      // if it isn't, then end word and add to hash
      if(!isalpha(c)){
        if(found_word){
          word[word_index] = '\0';
          found_word = 0;
          word_index = 0U;
          update_hash(hash, strdup(word));
        }
      }
      // else still constructing word
      else{
        found_word = 1;
        c = tolower(c); //ignore case
        word[word_index++] = (char)c;
      }
      line_index++;
    }
  }
  return 0;
}

/* Pass in text file, which reads it in, splits lines into words, counts the
    the frequency and print out the results.

  argc: number of arguments
  argv: array of arguments
*/
int main(int argc, char** argv) {
    char *input = argv[1];
    // Create hash map
    GHashTable* hash = g_hash_table_new(g_str_hash, g_str_equal);
    // Parse txt file
    parse_txt(hash, input);
    // Print hash map
    g_hash_table_foreach(hash, (GHFunc)iterator, "%s: %d\n");
    return 0;
}
