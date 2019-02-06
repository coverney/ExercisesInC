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

int card_value(char *card_name){
  int val;
  switch (card_name[0]) {
    case 'K': case 'Q': case 'J':
      return 10;
    case 'A':
      return 11;
    default:
      val = atoi(card_name);
      if ((val >= 1) && (val <= 10)) {
        return val;
      }
  }
  return -1;
}

void update_count(int *val, int *count) {
  if ((*val > 2) && (*val < 7)) {
    *count += 1;
  }
  else if (*val == 10) {
    (*count)--;
  }
}


/* Main function that gets the card_name and then uses it to update the count

    Initializes card_name and creates count and prompt variables, asks for user
    input by calling get_card_name, and updates the count variable with
    update_count

*/
int main() {
  char card_name[3];
  int count = 0;
  while(card_name[0] != 'X') {
    puts("Enter the card_name: ");
    scanf("%2s\n", card_name);
    //fgets(card_name,3,stdin);
    int val = card_value(card_name);
    update_count(&val, &count);
    printf("Current count: %i\n", count);
  }

  return 0;
}
