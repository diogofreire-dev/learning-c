#include <stdio.h>

int main(void)
{
    char name[100];
    printf("What is your name? ");
    scanf("%s", name);
    printf("hello, %s\n", name);
}