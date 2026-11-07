#include <stdio.h>

int main(void)
{
    int grades[] = {14, 9, 17, 12, 20, 8, 15};

    // sizeof(grades) is the whole array in bytes (7 * 4 = 28); dividing by the size
    // of one element gives the number of elements, so the 7 is never written by hand
    size_t count = sizeof(grades) / sizeof(grades[0]);

    int sum = 0;
    int min = grades[0];
    int max = grades[0];

    for (size_t i = 0; i < count; i++)
    {
        sum += grades[i];
        if (grades[i] < min)
        {
            min = grades[i];
        }
        if (grades[i] > max)
        {
            max = grades[i];
        }
    }

    printf("Array: %zu bytes, %zu elements\n\n", sizeof(grades), count);

    // The elements are stored one after another: each address is 4 bytes (sizeof(int))
    // after the previous one. %p prints an address and expects a (void *).
    for (size_t i = 0; i < count; i++)
    {
        printf("grades[%zu] = %2d   at %p\n", i, grades[i], (void *) &grades[i]);
    }

    printf("\nSum: %d\n", sum);
    printf("Average: %.2f\n", (double) sum / count);
    printf("Min: %d\n", min);
    printf("Max: %d\n", max);

    return 0;
}
