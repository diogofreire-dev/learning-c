#include <stdio.h>
#include <stdlib.h>

int global_initialised = 42;   // data segment
int global_zero;               // bss segment (globals with no value start at 0)

struct place
{
    const char *name;
    const void *address;
};

// qsort compare function: sorts by address, from the lowest to the highest
int by_address(const void *a, const void *b)
{
    const struct place *x = a;
    const struct place *y = b;
    return (x->address > y->address) - (x->address < y->address);
}

int main(void)
{
    static int static_local = 7;           // lives with the globals, not on the stack
    const char *literal = "hello";         // the text itself is in read-only memory
    int local_a = 1;
    int local_b = 2;
    int *heap_a = malloc(sizeof(int));
    int *heap_b = malloc(sizeof(int));

    if (heap_a == NULL || heap_b == NULL)
    {
        free(heap_a);
        free(heap_b);
        return 1;
    }

    // A function's address converted to void * is not standard C, but works on every
    // common platform and is what a debugger shows
    struct place places[] = {
        {"function main (code)", (const void *) main},
        {"string literal", literal},
        {"global initialised", &global_initialised},
        {"global zero", &global_zero},
        {"static local", &static_local},
        {"heap_a", heap_a},
        {"heap_b (malloc'd after a)", heap_b},
        {"local_a (stack)", &local_a},
        {"local_b (stack)", &local_b},
    };
    size_t count = sizeof(places) / sizeof(places[0]);

    qsort(places, count, sizeof(places[0]), by_address);

    printf("From the lowest to the highest address:\n");
    for (size_t i = 0; i < count; i++)
    {
        printf("  %-28s %p\n", places[i].name, places[i].address);
    }

    free(heap_a);
    free(heap_b);
    return 0;
}
