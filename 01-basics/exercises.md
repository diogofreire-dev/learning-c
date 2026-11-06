# 01 – Basics: exercises

Write each exercise in its own file in this folder, then build and run it from the repository root:

```bash
make 01-basics/sizes && ./01-basics/sizes
```

Fix every compiler warning, not just the errors.

## 1. `sizes.c` – types and their limits
- Print `sizeof` for `char`, `short`, `int`, `long`, `long long`, `float`, `double` and `int *`.
- Print `INT_MAX`, `INT_MIN` and `UINT_MAX` (from `<limits.h>`).
- What happens with `UINT_MAX + 1`? Try `INT_MAX + 1` too and read what the sanitizer says.
- Hint: `sizeof` returns a `size_t`, which is printed with `%zu`.

> 🔐 Why it matters: integer overflows are a classic cause of vulnerabilities (for example, a size calculation that wraps around to a tiny number).

## 2. `calc.c` – switch and input validation
- Read `number operator number` (for example `7 / 2`) with `scanf`.
- Use a `switch` on the operator to handle `+ - * / %`.
- Reject division by zero and unknown operators.
- Check the **return value** of `scanf`: what happens if you type `abc`?

## 3. `average.c` – loops and integer division
- Read integers until the user types `0`, then print the count, the sum and the average.
- Print the average with two decimal places. Why does `sum / count` give the wrong result? Fix it with a cast.

## 4. `primes.c` – functions
- Write `int is_prime(int n)` and use it to print every prime up to `N` (read from the user).
- Keep `main` short: it reads input and calls the functions.

## 5. `binary.c` – bitwise operators
- Read an `unsigned int` and print it in binary (32 bits), using only `>>` and `&`.
- Also print it in hexadecimal with `%x` and `%08x`.
- Bonus: count how many bits are set to 1.

> 🔐 You will be reading hex and binary all the time in Assembly and when analysing memory.

## 6. `hello-name.c` – your first bug (already in this folder)
- Run it and type a name longer than 100 characters. What does AddressSanitizer report?
- Explain in `notes.md` **why** it happens.
- Fix it in two ways: with a width in `scanf` (`%99s`) and with `fgets`. Which one handles names with spaces?

> 🔐 This is a stack buffer overflow, the same kind of bug you will exploit on purpose in topic 07.
