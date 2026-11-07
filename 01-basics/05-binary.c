#include <stdio.h>

int main(void)
{
    unsigned int n;

    printf("Enter a number: ");
    if (scanf("%u", &n) != 1)
    {
        printf("Invalid input\n");
        return 1;
    }

    int ones = 0;

    printf("Binary: ");
    // Go from bit 31 (leftmost) down to bit 0 (rightmost).
    // n >> i moves bit i to the rightmost position; & 1 keeps only that bit.
    for (int i = 31; i >= 0; i--)
    {
        unsigned int bit = (n >> i) & 1;
        printf("%u", bit);
        ones += bit;

        // A space every 4 bits makes it readable (each group of 4 = one hex digit)
        if (i % 4 == 0 && i != 0)
        {
            printf(" ");
        }
    }
    printf("\n");

    printf("Hex: %x\n", n);       // no padding
    printf("Hex: %08x\n", n);     // padded with zeros to 8 digits (8 hex digits = 32 bits)
    printf("Bits set to 1: %d\n", ones);

    return 0;
}
