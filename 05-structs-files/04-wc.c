#include <ctype.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s FILE\n", argv[0]);
        return 1;
    }

    // fopen returns a FILE * (a handle to the open file), or NULL on error
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        // perror prints our text plus the reason from the system (e.g. "No such file or directory")
        perror(argv[1]);
        return 1;
    }

    long lines = 0;
    long words = 0;
    long chars = 0;
    int in_word = 0;

    // fgetc returns an int, not a char: it must fit every byte (0-255) AND EOF (-1),
    // the special value for "end of file"
    int c;
    while ((c = fgetc(file)) != EOF)
    {
        chars++;
        if (c == '\n')
        {
            lines++;
        }

        // A word starts when a non-space comes after a space (or at the start)
        if (isspace(c))
        {
            in_word = 0;
        }
        else if (!in_word)
        {
            in_word = 1;
            words++;
        }
    }

    // EOF also comes back on a read error; ferror tells them apart
    if (ferror(file))
    {
        perror(argv[1]);
        fclose(file);
        return 1;
    }
    fclose(file);

    // Same order and meaning as wc: lines, words, bytes
    printf("%8ld %7ld %7ld %s\n", lines, words, chars, argv[1]);
    return 0;
}
