#include <stdio.h>
#include <stdlib.h>

#include "memdebug.h"

int main(void)
{
    char *first;
    char *second;
    char *ok = malloc(32);
    int *leaked = malloc(sizeof(int));

    if (ok == NULL || leaked == NULL) {
        free(ok);
        free(leaked);
        free_record_list();
        return 1;
    }

    free(ok);
    free(ok);

    if (print_leak_report() != 1) {
        free(leaked);
        free_record_list();
        return 1;
    }

    free(leaked);
    free_record_list();

    memdebug_fail_after(1);

    first = malloc(16);
    second = malloc(16);

    if (first == NULL || second != NULL || memdebug_failed_allocations() != 1) {
        free(first);
        memdebug_reset_failures();
        free_record_list();
        return 1;
    }

    free(first);
    memdebug_reset_failures();
    free_record_list();
    return 0;
}
