# Memory Debugger

A C memory debugging helper that wraps `malloc` and `free`, records allocation locations, and reports leaks or double frees.

This project is not a replacement for Valgrind. Its specific use case is lightweight, in-program memory checks that can be used in tests, including deterministic `malloc` failure simulation for error-handling paths.

## Features

- Tracks allocated address, size, source file, and line number.
- Marks records as freed instead of deleting them, which makes double-free checks possible.
- Reports active leaks with total leaked bytes.
- Simulates allocation failures after a chosen number of successful allocations.
- Provides `malloc` and `free` macros so application code can use normal allocation calls after including `memdebug.h`.

## Build

```sh
make
```

## Run the Demo

```sh
make run
```

The demo allocates three blocks, frees two, reports the remaining live allocation as a leak, then frees the final block and cleans up the debugger records.

## Run the Test

```sh
make test
```

The test exercises a normal free, a double free warning, and one intentional leak.
It also verifies allocation failure simulation by allowing one allocation to succeed and forcing the next allocation to fail.

## Use in Your Code

Include `memdebug.h`, then call `malloc` and `free` normally:

```c
#include <stdlib.h>
#include "memdebug.h"

int main(void)
{
    char *buffer = malloc(128);

    free(buffer);
    print_leak_report();
    free_record_list();
    return 0;
}
```

## Simulate Allocation Failure

Use `memdebug_fail_after` to test cleanup and error handling when allocation fails. This is the main case where this helper can be more convenient than Valgrind: your normal unit tests can force a specific allocation failure and assert the program handles it correctly.

```c
memdebug_fail_after(1);

char *first = malloc(16);   /* succeeds */
char *second = malloc(16);  /* returns NULL */

if (second == NULL) {
    /* verify your error path here */
}

free(first);
memdebug_reset_failures();
```

Files that implement the debugger itself should define `MEMDEBUG_NO_REPLACE` before including `memdebug.h` so the internal bookkeeping can use the real allocator.
