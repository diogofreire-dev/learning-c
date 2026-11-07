# Notes

## What I learned

### printf

```c
printf("Text that want to be shown\n"); // prints the text to the console; \n is a new line
```

| Format | Used for |
|---|---|
| `%d` | `int` |
| `%u` | `unsigned int` |
| `%zu` | `size_t` (the result of `sizeof`) |
| `%s` | string |

### sizeof – the size of a type (exercise 1, `01-sizes.c`)

`sizeof` measures the **type** (the size of the "box"), not the value inside it. A `char` is always 1 byte, whether it is empty or holds `'A'`. It is calculated by the compiler, before the program runs.

Results on my Mac (64-bit, arm64):

| Type | Bytes |
|---|---|
| `char` | 1 |
| `short` | 2 |
| `int`, `float` | 4 |
| `long`, `long long`, `double` | 8 |
| `int *` (pointer) | 8 |

A pointer stores a **memory address**. On a 64-bit machine addresses are 64 bits = 8 bytes, no matter what type it points to.

### Strings

C has no string type: a string is an array of `char` that ends with `'\0'` (the null character, value 0).

```
char nome[] = "Diogo";   →   | D | i | o | g | o | \0 |   = 6 bytes
```

- A string takes **number of letters + 1** bytes (the extra one is the `'\0'`).
- `printf("%s")` reads until it finds `'\0'`. Without it, it keeps reading whatever memory comes next.
- `sizeof(nome)` = space reserved (counts the `'\0'`); `strlen(nome)` = number of letters (from `<string.h>`).
- `char name[100]` fits at most **99 letters + `'\0'`** – that is why the fix uses `%99s`.

### Limits and integer overflow

`<limits.h>` defines the limits: `INT_MAX` = 2147483647, `INT_MIN` = -2147483648, `UINT_MAX` = 4294967295.

What I got when going past them:

```
INT_MAX + 1   →  -2147483648   (wrapped to the minimum)
INT_MIN - 1   →   2147483647   (wrapped to the maximum)
UINT_MAX + 1  →   0            (unsigned wraps to zero)
```

Like a car odometer going from 999999 to 000000. In binary (32 bits):

```
0111 1111 ... 1111   =  2147483647  (INT_MAX)
1000 0000 ... 0000   = -2147483648  (+1: the carry reaches the first bit)
```

In a signed `int` the **first bit is the sign** (0 = positive, 1 = negative). This is called **two's complement**.

- **Unsigned** overflow is defined behaviour: it always wraps to 0.
- **Signed** overflow is **undefined behaviour**: the compiler warns (`-Winteger-overflow`) and the sanitizer reports `runtime error: signed integer overflow`.

🔐 Security: if a program calculates `size + 1` to reserve memory and the sum wraps around, it may reserve far too little memory and then write past it.

### Warning vs error

- **Error:** the program is not built.
- **Warning:** the program is built, but something looks wrong. Fix every warning anyway – the compiler is usually right.

### scanf and input validation (exercise 2, `02-calc.c`)

```c
if (scanf("%d %c %d", &a, &op, &b) != 3)   // reads "7 + 2"
```

- `&a` tells `scanf` **where** to store the value (its address – a pointer, topic 03).
- Spaces in the format skip any spaces the user types.
- `scanf` **returns how many variables it filled**. `7 + 2` → 3, `abc` → 0. Without this check the program keeps going with variables that never got a value (garbage).

🔐 Rule number one: **never trust user input**. Always validate it.

### switch

```c
switch (op)
{
    case '+':
        printf("= %d\n", a + b);
        break;              // without break, execution "falls through" to the next case
    case '/':
    case '%':               // both cases share the same code (intentional fall-through)
        ...
        break;
    default:                // runs when no case matches
        ...
}
```

- `'+'` (single quotes) is a `char`; `"+"` (double quotes) would be a string.
- **Integer division by zero crashes the program** – check `b == 0` before `/` and `%`.
- `cond ? x : y` is a short `if`: it gives `x` if `cond` is true, `y` otherwise.

### Integer division and casts (exercise 3, `03-average.c`)

- `int / int` is an **integer division**: `7 / 2 = 3`, the decimal part is thrown away.
- Cast one side to `double` to get the real result: `(double) sum / count` → `3.50`.
- `%.2f` prints a `double` with 2 decimal places.
- `while (scanf("%d", &n) == 1 && n != 0)` stops on 0 and also on invalid input.

### Functions (exercise 4, `04-primes.c`)

```c
int is_prime(int n)       // return type, name, parameters
{
    ...
    return 1;             // "true" (C uses 0 for false, anything else for true)
}
```

- A function must be declared **before** it is used (that is why they are above `main`).
- `void` = the function returns nothing.
- Keep `main` short: it reads input and calls the functions.
- To check if `n` is prime, only test divisors up to √n. I wrote `d <= n / d` instead of `d * d <= n` because `d * d` can **overflow** for `n` close to `INT_MAX` (the bug from exercise 1, hiding in real code!).

### Bitwise operators and hex (exercise 5, `05-binary.c`)

| Operator | What it does | Example |
|---|---|---|
| `>>` | shift bits to the right | `0b1100 >> 2` = `0b0011` |
| `<<` | shift bits to the left | `0b0011 << 2` = `0b1100` |
| `&` | AND – 1 only if both bits are 1 | `x & 1` keeps only the last bit |
| `\|` | OR – 1 if any bit is 1 | |
| `^` | XOR – 1 if the bits are different | |

`(n >> i) & 1` = the value of bit `i` of `n` (move it to the right end, keep only it).

**Hexadecimal:** each hex digit = exactly 4 bits, so 32 bits = 8 hex digits.

```
255  →  0000 0000 0000 0000 0000 0000 1111 1111  →  0x000000ff
                                       f    f
```

`%x` prints in hex, `%08x` pads it with zeros to 8 digits.

### My first buffer overflow (exercise 6, `06-hello-name.c`)

```c
char name[100];
scanf("%s", name);   // no limit!
```

Typing 150 characters writes 150 + 1 bytes into a 100-byte array. The extra bytes overwrite whatever is next on the **stack**. AddressSanitizer catches it: `stack-buffer-overflow ... WRITE of size 151`.

Two fixes (see `06-hello-name-fixed.c`):

1. `scanf("%99s", name)` – reads at most 99 characters + `'\0'`. But `%s` stops at the first space: "Diogo Freire" → "Diogo".
2. `fgets(name, sizeof(name), stdin)` – reads the **whole line** (spaces included) and never writes more than `sizeof(name)` bytes. It keeps the `'\n'` from Enter, so remove it with `name[strcspn(name, "\n")] = '\0';`

`06-hello-name.c` stays buggy **on purpose** – I will exploit it in topic 07.

🔐 This is a **stack buffer overflow**, one of the most classic vulnerabilities. In topic 07 I will see how it can overwrite the function's return address.

## Questions

## Useful commands

Always build from the **repository root**, so the `Makefile` (with `-Wall -Wextra` and the sanitizers) is used:

```bash
make run/01-basics/01-sizes    # build and run 01-basics/01-sizes.c
make 01-basics/01-sizes        # only build it, into build/01-basics/01-sizes
./build/01-basics/01-sizes     # run it
make all                       # build every exercise
make clean                     # remove build/ (and any stray binaries)
```

Compiled programs go to `build/`, so the topic folders stay clean.

Running `make 01-sizes` inside `01-basics/` (or the VS Code run button) does **not** use our `Makefile`: it uses make's default rule (`cc 01-sizes.c -o 01-sizes`) – no warning flags, no sanitizers, and the binary lands next to the source. `make clean` removes those too.
