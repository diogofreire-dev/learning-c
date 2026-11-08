#include <stdio.h>

int main(void)
{
    int x = 42;

    // int *p: p is a pointer to an int – it stores the ADDRESS of an int.
    // &x: "the address of x".
    int *p = &x;

    printf("x  = %d      (the value)\n", x);
    printf("&x = %p   (the address of x)\n", (void *) &x);
    printf("p  = %p   (p stores the address of x, so it is equal to &x)\n", (void *) p);
    printf("*p = %d      (go to the address in p and read the int there = x)\n", *p);
    printf("&p = %p   (p is a variable too, so it has its own address)\n", (void *) &p);

    // *p on the left side: write into the int that p points to, which is x
    *p = 100;
    printf("\nAfter *p = 100, x = %d\n", x);

    // Every pointer stores an address, and on a 64-bit machine every address is 8 bytes,
    // no matter the type it points to. The type only says how to read what is there.
    printf("\nsizeof(char *)   = %zu\n", sizeof(char *));
    printf("sizeof(int *)    = %zu\n", sizeof(int *));
    printf("sizeof(double *) = %zu\n", sizeof(double *));

    // NULL means "points to nothing". An uninitialised pointer holds a random address
    // that may look valid; NULL can be checked, and using it crashes right away
    // instead of silently corrupting memory.
    int *nothing = NULL;
    if (nothing == NULL)
    {
        printf("\nnothing is NULL: never use *nothing\n");
    }

    return 0;
}
