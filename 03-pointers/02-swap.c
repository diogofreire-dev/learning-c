#include <stdio.h>

// C passes arguments BY VALUE: a and b are copies, so swapping them changes nothing outside
void swap_wrong(int a, int b)
{
    int tmp = a;
    a = b;
    b = tmp;
}

// With pointers the function receives the ADDRESSES of the caller's variables,
// so *a and *b are the original variables themselves
void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// A function can only return one value; pointers let it "return" more.
// This is also why scanf needs &: it has to write into your variable.
void min_max(const int arr[], size_t len, int *min, int *max)
{
    *min = arr[0];
    *max = arr[0];
    for (size_t i = 1; i < len; i++)
    {
        if (arr[i] < *min)
        {
            *min = arr[i];
        }
        if (arr[i] > *max)
        {
            *max = arr[i];
        }
    }
}

int main(void)
{
    int x = 1;
    int y = 2;

    swap_wrong(x, y);
    printf("After swap_wrong: x = %d, y = %d (nothing changed)\n", x, y);

    swap(&x, &y);
    printf("After swap:       x = %d, y = %d\n", x, y);

    int numbers[] = {7, -3, 12, 0, 5};
    int min, max;
    min_max(numbers, sizeof(numbers) / sizeof(numbers[0]), &min, &max);
    printf("\nmin = %d, max = %d\n", min, max);

    return 0;
}
