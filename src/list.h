#ifndef LIST_H
#define LIST_H

// Definition of a list element
struct obj {
    struct obj *next;
    int data;
};

//declaration of obj *head
extern struct obj *head;

// Functions
void list_add(struct obj **list_head, struct obj *new_element);
void list_remove(struct obj *element);


#endif //LIST_H
