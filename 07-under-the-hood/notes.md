# Notes

This topic is an **introduction** to assembly (ARM64, the CPU of this Mac). It is not "all of assembly": the next step is to study it properly, especially **x86-64** (used in most CTFs, books and malware), in a Linux environment.

## What I learned

### From C to assembly (exercise 1, `01-asm-basics.c`)

```bash
make asm/07-under-the-hood/01-asm-basics OPT=-O0   # no optimisation
make asm/07-under-the-hood/01-asm-basics           # -O1
```

**`add` with `-O0`** – every variable goes through memory (the stack):

```asm
_add:
    sub  sp, sp, #16        ; make room on the stack for the locals
    str  w0, [sp, #12]      ; store a (it arrived in w0)
    str  w1, [sp, #8]       ; store b (it arrived in w1)
    ldr  w8, [sp, #12]      ; load a again
    ldr  w9, [sp, #8]       ; load b again
    add  w0, w8, w9         ; result in w0 = the return value
    add  sp, sp, #16        ; free the stack space
    ret                     ; jump back to the address in x30
```

**`add` with `-O1`** – the same in one instruction: `add w0, w1, w0` + `ret`.

- `w0`/`w1` = the 32-bit halves of `x0`/`x1` (an `int` is 32 bits).
- Arguments arrive in `w0`, `w1`, ...; the **return value** goes in `w0`.
- `-O0` is easy to follow line by line (that is why debuggers like it); `-O1` and higher keep values in registers.

**`max`** – the `if` became **no jump at all**:

```asm
cmp   w0, w1             ; compare a and b
csel  w0, w0, w1, gt     ; w0 = (a > b) ? a : b   ("conditional select")
```

**`sum_to`** – with `-O1` the compiler **removed the loop** and used the formula for `1 + 2 + ... + n` (`umull`, `lsr`, ...). The compiler is free to change *how* the code works, as long as the result is the same – that is why reading optimised assembly is harder, and why undefined behaviour (signed overflow, topic 01) can lead to surprising results.

### Calling convention (exercise 2, `02-calling.c`)

```asm
mov  x9, sp
mov  w8, #9
str  w8, [x9]          ; 9th argument → on the stack
mov  w8, #10
str  w8, [x9, #4]      ; 10th argument → on the stack
mov  w0, #1            ; 1st to 8th → registers w0–w7
...
mov  w7, #8
bl   _sum10            ; call: x30 = the return address, then jump
```

- ARM64 has **8 argument registers** (`x0`–`x7`); any more go on the **stack**.
- `bl` (branch with link) = call: saves the return address in `x30` (`lr`) and jumps. `ret` jumps back to `x30`.
- At the start of `main`: `stp x29, x30, [sp, #32]` saves the frame pointer and the return address on the stack, because `bl _sum10` is about to overwrite `x30`.
- (x86-64 Linux passes 6 arguments in registers: `rdi, rsi, rdx, rcx, r8, r9`, and `call` pushes the return address on the stack.)

### The stack frame (exercise 3, `03-stack-frame.c`)

```
main       starts at 0x104c2ca94
show_frame starts at 0x104c2c858

depth 0: local at 0x16b1d2760   fp = 0x16b1d2830   returns to 0x104c2caec  ← inside main
depth 1: local at 0x16b1d2660   fp = 0x16b1d2730   returns to 0x104c2c9fc  ← inside show_frame
depth 2: local at 0x16b1d2560   fp = 0x16b1d2630   returns to 0x104c2c9fc
depth 3: local at 0x16b1d2460   fp = 0x16b1d2530   returns to 0x104c2c9fc
```

- Each frame is `0x100` (256) bytes **lower** than the previous one – the stack grows down.
- **Return address** = the instruction right after the `bl` in the caller. Depth 0 returns into `main`; the deeper calls return into `show_frame` itself (the recursive call), always to the **same** address.
- The return address is **stored on the stack**, next to the local variables. That is what makes stack buffer overflows dangerous: a write past a local buffer can reach it – and `ret` would then jump to a wrong address. This is exactly what the protections below exist to stop.

```
higher addresses
┌───────────────────────────┐
│ caller's frame            │
├───────────────────────────┤ ← fp (x29)
│ saved fp | return address │
│ canary                    │ ← checked before ret
│ local buffer[16]          │ ← an overflow writes upwards, towards the canary
└───────────────────────────┘ ← sp
lower addresses
```

### Debugging with lldb (exercise 4, `04-debug-me.c`)

```
(lldb) b average
(lldb) run
(lldb) v
(int) sum = -82820864      ← not initialised yet: garbage from the stack
(lldb) n
(lldb) n
(lldb) n
(lldb) v
(int) sum = 12
(int) i = 1                ← the first time in the loop, i is already 1!
```

`sum = 12` is `grades[1]`: the loop skipped `grades[0]` (18). **Bug:** `for (int i = 1; ...)` must start at `0`. With the fix: `(18+12+15+9+16) / 5 = 14.00`.

- `b` breakpoint, `r` run, `n` next line, `s` step into, `v` local variables, `p` print, `bt` backtrace, `register read`, `di` disassemble, `si` one instruction.
- Before the line `int sum = 0;` runs, the variable already exists with **garbage** – what was left on the stack (topic 04: uninitialised memory).

### Protections against stack overflows (exercise 5, `05-mitigations.c`)

| Build | Mode | Long name | What stopped it |
|---|---|---|---|
| ASan (`make run/...`) | `strcpy`, `loop` | `stack-buffer-overflow` (exit 134) | **AddressSanitizer** |
| plain (`make plain/...`) | `strcpy` | nothing printed (exit 133, `SIGTRAP`) | **FORTIFY_SOURCE** – before the copy |
| plain | `loop` | prints the long name, then exit 134 (`SIGABRT`) | **stack canary** – before `ret` |
| both | `safe` | `Hello, AAAAAAAAAAAAAAA` (cut to 15) | nothing to stop – no bug |

- **FORTIFY_SOURCE:** the compiler knows `buffer` has 16 bytes and turns `strcpy` into `__strcpy_chk`, which checks the size **before** copying. It only works when the compiler can see the size.
- **Stack canary:** a secret random value placed between the locals and the saved return address. Just before `ret`, the function checks it (`___stack_chk_guard` / `___stack_chk_fail` in the assembly). A hand-written loop is not checked by FORTIFY, but an overflow that reaches the return address must pass through the canary first – so the program is killed **before** `ret` uses a corrupted address. `Back in main` is never printed.
- "Canary" comes from the canaries miners took into coal mines: if the bird died, there was gas.

**Other protections** (all on by default on a modern system):

| Protection | What it does |
|---|---|
| **ASLR** | the stack, heap and libraries are at **random** addresses every run (compare two runs of `03-stack-frame`) |
| **W^X / NX** | memory is writable **or** executable, never both: data on the stack cannot run as code |
| **PAC** (ARM64) | pointer authentication: return addresses are signed and checked before use (`pacibsp` in system libraries) |

None of them fixes the bug – they make it much harder to abuse. **The real fix is the code:** `snprintf`, `fgets`, checking sizes – everything from topics 01 to 05.

## Questions

## Useful commands

```bash
make asm/07-under-the-hood/01-asm-basics [OPT=-O0]    # show the assembly
make plain/07-under-the-hood/05-mitigations ARGS="loop AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
lldb build/07-under-the-hood/04-debug-me
objdump -d build/07-under-the-hood/01-asm-basics      # disassemble a compiled program
nm build/07-under-the-hood/01-asm-basics              # list its symbols (functions, globals)
```
