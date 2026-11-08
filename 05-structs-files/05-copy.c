#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s SOURCE DEST\n", argv[0]);
        return 1;
    }

    // "rb"/"wb": binary mode – bytes are copied exactly as they are (on macOS and Linux
    // text and binary mode are the same, but on Windows text mode changes line endings)
    FILE *source = fopen(argv[1], "rb");
    if (source == NULL)
    {
        perror(argv[1]);
        return 1;
    }

    FILE *dest = fopen(argv[2], "wb");
    if (dest == NULL)
    {
        perror(argv[2]);
        fclose(source);   // the source is already open: close it before leaving
        return 1;
    }

    unsigned char buffer[4096];
    size_t total = 0;
    size_t n;
    int failed = 0;

    // fread returns how many bytes it really read: 4096, fewer at the end, 0 when done
    while ((n = fread(buffer, 1, sizeof(buffer), source)) > 0)
    {
        // Write exactly n bytes – not sizeof(buffer), the last block is usually shorter
        if (fwrite(buffer, 1, n, dest) != n)
        {
            perror(argv[2]);
            failed = 1;
            break;
        }
        total += n;
    }

    if (ferror(source))
    {
        perror(argv[1]);
        failed = 1;
    }

    fclose(source);
    // fclose writes whatever is still in the buffer, so it can fail too (e.g. disk full)
    if (fclose(dest) != 0)
    {
        perror(argv[2]);
        failed = 1;
    }

    if (failed)
    {
        return 1;
    }

    printf("Copied %zu bytes from %s to %s\n", total, argv[1], argv[2]);
    return 0;
}
