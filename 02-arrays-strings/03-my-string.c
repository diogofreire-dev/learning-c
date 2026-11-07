#include <stdio.h>
#include <string.h>

// Counts characters until the '\0' – this is exactly what strlen does
size_t my_strlen(const char s[])
{
    size_t len = 0;
    while (s[len] != '\0')
    {
        len++;
    }
    return len;
}

// Copies src into dest, writing at most dest_size bytes ('\0' included).
// If src does not fit, it is cut. Unlike strcpy, it can never overflow dest.
void my_strcpy(char dest[], const char src[], size_t dest_size)
{
    if (dest_size == 0)
    {
        return;
    }

    size_t i = 0;
    // Stop at the end of src OR when only 1 byte is left (reserved for the '\0')
    while (src[i] != '\0' && i < dest_size - 1)
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int main(void)
{
    char name[] = "Diogo";

    // <= instead of < so the loop also shows the '\0' at the end
    printf("Characters of \"%s\":\n", name);
    for (size_t i = 0; i <= my_strlen(name); i++)
    {
        if (name[i] == '\0')
        {
            printf("  name[%zu] = '\\0'  (%d)\n", i, name[i]);
        }
        else
        {
            printf("  name[%zu] = '%c'   (%d)\n", i, name[i], name[i]);
        }
    }

    printf("\nmy_strlen: %zu\n", my_strlen(name));
    printf("strlen:    %zu\n", strlen(name));
    printf("sizeof:    %zu (counts the '\\0')\n", sizeof(name));

    char small[6];
    my_strcpy(small, "Diogo Freire", sizeof(small));
    printf("\nCopy into 6 bytes: \"%s\" (cut to fit, no overflow)\n", small);

    char big[20];
    my_strcpy(big, "Diogo Freire", sizeof(big));
    printf("Copy into 20 bytes: \"%s\"\n", big);

    return 0;
}
