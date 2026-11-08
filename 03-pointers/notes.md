# Notes

## What I learned

### What a pointer is (exercise 1, `01-basics.c`)

A pointer is a variable that stores a **memory address**.

```c
int x = 42;
int *p = &x;    // p is "a pointer to int" and stores the address of x
```

| Expression | Meaning | Value in my run |
|---|---|---|
| `x` | the value | `42` |
| `&x` | the address of `x` | `0x16f94a720` |
| `p` | what `p` stores = the address of `x` | `0x16f94a720` |
| `*p` | go to that address and read = `x` | `42` |
| `&p` | `p` is a variable too, with its own address | `0x16f94a730` |

- `&` = "address of". `*` = "what is at this address" (**dereference**).
- `*p = 100;` writes into `x` through the pointer.
- Careful: in `int *p` the `*` means "p is a pointer"; in `*p` (in an expression) it means "dereference". Same symbol, two jobs.
- **Every pointer is 8 bytes** on a 64-bit machine (`char *`, `int *`, `double *`...): it is always just an address. The type only says how to read what is there.
- **`NULL`** = "points to nothing". An uninitialised pointer has a random address that may look valid; `NULL` can be checked (`if (p == NULL)`) and using it crashes immediately instead of silently corrupting memory.

### Changing variables from a function (exercise 2, `02-swap.c`)

C passes arguments **by value** – the function gets **copies**.

```c
void swap_wrong(int a, int b);   // swaps the copies – nothing changes outside
void swap(int *a, int *b);       // gets the addresses – *a and *b ARE the originals
swap(&x, &y);
```

- To let a function change a variable, pass its **address**.
- That is how a function "returns" several values: `min_max(arr, len, &min, &max)`.
- 💡 That is why `scanf("%d", &n)` needs the `&`: `scanf` must write into `n`.

### Pointer arithmetic (exercise 3, `03-arithmetic.c`)

`p + 1` moves to the **next element**, i.e. `sizeof(*p)` bytes:

```
char *:   0x...560 -> 0x...561   (+1 byte)
int *:    0x...570 -> 0x...574   (+4 bytes)
double *: 0x...590 -> 0x...598   (+8 bytes)
```

- `arr[i]` **is** `*(arr + i)`. Indexing is pointer arithmetic.
- An array name used as a value becomes the address of its first element: `int *p = arr;`
- Walking an array with a pointer: `for (const int *p = arr; p < arr + len; p++) sum += *p;`
- `last - first` = number of **elements** between them (`%td`), not bytes.
- 🔐 Nothing stops `p` from walking past the end of the array – that is a buffer overflow.

### Strings with pointers (exercise 4, `04-strings.c`)

```c
char a[] = "hello";          // an ARRAY on the stack with a copy of the text – can change it
const char *b = "hello";     // a POINTER to a string literal in read-only memory – cannot
```

| | `char a[] = "hello"` | `const char *b = "hello"` |
|---|---|---|
| What it is | an array of 6 bytes | an 8-byte pointer |
| `sizeof` | 6 | 8 |
| `a[0] = 'H'` | ✅ works | ❌ undefined behaviour (crash on macOS) |

- Always declare pointers to literals as `const char *` – then the compiler refuses to change them.
- `my_strlen` with a pointer: move `p` until `*p == '\0'`, the length is `p - s`.
- Reverse in place: one pointer at each end, swap, move both towards the middle.

### Looking at raw bytes (exercise 5, `05-bytes.c`)

An `unsigned char *` reads any object **one byte at a time**. `void *` = "pointer to anything".

```
int 0x12345678:  78 56 34 12    ← little-endian: least significant byte first
int -1:          ff ff ff ff    ← two's complement: all bits 1
float 1.0:       00 00 80 3f    ← IEEE 754 (0x3f800000), also little-endian
char[] "Hi!":    48 69 21 00    ← ASCII + '\0'
```

- **Little-endian** (Intel, Apple Silicon): the number `0x12345678` is stored "backwards" in memory. This is why in topic 02 the 9th character landed on the *low* byte of `is_admin`.
- The same 4 bytes mean completely different things depending on the **type** used to read them.

### Function pointers (exercise 6, `06-function-pointers.c`)

A function also lives at an address, and a pointer can store it:

```c
int (*op)(int, int) = add;                               // pointer to a function (int, int) -> int
int (*operations[])(int, int) = {add, sub, mul, divide}; // array of them
operations[2](12, 4);                                    // calls mul → 48
```

- `qsort(array, count, element_size, compare)` sorts anything: I pass it the function that decides the order (`ascending` or `descending`). A function passed to another function like this is called a **callback**.
- Compare functions receive `const void *` and must cast them to the real type.
- `return (x > y) - (x < y);` instead of `return x - y;` – the subtraction could overflow.

🔐 Calling a function pointer = **jumping to the address stored in it**. If a buffer overflow overwrites a function pointer, the program jumps wherever the attacker wants. The return address on the stack (topic 07) is the same idea.

### Dangling pointers (exercise 7, `07-dangling.c`)

```c
int *make_number(void)
{
    int local = 42;
    return &local;   // ❌ local dies when the function returns
}
```

My run:

```
First read:  42     ← still there, by pure luck
Second read: 999    ← another function reused the same stack slot
```

- When a function returns, its **stack frame** is thrown away (topic 02, exercise 6). The address is still in the pointer, but the memory now belongs to whoever is called next. This is a **dangling pointer**.
- The first read "worked" – which makes this bug dangerous: it can pass every test and fail later.
- AddressSanitizer only catches it with `ASAN_OPTIONS=detect_stack_use_after_return=1`: `stack-use-after-return`.
- Clang warns about `return &local;` directly (`-Wreturn-stack-address`), but not when the address goes through another variable first.

**Fixes:**
1. Return the **value**, not its address: `int make_number(void) { return 42; }`
2. Let the **caller** own the memory and pass its address: `void make_number(int *out) { *out = 42; }`
3. Allocate it on the **heap** with `malloc` – it lives until `free` (topic 04).

🔐 The heap version of this bug, **use-after-free**, is one of the most exploited vulnerabilities in browsers and operating systems.

## Questions

## Useful commands

```bash
make run/03-pointers/01-basics
ASAN_OPTIONS=detect_stack_use_after_return=1 ./build/03-pointers/07-dangling
```
