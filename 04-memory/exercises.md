# 04 – Memory: exercises

Write each exercise in its own numbered file in this folder, then build and run it from the repository root:

```bash
make run/04-memory/01-malloc
```

Fix every compiler warning, not just the errors.

## 1. `01-malloc.c` – your first heap allocation
- Ask the user how many numbers they want, `malloc` an `int` array of that size, fill it and print it.
- Always check whether `malloc` returned `NULL`, and `free` the memory at the end.
- Use `calloc` for a second array. What is the difference from `malloc`?
- Why can't you just write `int numbers[n];` and forget about `malloc`? (Think about what happens if `n` is huge, and about returning the array from a function.)

## 2. `02-layout.c` – where things live in memory
- Print the address of: a function, a string literal, a global variable, a `static` local, two heap blocks and two local variables.
- Sort them from the lowest to the highest address. Which areas of memory can you see?
- Does the heap grow up or down? And the stack?

## 3. `03-dynamic-array.c` – an array that grows
- Read integers until `0` and store **all** of them, without knowing in advance how many will come.
- Start with room for 2 and **double** the capacity with `realloc` whenever it is full. Print the capacity every time it grows.
- Why must the result of `realloc` go into a **temporary** pointer first?

## 4. `04-heap-strings.c` – functions that return memory
- Write `char *my_strdup(const char *s)` that returns a copy of `s` on the heap.
- Write `char *concat(const char *a, const char *b)` that returns a new string with `a` followed by `b`.
- Who must call `free` – the function or the caller? Write it as a comment above each function.

## 5. `05-leak.c` – a memory leak (bug on purpose)
- In a loop, allocate memory for 5 strings and "forget" to free them.
- Run it with `make leaks/04-memory/05-leak`. How many leaks does it find, and how many bytes? Does it tell you **where** the memory was allocated?
- Why doesn't the program crash? Why is a leak still a problem (think of a server that runs for months)?

## 6. `06-heap-bugs.c` – the three classic heap bugs (on purpose)
- Write a menu that, depending on the choice, causes one of these bugs:
  1. **heap buffer overflow** – write one element past the end of a `malloc`'d array
  2. **use-after-free** – read memory after `free`
  3. **double free** – call `free` twice on the same pointer
- Run each option and read what AddressSanitizer reports.

## 7. `07-uaf-admin.c` – why use-after-free is dangerous (bug on purpose)
- `malloc` a `struct user` with a `name` and `is_admin = 0`, then `free` it but keep the pointer.
- `malloc` a new block of the **same size** and fill it with data you control (for example all `'A'`).
- Read `is_admin` through the **old** pointer. What value does it have? Did the new block reuse the old memory?
- Run it normally, and then with `ASAN_OPTIONS=quarantine_size_mb=0`. Why does the result change?

> 🔐 This is how real use-after-free exploits work: the attacker gets their own data placed where a freed object used to be, and the program keeps trusting the old pointer.
