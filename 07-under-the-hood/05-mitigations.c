#include <stdio.h>
#include <string.h>

// Usage: 05-mitigations strcpy|loop|safe NAME
//
// Each mode copies NAME into a 16-byte buffer on the stack. With a long NAME,
// "strcpy" and "loop" overflow it – on purpose – to see which protection stops them.
// Run each mode in both builds:
//   make run/07-under-the-hood/05-mitigations ARGS="loop AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
//   make plain/07-under-the-hood/05-mitigations ARGS="loop AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"

// The compiler knows buffer has 16 bytes, so on macOS it replaces strcpy with
// __strcpy_chk, which checks the size BEFORE copying (FORTIFY_SOURCE)
void copy_strcpy(const char *name)
{
    char buffer[16];
    strcpy(buffer, name);
    printf("Hello, %s\n", buffer);
}

// A hand-written copy: the compiler cannot check it. Only the stack canary is left:
// a secret value placed on the stack at the start of the function and checked just
// before it returns – if it changed, something wrote past the buffer.
void copy_loop(const char *name)
{
    char buffer[16];
    size_t i = 0;
    while (name[i] != '\0')
    {
        buffer[i] = name[i];
        i++;
    }
    buffer[i] = '\0';
    printf("Hello, %s\n", buffer);
}

// The fix: never write more than the buffer can hold (a long name is cut)
void copy_safe(const char *name)
{
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%s", name);
    printf("Hello, %s\n", buffer);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s strcpy|loop|safe NAME\n", argv[0]);
        return 1;
    }

    // Without this, the output still waiting in the buffer is lost when the program is
    // stopped by a protection (an abort does not flush stdout)
    setvbuf(stdout, NULL, _IONBF, 0);

    if (strcmp(argv[1], "strcpy") == 0)
    {
        copy_strcpy(argv[2]);
    }
    else if (strcmp(argv[1], "loop") == 0)
    {
        copy_loop(argv[2]);
    }
    else if (strcmp(argv[1], "safe") == 0)
    {
        copy_safe(argv[2]);
    }
    else
    {
        fprintf(stderr, "Unknown mode: %s\n", argv[1]);
        return 1;
    }

    // Only reached if the function returned normally
    printf("Back in main safely\n");
    return 0;
}
