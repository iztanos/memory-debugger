#define MEMDEBUG_NO_REPLACE

#include <stdio.h>
#include <stdlib.h>
#include "memdebug.h"

static int fail_after_enabled = 0;
static size_t allocations_until_failure = 0;
static size_t failed_allocations = 0;

void *my_malloc(size_t size, const char *file, int line)
{
    void *ptr;

    if (fail_after_enabled) {
        if (allocations_until_failure == 0) {
            failed_allocations++;
            fprintf(stderr,
                    "memdebug warning: simulated malloc failure of %zu byte(s) at %s:%d\n",
                    size,
                    file,
                    line);
            return NULL;
        }

        allocations_until_failure--;
    }

    ptr = malloc(size);

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

void memdebug_fail_after(size_t successful_allocations)
{
    fail_after_enabled = 1;
    allocations_until_failure = successful_allocations;
    failed_allocations = 0;
}

void memdebug_reset_failures(void)
{
    fail_after_enabled = 0;
    allocations_until_failure = 0;
    failed_allocations = 0;
}

size_t memdebug_failed_allocations(void)
{
    return failed_allocations;
}
