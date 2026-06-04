#include <stdio.h>
#include <stdlib.h>
#include "memdebug.h"

int main(void)
{
    int *a = malloc(sizeof(int) * 4);
    char *b = malloc(20);
    double *c = malloc(sizeof(double) * 2);

    if (a == NULL || b == NULL || c == NULL) {
        free(a);
        free(b);
        free(c);
        free_record_list();
        return 1;
    }

    print_records();

    free(a);
    free(b);

    printf("\nAfter freeing a and b:\n");
    print_records();

    printf("\n");
    print_leak_report();

    free(c);

    free_record_list();

    return 0;
}
