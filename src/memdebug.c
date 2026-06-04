#define MEMDEBUG_NO_REPLACE

#include <stdio.h>
#include <stdlib.h>
#include "memdebug.h"

void *my_malloc(size_t size, const char *file, int line)
{
    void *ptr = malloc(size);

    if (ptr == NULL) {
        fprintf(stderr, "memdebug error: malloc failed at %s:%d\n", file, line);
        return NULL;
    }

    add_record(ptr, size, file, line);

    return ptr;
}

void my_free(void *address, const char *file, int line)
{
    struct allocation_record *record;

    if (address == NULL) {
        return;
    }

    record = find_record(address);

    if (record == NULL) {
        fprintf(stderr,
                "memdebug warning: attempt to free untracked pointer %p at %s:%d\n",
                address,
                file,
                line);
        return;
    }

    if (record->freed) {
        fprintf(stderr,
                "memdebug warning: double free of pointer %p at %s:%d; allocated at %s:%d\n",
                address,
                file,
                line,
                record->file,
                record->line);
        return;
    }

    record->freed = 1;
    free(address);
}
