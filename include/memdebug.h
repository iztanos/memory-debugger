#ifndef MEMDEBUG_H
#define MEMDEBUG_H

#include <stddef.h>

struct allocation_record {
    void *address;
    size_t size;
    int freed;
    const char *file;
    int line;
    struct allocation_record *next;
};


struct allocation_record *create_record(void *address, size_t size, const char *file, int line);
void add_record(void *address, size_t size, const char *file, int line);
struct allocation_record *find_record(void *address);
void print_records(void);
void free_record_list(void);

void *my_malloc(size_t size, const char *file, int line);
void my_free(void *address, const char *file, int line);
size_t print_leak_report(void);

#ifndef MEMDEBUG_NO_REPLACE
#define malloc(size) my_malloc((size), __FILE__, __LINE__)
#define free(address) my_free((address), __FILE__, __LINE__)
#endif

#endif
