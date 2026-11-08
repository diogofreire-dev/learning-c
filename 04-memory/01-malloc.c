#include <stdio.h>
#include <stdlib.h>

void print_array(const int *arr, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(void)
{
    size_t n;

    printf("How many numbers? ");
    if (scanf("%zu", &n) != 1 || n == 0)
    {
        printf("Invalid input\n");
        return 1;
    }

    // malloc(bytes) reserves memory on the heap and returns its address.
    // n * sizeof(*numbers) = n ints; sizeof(*numbers) stays right if the type changes.
    // The memory is NOT initialised: it contains whatever was there before.
    int *numbers = malloc(n * sizeof(*numbers));

    // malloc returns NULL when it cannot get the memory – always check it
    if (numbers == NULL)
    {
        printf("Out of memory\n");
        return 1;
    }

    for (size_t i = 0; i < n; i++)
    {
        numbers[i] = (int) (i * i);
    }
    printf("malloc'd squares: ");
    print_array(numbers, n);

    // calloc(count, size) also checks count * size for overflow and fills the memory with zeros
    int *zeros = calloc(n, sizeof(*zeros));
    if (zeros == NULL)
    {
        printf("Out of memory\n");
        free(numbers);
        return 1;
    }
    printf("calloc'd array:   ");
    print_array(zeros, n);

    // Every malloc/calloc needs exactly one free. After free the pointer still holds the
    // old address, so setting it to NULL avoids using it by mistake.
    free(numbers);
    numbers = NULL;
    free(zeros);
    zeros = NULL;

    return 0;
}
