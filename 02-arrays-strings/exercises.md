# 02 – Arrays and strings: exercises

Write each exercise in its own numbered file in this folder, then build and run it from the repository root:

```bash
make run/02-arrays-strings/01-arrays
```

Fix every compiler warning, not just the errors.

## 1. `01-arrays.c` – array basics
- Create an `int` array with 7 grades (0–20) and print them.
- Get the number of elements with `sizeof(array) / sizeof(array[0])` – never write the `7` by hand.
- Print the sum, the average (two decimal places), the minimum and the maximum.
- Print the **address** of each element with `%p`. How far apart are they? Why?

## 2. `02-reverse.c` – arrays and functions
- Write `void print_array(const int arr[], size_t len)` and `void reverse(int arr[], size_t len)`.
- `reverse` changes the array **in place** (swap the first with the last, and so on).
- Print the array before and after. Did `reverse` change the original array or a copy?
- Why do the functions need a `len` parameter? (Hint: what is `sizeof(arr)` inside the function?)

## 3. `03-my-string.c` – strings from scratch
- Print each character of `"Diogo"` with its ASCII code, **including** the `'\0'`.
- Write your own `my_strlen` (count until `'\0'`) and compare it with `strlen`.
- Write `my_strcpy(char dest[], const char src[], size_t dest_size)` that **never writes more than `dest_size` bytes** and always ends `dest` with `'\0'` (cut the string if it does not fit).

> 🔐 The standard `strcpy` has no size limit – it is behind countless buffer overflows.

## 4. `04-text.c` – working with a line of text
- Read a line with `fgets` (spaces included) and remove the `'\n'`.
- Count letters, vowels, digits and spaces (`<ctype.h>` helps: `isalpha`, `isdigit`, `isspace`, `tolower`).
- Print it in UPPERCASE and reversed.
- Say whether it is a **palindrome**, ignoring case, spaces and punctuation (`"Socorram-me, subi no onibus em Marrocos"` is one).

## 5. `05-matrix.c` – 2D arrays
- Create a 3×3 `int` matrix and print it as a grid.
- Print its **transpose** (rows become columns) and the sum of the main diagonal.
- Print all 9 values in the order they are stored in memory. What does that tell you about how C stores a 2D array?

## 6. `06-scope.c` – scope, lifetime and the stack
- Show a **global** variable, a **local** one and a **block** variable that hides (*shadows*) another with the same name.
- Write a function with a `static` local counter and call it 3 times. What is different from a normal local?
- Write a recursive function that prints the **address of a local variable** at depths 0 to 3. Are the addresses going up or down? What does that say about the stack?

## 7. `07-admin-overflow.c` – overwriting a neighbour (bug on purpose)
- A `struct` has `char name[8]` followed by `int is_admin = 0`. Read the name with `scanf("%s", ...)` and print whether the user is an admin.
- Become admin **without** being one, just by typing a long name. Exactly how many characters do you need?
- Why does AddressSanitizer **not** catch this one?
- Write the fix in `notes.md`.

> 🔐 Overwriting a variable next to a buffer is how many real exploits start – the stack (topic 07) works the same way.
