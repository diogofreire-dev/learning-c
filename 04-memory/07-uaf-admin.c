#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// BUG ON PURPOSE. Run it twice:
//   make run/04-memory/07-uaf-admin
//   ASAN_OPTIONS=quarantine_size_mb=0 ./build/04-memory/07-uaf-admin
//
// AddressSanitizer keeps freed blocks in a "quarantine" so they are not reused right away,
// which lets it catch the bug. With the quarantine off (or without ASan, like a normal
// program), the allocator hands the freed block to the next malloc of the same size.

struct user
{
    char name[16];
    int is_admin;
};

int main(void)
{
    struct user *user = malloc(sizeof(*user));
    if (user == NULL)
    {
        return 1;
    }
    strcpy(user->name, "diogo");
    user->is_admin = 0;
    printf("User created at %p, is_admin = %d\n", (void *) user, user->is_admin);

    // The user logs out: the block is freed, but the pointer is not set to NULL
    free(user);

    // The "attacker" sends a message of exactly the same size, full of 'A' (0x41)
    char *message = malloc(sizeof(struct user));
    if (message == NULL)
    {
        return 1;
    }
    memset(message, 'A', sizeof(struct user));
    printf("Message stored at %p\n", (void *) message);

    // The program still trusts the old pointer
    printf("is_admin through the old pointer = %d (0x%08x)\n", user->is_admin, user->is_admin);
    if (user->is_admin)
    {
        printf("Welcome, administrator!\n");
    }

    free(message);
    return 0;
}
