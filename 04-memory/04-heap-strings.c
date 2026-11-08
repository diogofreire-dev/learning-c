#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Returns a copy of s on the heap, or NULL if there is no memory.
// The CALLER owns the result and must free it.
char *my_strdup(const char *s)
{
    size_t size = strlen(s) + 1;   // + 1 for the '\0'
    char *copy = malloc(size);
    if (copy == NULL)
    {
        return NULL;
    }
    memcpy(copy, s, size);         // copies the '\0' too
    return copy;
}

// Returns a new heap string with a followed by b, or NULL if there is no memory.
// The CALLER owns the result and must free it. a and b are not changed.
char *concat(const char *a, const char *b)
{
    size_t len_a = strlen(a);
    size_t len_b = strlen(b);
    char *result = malloc(len_a + len_b + 1);
    if (result == NULL)
    {
        return NULL;
    }
    memcpy(result, a, len_a);
    memcpy(result + len_a, b, len_b + 1);   // + 1 copies b's '\0'
    return result;
}

int main(void)
{
    // Unlike a local array (topic 03, exercise 7), heap memory stays valid after the
    // function that allocated it returns – until someone calls free
    char *name = my_strdup("Diogo");
    char *full = concat(name, " Freire");

    if (name == NULL || full == NULL)
    {
        free(name);   // free(NULL) is allowed and does nothing
        free(full);
        return 1;
    }

    name[0] = 'd';    // the copy can be changed; the original literal is untouched
    printf("name: %s\n", name);
    printf("full: %s\n", full);

    free(name);
    free(full);
    return 0;
}
