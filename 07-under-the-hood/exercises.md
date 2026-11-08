# 07 – Under the hood: exercises

This topic is an **introduction**: how C becomes machine instructions, and how the stack really works. It is ARM64 assembly (this Mac); x86-64 comes later, in a Linux environment. The ideas – registers, the stack, calls and returns – are the same on both.

```bash
make asm/07-under-the-hood/01-asm-basics            # the assembly, optimised (-O1)
make asm/07-under-the-hood/01-asm-basics OPT=-O0    # without optimisation
make run/07-under-the-hood/03-stack-frame
make plain/07-under-the-hood/05-mitigations ARGS="canary AAAA..."   # without sanitizers
```

## ARM64 in 2 minutes

| Thing | Meaning |
|---|---|
| `x0`–`x30` | 31 general registers of 64 bits; `w0`–`w30` are their lower 32 bits (used for `int`) |
| `x0`–`x7` | the first 8 **arguments** of a function; `x0` (`w0`) is also the **return value** |
| `x29` (`fp`) | **frame pointer** – where the current stack frame starts |
| `x30` (`lr`) | **link register** – the **return address** (where to continue after `ret`) |
| `sp` | **stack pointer** – the top of the stack |
| `pc` | the address of the instruction running now |

| Instruction | Meaning |
|---|---|
| `mov w0, #5` | `w0 = 5` |
| `add w0, w0, w1` | `w0 = w0 + w1` (`sub`, `mul`, `sdiv` the same way) |
| `ldr w8, [sp, #12]` | **load** 4 bytes from memory at `sp + 12` into `w8` |
| `str w8, [sp, #12]` | **store** `w8` into memory at `sp + 12` |
| `stp x29, x30, [sp, #-16]!` | store a **pair** (save fp and the return address on the stack) |
| `cmp w0, w1` + `b.gt label` | compare, then **branch** (jump) if greater |
| `bl function` | **branch with link**: call a function (puts the return address in `x30`) |
| `ret` | return: jump to the address in `x30` |

## 1. `01-asm-basics.c` – reading your first assembly
- Write `add(a, b)`, `square(x)`, `max(a, b)` and `sum_to(n)` (a loop that adds `1 + 2 + ... + n`).
- Look at the assembly with `OPT=-O0` and with the default `-O1`. Which is shorter? Where are the variables in each?
- Find in `max` the instruction that replaces the `if`. And what did `-O1` do to the loop in `sum_to`?

## 2. `02-calling.c` – how arguments are passed
- Write a function with **10** `int` arguments that returns their sum, and call it from `main`.
- In the assembly of the call, which arguments go in registers and which go on the stack? Why?

## 3. `03-stack-frame.c` – seeing the stack frame
- In a recursive function, print the address of a local, the **frame pointer** (`__builtin_frame_address(0)`) and the **return address** (`__builtin_return_address(0)`).
- How far apart are the frames? Where does each return address point to?

## 4. `04-debug-me.c` – your first lldb session
- The program should print the average of some grades, but the result is wrong. **Do not read the code carefully** – find the bug with `lldb` (see the guide below).

## 5. `05-mitigations.c` – the protections against stack overflows
- A function copies its argument into a 16-byte buffer, in one of three ways: `strcpy`, a loop written by hand, or `snprintf`.
- Run each one with a short and a long name, in the normal (ASan) build and in the **plain** build. Which protection stops each one: AddressSanitizer, **FORTIFY_SOURCE** or the **stack canary**?
- Why must the canary be checked **before** the function returns?

## lldb guide

```bash
make 07-under-the-hood/04-debug-me
lldb build/07-under-the-hood/04-debug-me
```

| Command | Short | What it does |
|---|---|---|
| `breakpoint set --name average` | `b average` | stop when `average` is called |
| `breakpoint set --line 12` | `b 12` | stop at line 12 |
| `run` | `r` | start the program |
| `next` | `n` | run the next line (step **over** calls) |
| `step` | `s` | run the next line (step **into** calls) |
| `continue` | `c` | run until the next breakpoint |
| `frame variable` | `v` | show every local variable |
| `print i` | `p i` | show one variable or expression |
| `bt` | | backtrace: the chain of calls that led here |
| `register read` | | show the registers (`x0`, `fp`, `lr`, `sp`, `pc`...) |
| `memory read --size 4 --format x --count 8 &grades` | `x/8xw &grades` | show raw memory |
| `disassemble` | `di` | show the assembly of the current function |
| `stepi` / `nexti` | `si` / `ni` | run one **instruction** (not one line) |
| `quit` | `q` | exit |
