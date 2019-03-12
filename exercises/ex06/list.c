/* Example code for Exercises in C.

Based on an example from http://www.learn-c.org/en/Linked_lists

Copyright 2016 Allen Downey
License: Creative Commons Attribution-ShareAlike 3.0

Modified by Cassandra Overney

*/

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int val;
    struct node *next;
} Node;


/* Makes a new node structure.
*
* val: value to store in the node.
* next: pointer to the next node
*
* returns: pointer to a new node
*/
Node *make_node(int val, Node *next) {
    Node *node = malloc(sizeof(Node));
    node->val = val;
    node->next = next;
    return node;
}


/* Prints the values in a list.
*
* list: pointer to pointer to Node
*/
void print_list(Node **list) {
    Node *current = *list;

    printf("[ ");
    while (current != NULL) {
        printf("%d ", current->val);
        current = current->next;
    }
    printf("]\n");
}


/* Removes and returns the first element of a list.
*
* list: pointer to pointer to Node
*
* returns: int or -1 if the list is empty
*/
int pop(Node **list) {
    Node *current = *list;
    // return -1 if list is empty
    if (current==NULL) {
      return -1;
    }
    // get the value of the first node
    int val = current -> val;
    // set head to the node after the first one
    *list = current->next;
    // free the old first node
    free(current);
    return val;
}


/* Adds a new element to the beginning of the list.
*
* list: pointer to pointer to Node
* val: value to add
*/
void push(Node **list, int val) {
    // get the first node
    Node *current = *list;
    // make a new mode (setting first node to new node's next)
    Node * new_elm = make_node(val, current);
    //change heads value to point to new node
    *list = new_elm;
}


/* Removes the first element with the given value
*
* Frees the removed node.
*
* list: pointer to pointer to Node
* val: value to remove
*
* returns: value of node removed or -1 if it didn't work
*/
int remove_by_value(Node **list, int val) {
    // get the first node
    Node *current = *list;
    // return -1 if list is empty
    if (current==NULL) {
      return -1;
    }
    // get the next node (aka second node)
    Node *next_node = current->next;
    // iterate through list
    while (next_node != NULL) {
        if (next_node->val == val){
          // remove next and return value
          current->next = next_node->next;
          // make sure deleted node doesn't point to any other node
          next_node->next = NULL;
          // do I need a free node function?
          free(next_node);
          return val;
        }
        // update both current and next nodes
        current = current->next;
        next_node = next_node->next;
    }
    // didn't find node with val
    return -1;
}


/* Reverses the elements of the list.
*
* Does not allocate or free nodes.Node *current = *list;
*
* list: pointer to pointer to Node
*/
void reverse(Node **list) {
  // get the first node
  Node *head = *list;
  // exit if list if empty
  if (head==NULL) {
    return;
  }
  // keep track of the previous node (in nonreversed order)
  Node *past = head;
  // set current to the second node
  Node *current = head->next;
  // set head's next to NULL since it is going to be at the end
  head->next = NULL;
  // iterate through the rest of the nodes
  while (current != NULL) {
    // store current's next node
    Node *old_next = current->next;
    // set the new next node to be the previous one
    current->next = past;
    // update past to the current node
    past = current;
    // update current to the old next node, which will be connected to the updated past
    current = old_next;
  }
  // update head to past (originally pointing to the last node)
  *list = past;
}

void free_list(Node **list){
  int val = pop(list);
  while(val != -1){
    val = pop(list);
  }
}


int main() {
    Node *head = make_node(1, NULL);
    head->next = make_node(2, NULL);
    head->next->next = make_node(3, NULL);
    head->next->next->next = make_node(4, NULL);

    Node **list = &head;
    print_list(list);

    int retval = pop(list);
    print_list(list);

    push(list, retval+10);
    print_list(list);

    remove_by_value(list, 3);
    print_list(list);

    remove_by_value(list, 7);
    print_list(list);

    reverse(list);
    print_list(list);

    // Have to free rest of list
    free_list(list);
}
