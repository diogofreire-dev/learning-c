# Notes

## What I learned

### Arrays (exercise 1, `01-arrays.c`)

An array is a fixed number of elements of the **same type**, stored **one after another** in memory.

```c
int grades[] = {14, 9, 17};           // size taken from the list (3)
int zeros[10] = {0};                  // 10 ints, all 0
size_t count = sizeof(grades) / sizeof(grades[0]);   // number of elements
```

- Indexes go from `0` to `count - 1`. `grades[count]` is already **outside** the array.
- C does **not** check the index: reading or writing outside the array compiles fine and corrupts memory. AddressSanitizer catches it at runtime.
- `%p` prints an address (cast it to `(void *)`). The addresses of `int` elements are **4 bytes apart** (`sizeof(int)`):

```
grades[0] at 0x16bb2a620
grades[1] at 0x16bb2a624   (+4)
grades[2] at 0x16bb2a628   (+4)
```

### Arrays and functions (exercise 2, `02-reverse.c`)

- An array passed to a function is **not copied**: the function gets the **address of the first element**. Changes inside the function change the original array.
- Inside the function, `sizeof(arr)` is **8** (the size of an address), not the size of the array. That is why functions that take arrays also take a `len` parameter.
- `const int arr[]` = the function promises not to change the array.

```c
void reverse(int arr[], size_t len)   // swap arr[i] with arr[len - 1 - i] until the middle
```

### Strings (exercise 3, `03-my-string.c`)

A string is a `char` array ending in `'\0'`. Every `char` is just a small number (its ASCII code):

```
"Diogo"  →  'D'  'i'  'o'  'g'  'o'  '\0'
             68  105  111  103  111    0
```

| | Value for `"Diogo"` | What it measures |
|---|---|---|
| `strlen(name)` | 5 | characters until `'\0'` |
| `sizeof(name)` | 6 | bytes of the array (`'\0'` included) |

`strlen` is just a loop that counts until `'\0'` – I wrote my own `my_strlen`.

🔐 `strcpy(dest, src)` copies until the `'\0'` of `src`, **without knowing the size of `dest`**. My `my_strcpy(dest, src, dest_size)` stops at `dest_size - 1` characters and always writes the `'\0'`: a long string is cut, never overflowed. Safer standard options: `snprintf(dest, sizeof(dest), "%s", src)` or `strlcpy` (macOS/BSD).

### Text and `<ctype.h>` (exercise 4, `04-text.c`)

| Function | True when |
|---|---|
| `isalpha(c)` | letter |
| `isdigit(c)` | digit `0–9` |
| `isalnum(c)` | letter or digit |
| `isspace(c)` | space, tab, `\n`... |
| `toupper(c)` / `tolower(c)` | (convert the case) |

- Pass them an `unsigned char`: a plain `char` can be **negative** (accented letters like `é`), which is undefined behaviour for these functions.
- `size_t` can never be negative, so a loop going backwards must be `for (size_t i = len; i > 0; i--)` and use `i - 1`. With `i >= 0` the loop would never end (0 - 1 wraps to a huge number – unsigned overflow again!).
- **Palindrome:** two indexes, one at each end, walk towards the middle skipping anything that is not a letter or digit.

### 2D arrays (exercise 5, `05-matrix.c`)

```c
int m[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};   // m[row][col]
```

- Transpose: `t[row][col] = m[col][row]`. Main diagonal: `m[i][i]`.
- In memory it is just **9 ints in a row**: row 0, then row 1, then row 2 (**row-major order**). Read as a flat block: `1 2 3 4 5 6 7 8 9`.
- `#define N 3` creates a constant: the compiler replaces every `N` with `3`.

### Scope, lifetime and the stack (exercise 6, `06-scope.c`)

| Kind | Visible in | Lives |
|---|---|---|
| **global** (outside functions) | the whole file | the whole program |
| **local** (inside a function) | that function | while the function runs |
| **block** (inside `{ }`) | that block | while the block runs |
| **`static` local** | that function | the whole program – keeps its value between calls |

- A variable in an inner block with the same name **shadows** (hides) the outer one; the outer one is not changed.
- Inside one `printf`, the **order** in which C evaluates the arguments is **not guaranteed**: `printf("%d %d", f(), f())` could call them in any order.

**The stack:** every function call gets its own **stack frame** with its local variables. In my recursive test each deeper call was at a **lower** address:

```
depth 0: 0x16b6367e0
depth 1: 0x16b636700   (0xe0 = 224 bytes lower)
depth 2: 0x16b636620
depth 3: 0x16b636540
```

👉 **The stack grows towards lower addresses.** When a function returns, its frame is thrown away – that is why a local variable "disappears".

### Overwriting a neighbour (exercise 7, `07-admin-overflow.c`)

```c
struct user { char name[8]; int is_admin; };   // is_admin is right after name in memory
scanf("%s", u.name);                            // no limit
```

```
| n | a | m | e | . | . | . | . | is_admin        |
  0   1   2   3   4   5   6   7   8   9   10  11
```

| Name typed | Bytes written | `is_admin` | Result |
|---|---|---|---|
| `Diogo` (5) | 6 | `0x00000000` | normal user |
| `12345678` (8) | 9 | `0x00000000` | the `'\0'` lands on byte 8, which was already 0 |
| `AAAAAAAAB` (9) | 10 | `0x00000042` = 66 | **admin!** (`'B'` = 0x42) |
| 12+ characters | 13+ | — | AddressSanitizer: `stack-buffer-overflow` |

- **9 to 11 characters** turn me into admin. The 9th character lands on the **first byte** of `is_admin`, which shows the Mac is **little-endian** (the least significant byte is stored first).
- **Why ASan misses it:** ASan only notices writes **outside** a variable. Bytes 8–11 are still **inside** the struct `u`, so for ASan it is a valid write. Only at 12+ characters does the `'\0'` go past the end of `u`.
- **Fix:** limit the read to 7 characters + `'\0'`: `scanf("%7s", u.name)` (or `fgets(u.name, sizeof(u.name), stdin)`).

🔐 This is the idea behind many real exploits: a buffer overflow changes a **neighbouring variable** that controls security. On the stack, the neighbour can be the function's **return address** (topic 07).

## Questions

## Useful commands

```bash
make run/02-arrays-strings/01-arrays   # build and run
make all                               # build every exercise
```
