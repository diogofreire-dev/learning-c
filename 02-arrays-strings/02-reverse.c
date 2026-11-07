#include <stdio.h>

// An array passed to a function is NOT copied: the function receives the address of
// the first element. So sizeof(arr) here would be 8 (the size of an address), not the
// size of the array – that is why the length must be passed as a separate parameter.

// const: this function promises not to change the array
void print_array(const int arr[], size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Swaps the first with the last, the second with the second-to-last, ... until the middle
void reverse(int arr[], size_t len)
{
    for (size_t i = 0; i < len / 2; i++)
    {
        int tmp = arr[i];
        arr[i] = arr[len - 1 - i];
        arr[len - 1 - i] = tmp;
    }
}

int main(void)
{
    int numbers[] = {1, 2, 3, 4, 5, 6, 7};
    size_t len = sizeof(numbers) / sizeof(numbers[0]);

    printf("Before: ");
    print_array(numbers, len);

    reverse(numbers, len);

    // The original array changed, because reverse worked on the same memory
    printf("After:  ");
    print_array(numbers, len);

    return 0;
}
