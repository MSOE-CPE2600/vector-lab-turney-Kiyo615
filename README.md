# calc - tiny vector REPL (Matlab style)

`calc` is a small command line REPL for working with named 3D vectors. You can assign vectors, list them, perform basic arithmetic, and load or save a workspace from CSV.

## How it works
- Keeps a workspace of named 3D vectors in dynamic memory.
- Uses realloc on every vector addition to allocate appropriate memory.
- Frees memory on exit.
- Parses expressions like `a = 1,2,3`, `c = a + b`, or `2.5 * a`.
- Prints vectors as `name = x, y, z`.
- Supports loading and saving the workspace to a CSV file.

---

## Build
makefile

### Simple build
```bash

gcc -Wall -Wextra -o calc.c
