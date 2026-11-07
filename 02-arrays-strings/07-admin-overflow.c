#include <stdio.h>

// BUG ON PURPOSE – do not fix it here; the fix is in notes.md.
//
// A struct groups variables together (topic 05). Its members are stored one after
// another in memory, so is_admin sits right after the 8 bytes of name:
//
//   | n | a | m | e | . | . | . | . | is_admin (4 bytes) |
//     0   1   2   3   4   5   6   7   8   9   10  11
struct user
{
    char name[8];
    int is_admin;
};

int main(void)
{
    struct user u;
    u.is_admin = 0;

    printf("Name: ");
    // %s has no limit: a name longer than 7 characters spills into is_admin.
    // AddressSanitizer does not notice, because the write stays inside the struct u.
    if (scanf("%s", u.name) != 1)
    {
        return 1;
    }

    printf("Hello, %s\n", u.name);
    printf("is_admin = %d (0x%08x)\n", u.is_admin, u.is_admin);

    if (u.is_admin)
    {
        printf("Welcome, administrator!\n");
    }
    else
    {
        printf("Normal user.\n");
    }

    return 0;
}
