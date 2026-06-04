#include <stdio.h>
#include <stdlib.h>

#include "memdebug.h"

int main(void)
{
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
    return 0;
}
