#include <stdio.h>

// void * can point to anything; converting it to unsigned char * lets us read
// any object one byte at a time (unsigned char is the C type for "a raw byte")
void dump(const void *data, size_t size)
{
    const unsigned char *bytes = data;
    for (size_t i = 0; i < size; i++)
    {
        printf("%02x ", bytes[i]);
    }
    printf("\n");
}

int main(void)
{
    int n = 0x12345678;
    printf("int 0x12345678: ");
    dump(&n, sizeof(n));
    // Prints 78 56 34 12: the least significant byte is stored first (little-endian)

    int minus_one = -1;
    printf("int -1:         ");
    dump(&minus_one, sizeof(minus_one));
    // All bits set to 1 (two's complement)

    float f = 1.0f;
    printf("float 1.0:      ");
    dump(&f, sizeof(f));
    // 0x3f800000 in IEEE 754 format, also stored little-endian: 00 00 80 3f

    char text[] = "Hi!";
    printf("char[] \"Hi!\":   ");
    dump(text, sizeof(text));
    // ASCII codes plus the '\0'

    return 0;
}
