#include <stdio.h>
#include <limits.h>

int main(void)
{
    printf("char: %zu bytes\n", sizeof(char));
    printf("int: %zu bytes\n", sizeof(int));
    printf("float: %zu bytes\n", sizeof(float));
    printf("double: %zu bytes\n", sizeof(double));
    printf("long: %zu bytes\n", sizeof(long));
    printf("long long: %zu bytes\n", sizeof(long long));
    printf("short: %zu bytes\n", sizeof(short));
    printf("int *: %zu bytes\n", sizeof(int *));

    printf("%d\n", INT_MAX);
    printf("%d\n", INT_MIN);
    printf("%u\n", UINT_MAX);

    int x;
    int y;
    unsigned int z;

    x = INT_MAX + 1;
    y = INT_MIN - 1;
    z = UINT_MAX + 1;

    printf("%d\n", x);
    printf("%d\n", y);
    printf("%u\n", z);

    return 0;

}