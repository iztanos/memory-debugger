# Memory Debugger

A small C memory debugging helper that wraps `malloc` and `free`, records allocation locations, and reports leaks or double frees.

## Features

- Tracks allocated address, size, source file, and line number.
- Marks records as freed instead of deleting them, which makes double-free checks possible.
- Reports active leaks with total leaked bytes.
- Provides `malloc` and `free` macros so application code can use normal allocation calls after including `memdebug.h`.

## Build

```sh
make
```

## Run the Demo

```sh
make run
```

The demo allocates three blocks, frees two, prints the tracked records, reports the remaining leak, then cleans everything up.

## Run the Test

```sh
make test
```

The test exercises a normal free, a double free warning, and one intentional leak.

## Use in Your Code

Include standard library headers before `memdebug.h`, then call `malloc` and `free` normally:

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

Files that implement the debugger itself should define `MEMDEBUG_NO_REPLACE` before including `memdebug.h` so the internal bookkeeping can use the real allocator.
