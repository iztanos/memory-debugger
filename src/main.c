#include <stdio.h>
#include <stdlib.h>
#include "memdebug.h"

int main(void)
{
    int *a = malloc(sizeof(int) * 4);
    char *b = malloc(20);
    double *c = malloc(sizeof(double) * 2);

    if (a == NULL || b == NULL || c == NULL) {
        fprintf(stderr, "malloc failed\n");
        free(a);
        free(b);
        free(c);
        return 1;
    }

    add_record(a, sizeof(int) * 4, "main.c", 8);
    add_record(b, 20, "main.c", 9);
    add_record(c, sizeof(double) * 2, "main.c", 10);

    print_records();

    struct allocation_record *found = find_record(b);

    if (found != NULL) {
        printf("found record for b: address=%p size=%zu\n", found->address, found->size);
    } else {
        printf("record for b not found\n");
    }

    free(a);
    free(b);
    free(c);

    free_record_list();

    return 0;
}