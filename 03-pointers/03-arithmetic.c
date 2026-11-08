#include <stdio.h>

int main(void)
{
    char c[2];
    int i[2];
    double d[2];

    char *pc = c;      // an array name used as a value is the address of its first element
    int *pi = i;
    double *pd = d;

    // p + 1 does not add 1 byte: it moves to the NEXT ELEMENT, i.e. sizeof(type) bytes
    printf("char *:   %p -> %p (+%zu byte)\n", (void *) pc, (void *) (pc + 1), sizeof(*pc));
    printf("int *:    %p -> %p (+%zu bytes)\n", (void *) pi, (void *) (pi + 1), sizeof(*pi));
    printf("double *: %p -> %p (+%zu bytes)\n", (void *) pd, (void *) (pd + 1), sizeof(*pd));

    int arr[] = {10, 20, 30, 40, 50};
    size_t len = sizeof(arr) / sizeof(arr[0]);

    // arr[k] is defined as *(arr + k): "move k elements forward, then read"
    printf("\narr[2] = %d, *(arr + 2) = %d\n", arr[2], *(arr + 2));

    // Walk the array with a pointer only: start at the first element and stop at
    // the address just past the last one (end may be computed, but never read)
    int sum = 0;
    const int *end = arr + len;
    for (const int *p = arr; p < end; p++)
    {
        sum += *p;
    }
    printf("Sum with a pointer: %d\n", sum);

    // Subtracting two pointers into the same array gives the number of ELEMENTS
    // between them (not bytes). The result type is ptrdiff_t, printed with %td.
    int *first = &arr[0];
    int *last = &arr[4];
    printf("last - first = %td elements (%td bytes)\n",
           last - first, (char *) last - (char *) first);

    return 0;
}
