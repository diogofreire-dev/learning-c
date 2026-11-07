#include <stdio.h>
#include <string.h>

// Fixed version of 06-hello-name.c (kept unchanged on purpose, to study the overflow later).
//
// Fix 1: scanf("%99s", name) limits the read to 99 characters + '\0'.
//        But %s stops at the first space, so "Diogo Freire" becomes "Diogo".
// Fix 2 (used here): fgets reads a whole line, spaces included, and never writes
//        more than sizeof(name) bytes, '\0' included.

int main(void)
{
    char name[100];

    printf("What is your name? ");
    if (fgets(name, sizeof(name), stdin) == NULL)
    {
        return 1;
    }

    // fgets keeps the '\n' from pressing Enter; replace it with '\0' to remove it
    name[strcspn(name, "\n")] = '\0';

    printf("hello, %s\n", name);
    return 0;
}
