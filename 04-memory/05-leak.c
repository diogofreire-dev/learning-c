#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// BUG ON PURPOSE – check it with: make leaks/04-memory/05-leak
//
// Each loop iteration overwrites the only pointer to the previous block. Nothing
// points to it anymore, so it can never be freed: that is a memory leak.

int main(void)
{
    char *message = NULL;

    for (int i = 0; i < 5; i++)
    {
        message = malloc(32);
        if (message == NULL)
        {
            return 1;
        }
        snprintf(message, 32, "Message number %d", i);
        printf("%s\n", message);
        // Missing: free(message);
    }

    return 0;
}
