#include <stdio.h>
#include <stdlib.h>

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int divide(int a, int b) { return b != 0 ? a / b : 0; }

// qsort calls these to compare two elements: negative if a comes first, positive if b
// does, 0 if equal. They receive const void * because qsort works with any type.
int ascending(const void *a, const void *b)
{
    int x = *(const int *) a;
    int y = *(const int *) b;
    // (x > y) - (x < y) gives -1, 0 or 1; x - y could overflow for large values
    return (x > y) - (x < y);
}

int descending(const void *a, const void *b)
{
    return ascending(b, a);
}

void print_array(const int arr[], size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(void)
{
    // An array of 4 pointers to functions that take (int, int) and return int.
    // A function's name used as a value is its address, just like an array's name.
    int (*operations[])(int, int) = {add, sub, mul, divide};
    const char *names[] = {"add", "sub", "mul", "divide"};

    for (int i = 0; i < 4; i++)
    {
        // Calling through the pointer: the program jumps to the address stored there
        printf("%-6s(12, 4) = %-3d (function at %p)\n",
               names[i], operations[i](12, 4), (void *) operations[i]);
    }

    int numbers[] = {42, 7, -3, 19, 0, 8};
    size_t len = sizeof(numbers) / sizeof(numbers[0]);

    qsort(numbers, len, sizeof(numbers[0]), ascending);
    printf("\nAscending:  ");
    print_array(numbers, len);

    qsort(numbers, len, sizeof(numbers[0]), descending);
    printf("Descending: ");
    print_array(numbers, len);

    return 0;
}
