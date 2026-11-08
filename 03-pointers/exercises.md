# 03 – Pointers: exercises

Write each exercise in its own numbered file in this folder, then build and run it from the repository root:

```bash
make run/03-pointers/01-basics
```

Fix every compiler warning, not just the errors.

## 1. `01-basics.c` – what a pointer is
- Create `int x = 42` and `int *p = &x`. Print `x`, `&x`, `p`, `*p` and `&p`. Which ones are equal?
- Change `x` **through** the pointer (`*p = 100`) and print `x` again.
- Print `sizeof` of `char *`, `int *` and `double *`. Why are they all the same?
- Create a pointer set to `NULL`. Why is it better than a pointer with no value?

## 2. `02-swap.c` – changing variables from a function
- Write `swap_wrong(int a, int b)` and `swap(int *a, int *b)`. Call both and show that only one works. Why?
- Write `void min_max(const int arr[], size_t len, int *min, int *max)` that "returns" two results through pointers.
- Now you know why `scanf` needs `&`.

## 3. `03-arithmetic.c` – pointer arithmetic
- For a `char *`, an `int *` and a `double *`, print the address `p` and `p + 1`. How many bytes does each `+ 1` move?
- Show that `arr[i]` and `*(arr + i)` are the same thing.
- Go through an array with a pointer only (no index) and add up its elements.
- Subtract two pointers into the same array. What does the result mean?

## 4. `04-strings.c` – strings with pointers
- Write `my_strlen` using only a pointer (no index).
- Reverse a string **in place** with two pointers, one at each end.
- What is the difference between `char a[] = "hello";` and `char *b = "hello";`? Which one can you change? (Explain it, do not crash the program.)

## 5. `05-bytes.c` – looking at the raw bytes
- Use an `unsigned char *` to print, byte by byte in hex, an `int` with the value `0x12345678`. In what order do the bytes come out? Why?
- Do the same for an `int` with the value `-1` and for a `float` with the value `1.0f`.
- Write `void dump(const void *data, size_t size)` that prints the bytes of anything.

> 🔐 This is how a debugger or a hex editor sees memory – and the basis for the `hexdump` project (topic 06).

## 6. `06-function-pointers.c` – pointers to functions
- Write `add`, `sub`, `mul` and `divide`, and put them in an **array of function pointers**. Choose the operation by index.
- Sort an array with `qsort`, first ascending and then descending, by passing it different compare functions.

> 🔐 A function pointer is an address the program will **jump to**. Overwriting one with a buffer overflow makes the program run code chosen by the attacker.

## 7. `07-dangling.c` – pointers that point to nothing (bug on purpose)
- Write a function that returns the address of one of its **local** variables, and use it in `main`. What value do you get?
- Run it with `ASAN_OPTIONS=detect_stack_use_after_return=1`. What does AddressSanitizer report?
- Why is this pointer invalid? (Hint: topic 02, exercise 6 – what happens to the stack frame when a function returns?)
- Write the fix in `notes.md`.
