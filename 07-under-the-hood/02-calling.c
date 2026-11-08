#include <stdio.h>

// Look at the call in main:
//   make asm/07-under-the-hood/02-calling OPT=-O0
//
// ARM64 passes the first 8 arguments in registers w0-w7 (x0-x7). There are no more
// argument registers, so the 9th and 10th go on the stack.

int sum10(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j)
{
    return a + b + c + d + e + f + g + h + i + j;
}

int main(void)
{
    int total = sum10(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    printf("sum10 = %d\n", total);
    return 0;
}
