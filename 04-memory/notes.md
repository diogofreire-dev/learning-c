# Notes

## What I learned

### malloc, calloc and free (exercise 1, `01-malloc.c`)

```c
int *numbers = malloc(n * sizeof(*numbers));   // n ints on the heap, NOT initialised
int *zeros   = calloc(n, sizeof(*zeros));      // n ints on the heap, all set to 0
if (numbers == NULL) { /* out of memory */ }
...
free(numbers);
numbers = NULL;
```

- `malloc(bytes)` reserves memory on the **heap** and returns its address (or `NULL` if it fails – **always check**).
- `sizeof(*numbers)` instead of `sizeof(int)`: still right if the type changes later.
- `malloc` memory contains **garbage**; `calloc` fills it with zeros and also checks `count * size` for overflow.
- **Every** `malloc`/`calloc`/`realloc` needs exactly **one** `free`. `free(NULL)` is allowed and does nothing.
- After `free`, set the pointer to `NULL` so it cannot be used by mistake.

**Why not just a local array?**
1. The **stack** is small (8 MB by default on macOS); a huge local array crashes the program. The heap can use gigabytes.
2. A local array dies when the function returns (topic 03, exercise 7). Heap memory lives **until `free`**.
3. The size can be decided while the program runs.

### Where things live in memory (exercise 2, `02-layout.c`)

Without sanitizers (`build/plain/`), from the lowest to the highest address:

```
0x102cb05a0  function main        ┐ text   – the code (read-only, executable)
0x102cb0818  string literal       ┘ rodata – constants (read-only)
0x102cb8000  global initialised     data   – globals with a value
0x102cb8004  static local           data   – static locals live with the globals
0x102cb8008  global zero            bss    – globals without a value (start at 0)
0x1033ada60  heap_a                 heap   – malloc ↑ grows UP
0x1033ada70  heap_b                 (allocated after heap_a → higher address)
   ...  big gap  ...
0x16d14e7b0  local_b                stack  – locals ↓ grows DOWN
0x16d14e7b4  local_a
```

- The **heap grows up**, the **stack grows down** (towards each other).
- With AddressSanitizer the heap looks different (`0x6020...`), because ASan uses its own allocator with extra space around each block to catch overflows.

### realloc and a growing array (exercise 3, `03-dynamic-array.c`)

```c
int *bigger = realloc(numbers, new_capacity * sizeof(*numbers));
if (bigger == NULL) { free(numbers); return 1; }   // the old block is still valid
numbers = bigger;
```

- `realloc` may **move** the block to a new address (it copies the data for you): in my run it moved every time it grew.
- If it fails it returns `NULL` and the **old block is untouched**. `numbers = realloc(numbers, ...)` would overwrite the only pointer to it → data lost and a leak. Always use a temporary pointer.
- **Doubling** the capacity (2 → 4 → 8 → 16) means few `realloc` calls. `count` = elements used, `capacity` = room available.

### Functions that return memory (exercise 4, `04-heap-strings.c`)

```c
char *my_strdup(const char *s);            // malloc(strlen(s) + 1), then copy
char *concat(const char *a, const char *b);
```

- Heap memory **survives** the function that allocated it – the right way to fix the dangling pointer from topic 03.
- **Ownership:** whoever receives the pointer must `free` it. Write it in a comment above the function, because C cannot enforce it.
- Always `strlen + 1` for the `'\0'`. `memcpy(dest, src, n)` copies exactly `n` bytes.

### Memory leaks (exercise 5, `05-leak.c`)

A **leak** = memory that is still allocated but that **nothing points to anymore**, so it can never be freed.

```c
for (...) {
    message = malloc(32);   // the previous block loses its only pointer
}                           // missing free(message)
```

`make leaks/04-memory/05-leak` (builds **without** ASan and runs Apple's `leaks` tool):

```
Process 12647: 5 leaks for 240 total leaked bytes.
STACK OF 5 INSTANCES OF 'ROOT LEAK: <malloc in main>':
    main + 52  05-leak.c:16
```

- It finds all 5 and the **exact line** of the `malloc` (thanks to `MallocStackLogging=1`). Each 32-byte request used 48 bytes: the allocator rounds sizes up.
- ASan's leak detector (`detect_leaks`) **does not work on macOS**, and `leaks` does not work on an ASan build – that is why the `Makefile` makes a separate plain build.
- The program does not crash: when it ends, the operating system takes all its memory back. But a **server running for months** keeps losing memory until it runs out.

🔐 Leaks can be a denial-of-service: an attacker repeats the request that leaks until the server dies.

### The three classic heap bugs (exercise 6, `06-heap-bugs.c`)

| Bug | What happens | ASan says |
|---|---|---|
| **heap buffer overflow** | writes past the end of a `malloc`'d block | `heap-buffer-overflow ... WRITE of size 4` |
| **use-after-free** | uses a block after `free` | `heap-use-after-free ... READ of size 4` |
| **double free** | `free` twice on the same pointer | `attempting double-free` |

ASan shows the file and line of the bug. Without ASan these may **not crash at all** – they silently corrupt the allocator's data and the program fails much later, somewhere else.

### Why use-after-free is dangerous (exercise 7, `07-uaf-admin.c`)

```
User created at 0x603000001b70, is_admin = 0
Message stored at 0x603000001b70           ← SAME address: the freed block was reused
is_admin through the old pointer = 1094795585 (0x41414141)   ← 'AAAA'
Welcome, administrator!
```

1. The program frees `user` but keeps the pointer.
2. The next `malloc` of the **same size** gets the **same block** – and the attacker fills it with `'A'`.
3. The program reads `user->is_admin` through the old pointer and finds the attacker's bytes.

- With normal ASan the bug is caught (`heap-use-after-free`): ASan keeps freed blocks in a **quarantine** so they are not reused right away. With `ASAN_OPTIONS=quarantine_size_mb=0` (or in a normal program) the block is reused and the attack works.
- **Fix:** `free(user); user = NULL;` and never use a pointer after freeing it. Reading through `NULL` crashes immediately instead of reading someone else's data.

🔐 Real use-after-free exploits do exactly this, but with a **function pointer** in the freed object (topic 03, exercise 6): the attacker's data replaces it and the program jumps to their code.

## Questions

## Useful commands

```bash
make run/04-memory/01-malloc
make leaks/04-memory/05-leak                                   # find leaks (macOS leaks tool)
make build/plain/04-memory/02-layout                           # build without sanitizers
ASAN_OPTIONS=quarantine_size_mb=0 ./build/04-memory/07-uaf-admin
```
