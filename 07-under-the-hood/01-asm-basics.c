#include <stdio.h>

// Look at the assembly of these functions:
//   make asm/07-under-the-hood/01-asm-basics OPT=-O0
//   make asm/07-under-the-hood/01-asm-basics

int add(int a, int b)
{
    return a + b;
}

int square(int x)
{
    return x * x;
}

int max(int a, int b)
{
    if (a > b)
    {
        return a;
    }
    return b;
}

int sum_to(int n)
{
    int sum = 0;
    for (int i = 1; i <= n; i++)
    {
        sum += i;
    }
    return sum;
}

int main(void)
{
    printf("add(2, 3) = %d\n", add(2, 3));
    printf("square(7) = %d\n", square(7));
    printf("max(4, 9) = %d\n", max(4, 9));
    printf("sum_to(10) = %d\n", sum_to(10));
    return 0;
}
