# Notes

## What I learned

### Structs (exercise 1, `01-structs.c`)

```c
typedef struct student
{
    char name[32];
    int age;
    double grades[3];
} Student;                       // typedef: call it Student instead of struct student

Student s = {.name = "Diogo", .age = 20, .grades = {16, 14, 18}};
s.age = 21;                      // . on a struct
Student *p = &s;
p->age = 22;                     // -> on a pointer to a struct (same as (*p).age)
```

- A struct is **copied** when passed to a function (like an `int`). Pass a pointer to avoid the copy, and `const Student *` if the function only reads it.
- Structs can be copied with `=` (arrays cannot).

**Padding:** the compiler adds empty bytes so that each member is **aligned** (an `int` at an address that is a multiple of 4 – the CPU reads it faster).

```
struct { char a; int b; char c; }    → 12 bytes
  a at 0, [3 bytes padding], b at 4, c at 8, [3 bytes padding]

struct { int b; char a; char c; }    → 8 bytes
  b at 0, a at 4, c at 5, [2 bytes padding]
```

- Same members, different order → different size. Put the biggest members first.
- `offsetof(type, member)` (from `<stddef.h>`) gives the position of a member in bytes.
- 🔐 Padding bytes are not initialised: copying a struct to a file or the network can **leak** old memory hidden in them.

### Linked lists (exercise 2, `02-linked-list.c`)

```
head -> [3 | *] -> [1 | *] -> [4 | NULL]
```

```c
typedef struct node
{
    int value;
    struct node *next;   // the typedef name does not exist yet inside the struct
} Node;
```

- Every node is `malloc`'d; the list ends at `NULL`; an empty list is `Node *list = NULL;`.
- Functions that may change the **first** node take `Node **head` – the address of the caller's pointer (like `swap` in topic 03).
- `free_list`: save `next` **before** `free(current)`, otherwise reading `current->next` is a use-after-free.
- `make leaks/05-structs-files/02-linked-list` → `0 leaks`.

| | Array | Linked list |
|---|---|---|
| Access element `i` | instant (`arr[i]`) | walk `i` nodes |
| Insert/remove at the start | move every element | change 1 pointer |
| Memory | one block, no extra | one `malloc` + one pointer per node |
| Grow | `realloc` (may copy everything) | just add a node |

### Command-line arguments (exercise 3, `03-args.c`)

```c
int main(int argc, char *argv[])
```

```
./03-args 10 20 30
argc = 4
argv[0] = "./build/05-structs-files/03-args"   ← the program itself
argv[1] = "10"  argv[2] = "20"  argv[3] = "30"  (argv[4] = NULL)
```

- Every argument is a **string**, even numbers.
- `make run/... ARGS="10 20 30"` passes arguments through our `Makefile`.

**`atoi` vs `strtol`:** `atoi("abc")` returns 0 and `atoi("12abc")` returns 12 – no way to know the input was wrong. `strtol` tells you:

```c
char *end;
errno = 0;
long value = strtol(text, &end, 10);
if (end == text || *end != '\0')   → not a number / junk after it
if (errno == ERANGE)               → too big
```

- Errors go to **`stderr`**: `fprintf(stderr, ...)`. Normal output goes to `stdout`.
- `return 1` from `main` = the program failed (the shell sees it in `$?`).

### Reading a file (exercise 4, `04-wc.c`)

```c
FILE *file = fopen(path, "r");       // NULL if it fails
if (file == NULL) { perror(path); return 1; }
int c;
while ((c = fgetc(file)) != EOF) { ... }
fclose(file);
```

- `perror(text)` prints `text: reason` (e.g. `nope.txt: No such file or directory`).
- `fgetc` returns an **`int`**, not a `char`: it has to fit every byte (0–255) **and** `EOF` (-1).
- `EOF` also comes back on a read error; `ferror(file)` tells them apart.
- My `wc` gave exactly the same as the real one: `61 203 1391`.

| Mode | Meaning |
|---|---|
| `"r"` | read (the file must exist) |
| `"w"` | write – **erases** the file or creates it |
| `"a"` | append – write at the end, creates it if needed |
| `"rb"` / `"wb"` | the same in binary mode |

### Copying a file (exercise 5, `05-copy.c`)

```c
while ((n = fread(buffer, 1, sizeof(buffer), source)) > 0)
    fwrite(buffer, 1, n, dest);      // n, not sizeof(buffer): the last block is shorter
```

- `fread`/`fwrite` work with blocks of bytes, any file (text or a compiled program). Checked with `cmp`: identical copies.
- **Close every file on every path**, also when something fails in the middle.
- `fclose` can fail too: it writes what is still in the buffer (e.g. disk full).

### Contacts app (exercise 6, `06-contacts.c`)

Puts everything together: a `struct`, a dynamic array (topic 04), `argv` commands and a text file (`name,phone` per line).

```bash
make run/05-structs-files/06-contacts ARGS="build/contacts.csv add Diogo 912345678"
make run/05-structs-files/06-contacts ARGS="build/contacts.csv list"
```

- `strcmp(a, b) == 0` → the strings are equal (`a == b` would compare the **addresses**).
- `strchr(s, ',')` finds a character; `strpbrk(s, ",\n")` finds any of several.
- `snprintf(dest, size, "%s", src)` = safe string copy (never more than `size` bytes).
- Input is validated **twice**: the arguments, and the file when loading (it may have been edited by hand). A `,` in a name would break the file format.

### Format string bug (exercise 7, `07-format-string.c`)

```c
printf(name);        // ❌ the user's text is the FORMAT
printf("%s", name);  // ✅ the user's text is just DATA
```

The compiler warns: `format string is not a string literal (potentially insecure) [-Wformat-security]`.

Typing `%p %p %p %p %p %p %p %p`:

```
Hello, 0x41b58ab3 0x100090e84 0x100090858 0x0 0x1337 0x203 0x7025207025207025 0x2520702520702520
                                               ^^^^^^                ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                                               secret!               "%p %p %p" – my own input, in hex
(the secret 0x1337 lives at 0x16d1226c0)
```

- Each `%p` makes `printf` read one more "argument" that was never passed – it takes whatever is next in the **registers and on the stack**. The 5th value is my `secret`.
- `0x7025207025207025` = the bytes `%p %p %p` (`0x25` = `%`, `0x70` = `p`, `0x20` = space) – the input itself, stored on the stack. Attackers use this to place addresses there.
- `%n` **writes** to memory (the number of characters printed so far, at an address taken from the stack). On macOS the program is **aborted** (exit code 134, SIGABRT): Apple's libc refuses `%n` in a format that is not a constant. Other systems are not always so careful.

🔐 With a format string bug an attacker can **read** the stack (passwords, keys, addresses that break ASLR) and, with `%n`, **write** to memory – for example a return address.

**Fix:** never let input be the format: `printf("%s", name);` or `fputs(name, stdout);`.

## Questions

## Useful commands

```bash
make run/05-structs-files/03-args ARGS="10 20 30"
make run/05-structs-files/04-wc ARGS="05-structs-files/04-wc.c"
make leaks/05-structs-files/02-linked-list
cmp file1 file2                       # compare two files byte by byte (no output = identical)
echo $?                               # exit code of the last command
```
