#include <stdio.h>
#include <stdlib.h>

// BUGS ON PURPOSE – each option makes AddressSanitizer stop the program.
// Without ASan these would silently corrupt the heap (or crash much later).

void heap_overflow(void)
{
    int *arr = malloc(4 * sizeof(int));
    if (arr == NULL)
    {
        return;
    }
    // Valid indexes are 0 to 3; i <= 4 writes one int past the end of the block
    for (int i = 0; i <= 4; i++)
    {
        arr[i] = i;
    }
    free(arr);
}

void use_after_free(void)
{
    int *value = malloc(sizeof(int));
    if (value == NULL)
    {
        return;
    }
    *value = 42;
    free(value);
    // value still holds the old address, but the block was given back to the allocator
    printf("value = %d\n", *value);
}

void double_free(void)
{
    int *value = malloc(sizeof(int));
    if (value == NULL)
    {
        return;
    }
    free(value);
    // Freeing twice corrupts the allocator's bookkeeping – exploitable in real attacks
    free(value);
}

int main(void)
{
    int choice;

    printf("1) heap buffer overflow\n");
    printf("2) use-after-free\n");
    printf("3) double free\n");
    printf("Choose: ");

    if (scanf("%d", &choice) != 1)
    {
        return 1;
    }

    switch (choice)
    {
        case 1:
            heap_overflow();
            break;
        case 2:
            use_after_free();
            break;
        case 3:
            double_free();
            break;
        default:
            printf("Unknown option\n");
            return 1;
    }

    printf("No error detected\n");
    return 0;
}
