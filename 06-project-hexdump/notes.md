# Notes

## What I learned

### What a hex dump is

A hex dump shows the **raw bytes** of a file, without interpreting them. Each line of `hexdump -C`:

```
00000000  23 69 6e 63 6c 75 64 65  20 3c 73 74 64 69 6f 2e  |#include <stdio.|
```

- **Offset** (`00000000`, `00000010`...): position in the file, in hex. Each line = 16 bytes = `0x10`.
- **16 bytes** in hex, two groups of 8.
- **The same bytes as text**: printable ASCII (`0x20`–`0x7e`) as itself, anything else as `.` (e.g. `0a` = newline).

| Command | Example | Notes |
|---|---|---|
| `hexdump` | `6923 636e` | 2 bytes at a time, **swapped** (read as little-endian 16-bit numbers) |
| `hexdump -C` | `23 69 6e 63` | 1 byte at a time, in file order – "canonical" format |
| `xxd` | `2369 6e63` | 2 bytes at a time, **not** swapped; `xxd -r -p` turns hex back into bytes |

**Magic bytes:** the first bytes identify the format. A macOS program starts with `cf fa ed fe` (Mach-O 64-bit).

### Parsing options with `getopt`

```c
#include <unistd.h>
int opt;
while ((opt = getopt(argc, argv, "s:n:v")) != -1)   // "s:" = -s takes a value
{
    switch (opt)
    {
        case 's': /* the value is in optarg */ break;
        case 'v': break;
        default:  /* unknown option: getopt already printed an error */ return -1;
    }
}
// argv[optind] is the first argument that is not an option (the file name)
```

- `strtoull(text, &end, 0)` – base **0** accepts decimal (`16`), hex (`0x10`) and octal (`020`).
- `strtoull` accepts `"-1"` and turns it into a huge number (unsigned wrap-around again!), so the `-` must be rejected by hand.

### Details that make the output identical

- `%08llx` – an `unsigned long long` in hex, padded with zeros to 8 digits (files can be bigger than 4 GB).
- A **short last line** is padded with 3 spaces per missing byte, so the `|` column stays aligned.
- **Repeated lines**: a full line identical to the previous one is replaced by `*`, only once per run (`memcmp` compares the 16 bytes). `-v` turns this off.
- The **final line** is the total offset – but nothing at all for an empty input or `-n 0`.
- `-s` past the end of the file: the final offset is the file size, not the requested offset. That is why `skip_bytes` returns how many bytes it **really** skipped.
- Skipping by **reading and throwing away** works on stdin too (`fseek` does not work on a pipe).

### Testing against a reference

The best test for a clone: compare it with the original.

```bash
diff <(hexdump -C FILE) <(./build/06-project-hexdump/hexdump FILE) && echo identical
```

- `<(command)` (process substitution) turns a command's output into something `diff` can read like a file.
- `diff` prints nothing when both are equal.
- Testing many edge cases (empty file, zeros, exact 16 bytes, past the end, stdin...) found the one case I had wrong: `-n 0` with `-s` must print nothing.

## Questions

## Useful commands

```bash
make run/06-project-hexdump/hexdump ARGS="01-basics/00-hello.c"
make run/06-project-hexdump/hexdump ARGS="-s 0x10 -n 32 01-basics/00-hello.c"
hexdump -C FILE | head        # the real one
xxd FILE | head
echo "48 65 6c 6c 6f" | xxd -r -p    # hex → bytes: Hello
man hexdump
```
