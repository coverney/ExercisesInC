/*
 * Program to evaluate face values of user inputted card names.
 * Author: Cassandra Overney
 * Released under the Vegas Public License.
 * (c)2019 The Olin College Snackjack Team.
 */

#include <stdio.h>
#include <stdlib.h>

/* Prompts the user for input and puts the reply in the card_name string.

   User input is truncated to the first two characters (not including \0).

   prompt: string prompt to display
   card_name: string where result is stored
*/
void get_card_name(char *prompt, char *card_name){
  puts(prompt);
  fgets(card_name,3,stdin); /*fgets is updated version of scanf*/

  /*For inputs more than one character, might need to clear input buffer*/
  if(card_name[1] != '\n'){
    while ((getchar()) != '\n');
  }
}

/* Computes the value based on card_name and updates the count

    A switch statement is used to determine the value of the card based
    on the first character. If the card_name is a number, then atoi is used to
    convert from a string to a integer. The val is checked to make sure it is
    in possible range (1 to 10)

    card_name: string where user input is stored
    count: the current count from the card game

*/
void update_count(char *card_name, int *count){
  int val = 0;
  char first_char = card_name[0];
  switch(first_char) {
    case 'K':
    case 'Q':
    case 'J':
      val = 10;
      break;
    case 'A':
      val = 11;
      break;
    case 'X':
      return;
    default:
      val = atoi(card_name);
      if ((val < 1) || (val > 10)) {
        puts("I don't understand that value!");
        return;
      }
  }
  /* Calculate count from val*/
  if ((val > 2) && (val < 7)) {
    (*count)++;
  } else if (val == 10) {
    (*count)--;
  }
  printf("Current count: %i\n", *count);
}

/* Main function that gets the card_name and then uses it to update the count

    Initializes card_name and creates count and prompt variables, asks for user
    input by calling get_card_name, and updates the count variable with
    update_count

*/
int main() {
  char card_name[3];
  int count = 0;
  do {
    char *prompt = "Enter the card_name: ";
    get_card_name(prompt, card_name);
    update_count(card_name, &count);
  } while(card_name[0] != 'X');

  return 0;
}
