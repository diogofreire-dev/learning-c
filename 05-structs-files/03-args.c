#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// Converts text to an int, checking everything atoi ignores.
// Returns 0 on success (result in *out), -1 if text is not a valid int.
int parse_int(const char *text, int *out)
{
    char *end;
    errno = 0;
    long value = strtol(text, &end, 10);   // 10 = decimal

    // end points to the first character strtol did not use
    if (end == text || *end != '\0')
    {
        return -1;   // nothing was converted ("abc") or there is junk after it ("12abc")
    }
    if (errno == ERANGE || value < INT_MIN || value > INT_MAX)
    {
        return -1;   // too big for a long or for an int
    }
    *out = (int) value;
    return 0;
}

// argc = how many strings are in argv; argv[0] is the program itself,
// argv[1] to argv[argc - 1] are the arguments, and argv[argc] is NULL
int main(int argc, char *argv[])
{
    printf("argc = %d\n", argc);
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d] = \"%s\"\n", i, argv[i]);
    }

    if (argc < 2)
    {
        printf("\nUsage: %s NUMBER...\n", argv[0]);
        return 1;
    }

    long long sum = 0;   // bigger than int, so adding many ints cannot overflow
    for (int i = 1; i < argc; i++)
    {
        int n;
        if (parse_int(argv[i], &n) != 0)
        {
            // Errors go to stderr, not stdout
            fprintf(stderr, "Not a valid integer: \"%s\"\n", argv[i]);
            return 1;
        }
        sum += n;
    }

    printf("\nSum = %lld\n", sum);
    return 0;
}
