#define MEMDEBUG_NO_REPLACE

#include <stdio.h>
#include <stdlib.h>
#include "memdebug.h"

/*
This is the head of the tracking list.
*/
static struct allocation_record *head = NULL;

/*
Create one allocation record node.
*/
struct allocation_record *create_record(void *address, size_t size, const char *file, int line)
{
    struct allocation_record* new_record = malloc(sizeof(struct allocation_record));

    if(new_record == NULL) {
        return NULL;
    }

    new_record -> address = address;
    new_record -> size = size;
    new_record -> line = line;
    new_record -> file = file;
    new_record -> freed = 0;
    new_record -> next = NULL;
    
    return new_record; 
}

/*
Add a new record to the front of the linked list.
*/
int add_record(void *address, size_t size, const char *file, int line)
{
    struct allocation_record *new_record = create_record(address, size, file, line);

    if(new_record == NULL) {
        fprintf(stderr, "Error: Failed to create new allocation memory\n");
        return 0;
    }
    new_record -> next = head;
    head = new_record;

    return 1;
}

/*
Search the linked list for a record matching the given address.
*/
struct allocation_record *find_record(void *address)
{
    struct allocation_record *current = head;

    while(current != NULL) {
        if(current -> address == address) {
            return current;
        }
        current = current -> next;
    }
    return NULL;
}

/*
Print every record in the list.
*/
void print_records(void)
{
    struct allocation_record *current = head;

    printf("---- allocation records ----\n");

    if (current == NULL) {
        printf("(no records)\n");
        return;
    }

    while (current != NULL) {
        printf("address=%p size=%zu freed=%d location=%s:%d\n",
               current->address,
               current->size,
               current->freed,
               current->file,
               current->line);

        current = current->next;
    }
}

/*
Print all allocations that have not been released yet.
*/
size_t print_leak_report(void)
{
    struct allocation_record *current = head;
    size_t leak_count = 0;
    size_t leaked_bytes = 0;

    printf("---- leak report ----\n");

    while (current != NULL) {
        if (!current->freed) {
            printf("LEAK: address=%p size=%zu location=%s:%d\n",
                   current->address,
                   current->size,
                   current->file,
                   current->line);
            leak_count++;
            leaked_bytes += current->size;
        }

        current = current->next;
    }

    if (leak_count == 0) {
        printf("No leaks detected.\n");
    } else {
        printf("%zu leak(s), %zu byte(s) total.\n", leak_count, leaked_bytes);
    }

    return leak_count;
}

/*
Free the debugger's internal linked list nodes.
*/
void free_record_list(void)
{
    struct allocation_record *current = head;

    while (current != NULL) {
        struct allocation_record *next_record = current->next;
        free(current);
        current = next_record;
    }

    head = NULL;
}
