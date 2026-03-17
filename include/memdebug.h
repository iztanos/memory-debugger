#ifndef MEMDEBUG_H
#define MEMDEBUG_H

#include <stddef.h>

typedef struct allocation_record {
    void *address;
    size_t size;
    int freed;
    const char *file;
    int line;
    struct allocation_record *next;
} allocation_record;


allocation_record *create_record(void *address, size_t size, const char *file, int line);
void add_record(void *address, size_t size, const char *file, int line);
allocation_record *find_record(void *address);
void print_records(void);
void free_record_list(void);

#endif