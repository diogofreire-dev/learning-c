// A clone of `hexdump -C`: same output, byte for byte.
//
// Usage: hexdump [-v] [-s OFFSET] [-n LENGTH] [FILE]
//   -s OFFSET  skip OFFSET bytes from the start (decimal, or hex with 0x)
//   -n LENGTH  show at most LENGTH bytes
//   -v         show every line (do not replace repeated lines with *)
//   FILE       file to read; without it, read standard input
//
// Example: make run/06-project-hexdump/hexdump ARGS="-s 0x10 -n 32 01-basics/00-hello.c"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BYTES_PER_LINE 16

typedef struct
{
    unsigned long long skip;     // -s
    unsigned long long length;   // -n
    int has_length;              // was -n given? (-n 0 is valid and different from no -n)
    int verbose;                 // -v
    const char *path;            // NULL = standard input
} Options;

void usage(const char *program)
{
    fprintf(stderr, "Usage: %s [-v] [-s OFFSET] [-n LENGTH] [FILE]\n", program);
}

// Like parse_int in topic 05, but base 0 also accepts hex (0x10) and octal (020)
int parse_size(const char *text, unsigned long long *out)
{
    char *end;
    errno = 0;
    unsigned long long value = strtoull(text, &end, 0);
    // strtoull accepts "-1" and turns it into a huge number, so reject the sign by hand
    if (end == text || *end != '\0' || errno == ERANGE || text[0] == '-')
    {
        return -1;
    }
    *out = value;
    return 0;
}

// Returns 0 on success, -1 on invalid options
int parse_options(int argc, char *argv[], Options *opts)
{
    *opts = (Options) {0};

    // getopt walks through argv for us: "s:" means -s takes a value (put in optarg),
    // "v" means -v takes none. It returns -1 when there are no more options.
    int opt;
    while ((opt = getopt(argc, argv, "s:n:v")) != -1)
    {
        switch (opt)
        {
            case 's':
                if (parse_size(optarg, &opts->skip) != 0)
                {
                    fprintf(stderr, "Invalid offset: %s\n", optarg);
                    return -1;
                }
                break;
            case 'n':
                if (parse_size(optarg, &opts->length) != 0)
                {
                    fprintf(stderr, "Invalid length: %s\n", optarg);
                    return -1;
                }
                opts->has_length = 1;
                break;
            case 'v':
                opts->verbose = 1;
                break;
            default:   // getopt already printed what was wrong
                return -1;
        }
    }

    // optind = index of the first argument that is not an option
    if (argc - optind > 1)
    {
        return -1;
    }
    if (argc - optind == 1)
    {
        opts->path = argv[optind];
    }
    return 0;
}

// Reads and throws away up to `count` bytes. Returns how many were really skipped
// (fewer if the input ends first). Reading instead of fseek also works on stdin.
unsigned long long skip_bytes(FILE *in, unsigned long long count)
{
    unsigned char discard[4096];
    unsigned long long skipped = 0;

    while (skipped < count)
    {
        size_t want = sizeof(discard);
        if (count - skipped < want)
        {
            want = count - skipped;
        }
        size_t got = fread(discard, 1, want, in);
        skipped += got;
        if (got < want)
        {
            break;   // end of input (or an error, checked later with ferror)
        }
    }
    return skipped;
}

// Prints one line in the hexdump -C format:
// 00000000  23 69 6e 63 6c 75 64 65  20 3c 73 74 64 69 6f 2e  |#include <stdio.|
void print_line(unsigned long long offset, const unsigned char *bytes, size_t count)
{
    printf("%08llx  ", offset);

    for (size_t i = 0; i < BYTES_PER_LINE; i++)
    {
        if (i < count)
        {
            printf("%02x ", bytes[i]);
        }
        else
        {
            printf("   ");   // a short last line keeps the | column aligned
        }
        if (i == 7)
        {
            printf(" ");     // extra space between the two groups of 8
        }
    }

    printf(" |");
    for (size_t i = 0; i < count; i++)
    {
        // Printable ASCII (0x20 to 0x7e) as itself, anything else as '.'
        putchar(isprint(bytes[i]) ? bytes[i] : '.');
    }
    printf("|\n");
}

// Reads the input and prints every line. Returns 0 on success, -1 on a read error.
int dump(FILE *in, const Options *opts)
{
    // -n 0 asks for nothing: hexdump prints nothing at all, not even the final offset
    if (opts->has_length && opts->length == 0)
    {
        return 0;
    }

    unsigned long long offset = skip_bytes(in, opts->skip);
    unsigned long long remaining = opts->length;

    unsigned char line[BYTES_PER_LINE];
    unsigned char previous[BYTES_PER_LINE];
    size_t previous_count = 0;
    int squeezing = 0;   // already printed '*' for the current run of repeated lines?

    for (;;)
    {
        size_t want = BYTES_PER_LINE;
        if (opts->has_length && remaining < want)
        {
            want = remaining;
        }
        if (want == 0)
        {
            break;
        }

        size_t count = fread(line, 1, want, in);
        if (count == 0)
        {
            break;
        }

        // A full line identical to the previous one is replaced by a single '*'
        int repeated = !opts->verbose && count == BYTES_PER_LINE &&
                       previous_count == BYTES_PER_LINE &&
                       memcmp(line, previous, BYTES_PER_LINE) == 0;
        if (repeated)
        {
            if (!squeezing)
            {
                printf("*\n");
                squeezing = 1;
            }
        }
        else
        {
            print_line(offset, line, count);
            squeezing = 0;
        }

        memcpy(previous, line, count);
        previous_count = count;
        offset += count;
        remaining -= count;   // only used when has_length is set

        if (count < want)
        {
            break;   // end of input
        }
    }

    if (ferror(in))
    {
        return -1;
    }

    // The last line is the total offset, unless nothing was read or skipped at all
    if (offset != 0)
    {
        printf("%08llx\n", offset);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    Options opts;
    if (parse_options(argc, argv, &opts) != 0)
    {
        usage(argv[0]);
        return 1;
    }

    FILE *in = stdin;
    if (opts.path != NULL)
    {
        in = fopen(opts.path, "rb");
        if (in == NULL)
        {
            fprintf(stderr, "%s: %s: %s\n", argv[0], opts.path, strerror(errno));
            return 1;
        }
    }

    int status = 0;
    if (dump(in, &opts) != 0)
    {
        fprintf(stderr, "%s: read error\n", argv[0]);
        status = 1;
    }

    if (in != stdin)
    {
        fclose(in);
    }
    return status;
}
