#include "list.h"
#include "serial.h"
#include "rprintf.h"
//#include <stdio.h>
#ifndef NULL
#define NULL ((void*)0)
#endif

// Global head of the list
struct obj *head = NULL;

// Define the list elements statically
struct obj elements[3];

void list_add(struct obj **list_head, struct obj *new_element) {
    if (new_element == NULL)
        return;

    new_element->next = *list_head;
    *list_head = new_element;
}

void list_remove(struct obj *element) {
    if (element == NULL)
        return;

    struct obj **current = &head;

    while (*current != NULL) {
	if (*current == element) {
	    *current = element->next;
	    return;
	}
    }
    current = &(*current)->next;
}

void print_list() {
    //printf("Linked list:\n");
    struct obj *current = head;
    while (current != NULL) {
        esp_printf(putc, "Data: 0x%lx, Address: %p, Next Address: %p\n",
               current->data, (void*)current, (void*)current->next);
        current = current->next;
    }
}

int main() {
    // Initialize the list elements with specific hexadecimal values
    elements[0].data = 0xDEAD;
    elements[0].next = NULL;
    elements[1].data = 0xBEEF;
    elements[1].next = NULL;
    elements[2].data = 0xCAFE;
    elements[2].next = NULL;

    // Add elements to the list
    list_add(&head, &elements[2]);  // Adding 0xCAFE
    list_add(&head, &elements[1]);  // Adding 0xBEEF
    list_add(&head, &elements[0]);  // Adding 0xDEAD

    // Print the linked list with debug information
    //print_list();

    // Remove the first element (which was added first)
    if (head != NULL) {
        struct obj *to_remove = head;
        head = head->next;
        list_remove(to_remove);
    }

    // Print the linked list after removal
    //printf("\nLinked list after removal:\n");
    //print_list();

    return 0;
}
