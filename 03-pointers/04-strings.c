#include <stdio.h>

// Move the pointer until it reaches the '\0'; the length is how far it moved
size_t my_strlen(const char *s)
{
    const char *p = s;
    while (*p != '\0')
    {
        p++;
    }
    return p - s;
}

// Two pointers, one at each end, swap characters and walk towards the middle
void reverse(char *s)
{
    size_t len = my_strlen(s);
    if (len == 0)
    {
        return;
    }

    char *left = s;
    char *right = s + len - 1;
    while (left < right)
    {
        char tmp = *left;
        *left = *right;
        *right = tmp;
        left++;
        right--;
    }
}

int main(void)
{
    // a is an ARRAY of 6 chars on the stack, filled with a copy of "hello": it can be changed
    char a[] = "hello";

    // b is a POINTER to a string literal, which lives in read-only memory.
    // Writing to it (b[0] = 'H') is undefined behaviour – on macOS the program crashes.
    // That is why it is declared const: the compiler then refuses b[0] = 'H'.
    const char *b = "hello";

    printf("my_strlen(\"%s\") = %zu\n", a, my_strlen(a));

    a[0] = 'H';
    printf("a after a[0] = 'H': %s\n", a);

    reverse(a);
    printf("a reversed: %s\n", a);

    printf("\nsizeof(a) = %zu (the whole array)\n", sizeof(a));
    printf("sizeof(b) = %zu (just a pointer)\n", sizeof(b));

    return 0;
}
