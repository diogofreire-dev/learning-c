#include <stdio.h>

// Global: visible in the whole file and lives for the whole program
int global = 100;

int next_ticket(void)
{
    // static local: only visible inside this function, but it is created once and
    // keeps its value between calls (a normal local is recreated every call)
    static int ticket = 0;
    ticket++;
    return ticket;
}

void depth(int level)
{
    // Every call gets its own copy of local, in its own stack frame
    int local = level;
    printf("  depth %d: local is at %p\n", level, (void *) &local);

    if (level < 3)
    {
        depth(level + 1);
    }
}

int main(void)
{
    int x = 1;
    printf("Global: %d\n", global);
    printf("x in main: %d\n", x);

    {
        // Block scope: this x only exists inside these braces and hides (shadows) the outer x
        int x = 2;
        printf("x inside the block: %d\n", x);
    }
    printf("x after the block: %d (the outer x was never touched)\n", x);

    // One call per statement: inside a single printf the order in which C evaluates
    // the arguments is not guaranteed, so three calls there could print 3 2 1
    printf("\nTickets:");
    for (int i = 0; i < 3; i++)
    {
        printf(" %d", next_ticket());
    }
    printf("\n");

    // Each deeper call is placed further down in memory: the stack grows towards lower addresses
    printf("\nAddresses of local variables in recursive calls:\n");
    depth(0);

    return 0;
}
