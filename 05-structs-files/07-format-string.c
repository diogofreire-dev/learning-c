#include <stdio.h>
#include <string.h>

// BUG ON PURPOSE – the compiler warning (-Wformat-security) is expected here.
// The fix is in notes.md.
//
// Try these names:
//   Diogo
//   %p %p %p %p %p %p %p %p
//   %n

int main(void)
{
    int secret = 0x1337;
    char name[100];

    printf("Name: ");
    if (fgets(name, sizeof(name), stdin) == NULL)
    {
        return 1;
    }
    name[strcspn(name, "\n")] = '\0';

    printf("Hello, ");
    // The user's text is used AS THE FORMAT: every %p in it makes printf read one more
    // "argument" that was never passed – it takes whatever is next in the registers and on the stack
    printf(name);
    printf("\n");

    printf("(the secret 0x%x lives at %p)\n", secret, (void *) &secret);
    return 0;
}
