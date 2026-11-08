#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    size_t capacity = 2;   // how many ints fit in the block
    size_t count = 0;      // how many are actually used
    int *numbers = malloc(capacity * sizeof(*numbers));
    if (numbers == NULL)
    {
        return 1;
    }

    printf("Enter integers (0 to stop):\n");

    int value;
    while (scanf("%d", &value) == 1 && value != 0)
    {
        if (count == capacity)
        {
            // Doubling (instead of +1) means realloc is rarely needed: 2, 4, 8, 16...
            size_t new_capacity = capacity * 2;

            // realloc returns a NEW address (it may move the block) or NULL on failure.
            // If the result went straight into numbers, a NULL would overwrite the only
            // pointer to the old block: its data would be lost and it could never be freed.
            int *bigger = realloc(numbers, new_capacity * sizeof(*numbers));
            if (bigger == NULL)
            {
                printf("Out of memory\n");
                free(numbers);
                return 1;
            }

            printf("  (full at %zu: capacity %zu -> %zu, block %p -> %p)\n",
                   count, capacity, new_capacity, (void *) numbers, (void *) bigger);
            numbers = bigger;
            capacity = new_capacity;
        }

        numbers[count] = value;
        count++;
    }

    printf("Stored %zu numbers (capacity %zu): ", count, capacity);
    for (size_t i = 0; i < count; i++)
    {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    free(numbers);
    return 0;
}
