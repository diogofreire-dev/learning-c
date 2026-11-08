# learning-c

C exercises from my first year of a CTESP in Cybersecurity at ISTEC.

## Goal

Build a solid foundation in C (pointers and memory) so I can later understand Assembly and, eventually, software security.

## Plan (by topic)

- [x] 01 – Basics: types, operators, control flow, functions, printf/scanf
- [x] 02 – Arrays and strings, scope, the stack
- [x] 03 – Pointers
- [ ] 04 – Memory: malloc/free, stack vs heap, leaks, AddressSanitizer
- [ ] 05 – Structs, linked lists, argc/argv, file I/O
- [ ] 06 – Mini-Project: a `hexdump` clone
- [ ] 07 – Under the hood: `clang -S`, lldb, the stack frame, a buffer overflow

Each folder has the exercises (`.c`), an `exercises.md` with the statements and a `notes.md`.

## How to compile

From the repository root:

```bash
make run/01-basics/00-hello     # build and run
make 01-basics/00-hello         # only build
./build/01-basics/00-hello      # run an already built program
make leaks/04-memory/05-leak    # check for memory leaks (built without sanitizers)
make all                        # build every exercise
make clean                      # remove build/
```

Compiled programs go to `build/` (ignored by git), so the topic folders only have sources and notes. The `Makefile` compiles with `-Wall -Wextra -g -fsanitize=address,undefined`, so memory errors and undefined behaviour are reported at runtime.

## Next

- Assembly x86-64 (Linux, via Docker or a VM — this Mac is arm64)
- Binary exploitation: pwn.college, OverTheWire, picoCTF

## Resources

- Beej's Guide to C
- pwn.college
- OverTheWire (Bandit)
