# 05 – Structs, linked lists, argc/argv and files: exercises

Write each exercise in its own numbered file in this folder, then build and run it from the repository root:

```bash
make run/05-structs-files/01-structs
make run/05-structs-files/03-args ARGS="10 20 30"     # with command-line arguments
```

Fix every compiler warning, not just the errors.

## 1. `01-structs.c` – grouping data
- Create a `struct student` with a name, an age and 3 grades, and use `typedef` to call it just `Student`.
- Make an array of 3 students and print them with a function that receives a `const Student *` (use `->`).
- Write a function that changes a student's age through a pointer.
- Compare `sizeof` of `struct { char a; int b; char c; }` and `struct { int b; char a; char c; }`. Why are they different? Use `offsetof` to find the **padding**.

## 2. `02-linked-list.c` – a linked list
- Each `Node` has an `int value` and a pointer to the next `Node`; the list ends with `NULL`.
- Write `push_front`, `push_back`, `print_list`, `find`, `remove_value` and `free_list`.
- Every node is allocated with `malloc`. Check with `make leaks/05-structs-files/02-linked-list` that nothing leaks.
- When is a linked list better than an array? When is it worse?

## 3. `03-args.c` – command-line arguments
- Print `argc` and every `argv[i]`. What is `argv[0]`?
- Add up every argument as an integer: `make run/05-structs-files/03-args ARGS="10 20 30"` → 60.
- Use `strtol` (not `atoi`) and reject arguments that are not numbers, like `12abc` or `abc`. Why is `atoi` a bad idea?

## 4. `04-wc.c` – counting a file
- A simple clone of `wc`: receive a file name in `argv[1]` and print its lines, words and characters.
- Read it with `fopen` and `fgetc`, and check for errors (file missing → `perror`).
- Compare your result with the real `wc` on the same file.

## 5. `05-copy.c` – copying any file
- `copy SOURCE DEST`: copy a file in blocks of 4096 bytes with `fread`/`fwrite`, in binary mode (`"rb"` / `"wb"`).
- Test it with a text file and with a compiled program, and compare with `cmp`.
- Close every file, also when something fails in the middle.

## 6. `06-contacts.c` – a small contacts app
- A `Contact` has a name and a phone. Contacts are saved in a text file, one per line: `name,phone`.
- Commands: `contacts FILE add NAME PHONE`, `contacts FILE list`, `contacts FILE find NAME`.
- Load the file into a dynamic array (topic 04), and reject names or phones with a `,` or that are too long.

## 7. `07-format-string.c` – a format string bug (on purpose)
- Read a name with `fgets` and print it with `printf(name)` instead of `printf("%s", name)`. Keep a local `int secret = 0x1337` in `main`.
- Type a normal name. Then type `%p %p %p %p %p %p %p %p`. Can you find the secret?
- What does the compiler warn about?
- What happens with `%n`? Write the fix in `notes.md`.

> 🔐 Format string bugs let an attacker **read** the stack (passwords, addresses) and, with `%n`, even **write** to memory.
