# include "list.h"
# include <stddef.h>

// linked list functions add and remove

// linked list_add to add an element to the linked_list
void list_add(struct list_element **list_head, struct list_element *new_element) {
  new_element -> next = *list_head;	// point the new element to the current head
  *list_head = new_element; 		// update head to new element
}


// linked list_remoove an element from a list
void list_remove(struct list_element **list_head, struct list_element *element_to_remove) {
  struct list_element *current = *list_head;
  struct list_element *prev = NULL;

  // travers to find element to remove
  while (current != NULL && current != element_to_remove) {
    prev = current;
    current = current -> next;
  }

  if (current == element_to_remove) {
    // if you want to remove the head
    if (prev == NULL) {
      *list_head = current -> next;
    } else { 
      // bypass the element to remove
      prev -> next = current -> next;
    }

  }

}
