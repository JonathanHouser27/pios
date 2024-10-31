#include "page.h"
#include <stddef.h>
#define NUM_PAGES 128

struct ppage physical_page_array[NUM_PAGES]; // 128 pages, each 2MB in length
struct ppage *free_pages = NULL; // Head of the free list

// Function to initialize the list of free physical page structures
void init_pfa_list(void) {
    for (int i = 0; i < NUM_PAGES; i++) {
        physical_page_array[i].physical_addr = (void *)(unsigned long)(0x1000 * i); // Assign a physical address
        physical_page_array[i].next = (i < NUM_PAGES - 1) ? &physical_page_array[i + 1] : NULL; // Link to next
        physical_page_array[i].prev = (i > 0) ? &physical_page_array[i - 1] : NULL; // Link to previous
        list_add((struct obj**)&free_pages, (struct obj*)&physical_page_array[i]); // Add to the free list using list_add
    }
}

// Function to allocate physical pages
struct ppage *allocate_physical_pages(unsigned int npages) {
    if (npages == 0 || free_pages == NULL) {
        return NULL; // No pages to allocate or free list is empty
    }

    struct ppage *allocd_list = NULL; // New list for allocated pages

    // Loop to unlink the requested number of pages from the free list
    for (unsigned int i = 0; i < npages; i++) {
        if (free_pages == NULL) {
            break; // Not enough pages available
        }

        struct ppage *current_page = free_pages;
        free_pages = free_pages->next; // Move the head of the free list

        // Unlink from the free list
        current_page->next = allocd_list; // Link to the new allocated list
        if (allocd_list != NULL) {
            allocd_list->prev = current_page; // Link back to the previous allocated list
        }
        allocd_list = current_page; // Update the head of the allocated list
        current_page->prev = NULL; // Set previous of allocated page to NULL
    }

    return allocd_list; // Return the allocated list
}

// Function to free physical pages
void free_physical_pages(struct ppage *ppage_list) {
    if (ppage_list == NULL) {
        return; // Nothing to free
    }

    struct ppage *current_page = ppage_list;

    // Loop through the allocated list and add each page back to the free list
    while (current_page != NULL) {
        struct ppage *next_page = current_page->next; // Save the next page

        // Link back to the free list using list_add
        list_add((struct obj**)&free_physical_pages, (struct obj*)current_page);

        current_page = next_page; // Move to the next allocated page
    }
}
