#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Usage:
//   contacts FILE add NAME PHONE
//   contacts FILE list
//   contacts FILE find NAME
//
// The file has one contact per line: name,phone

#define NAME_SIZE 32
#define PHONE_SIZE 16

typedef struct
{
    char name[NAME_SIZE];
    char phone[PHONE_SIZE];
} Contact;

// A dynamic array of contacts, like topic 04 exercise 3
typedef struct
{
    Contact *items;
    size_t count;
    size_t capacity;
} ContactList;

// Returns 0 on success, -1 if out of memory
int list_add(ContactList *list, const Contact *c)
{
    if (list->count == list->capacity)
    {
        size_t new_capacity = list->capacity == 0 ? 4 : list->capacity * 2;
        Contact *bigger = realloc(list->items, new_capacity * sizeof(*bigger));
        if (bigger == NULL)
        {
            return -1;
        }
        list->items = bigger;
        list->capacity = new_capacity;
    }
    list->items[list->count] = *c;   // structs can be copied with =
    list->count++;
    return 0;
}

// A value is valid if it is not empty, fits in the field ('\0' included) and has no
// ',' or newline, which would break the file format
int is_valid(const char *text, size_t field_size)
{
    size_t len = strlen(text);
    return len > 0 && len < field_size && strpbrk(text, ",\n") == NULL;
}

// Loads every contact from the file. A missing file just means an empty list.
// Returns 0 on success, -1 on error.
int load(const char *path, ContactList *list)
{
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        return 0;
    }

    char line[NAME_SIZE + PHONE_SIZE + 8];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';

        // Split "name,phone" at the comma
        char *comma = strchr(line, ',');
        if (comma == NULL)
        {
            continue;   // skip broken lines
        }
        *comma = '\0';
        const char *name = line;
        const char *phone = comma + 1;

        // Never trust the file either: it may have been edited by hand
        if (!is_valid(name, NAME_SIZE) || !is_valid(phone, PHONE_SIZE))
        {
            continue;
        }

        Contact c;
        snprintf(c.name, sizeof(c.name), "%s", name);
        snprintf(c.phone, sizeof(c.phone), "%s", phone);
        if (list_add(list, &c) != 0)
        {
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s FILE add NAME PHONE | list | find NAME\n", argv[0]);
        return 1;
    }

    const char *path = argv[1];
    const char *command = argv[2];

    if (strcmp(command, "add") == 0 && argc == 5)
    {
        if (!is_valid(argv[3], NAME_SIZE) || !is_valid(argv[4], PHONE_SIZE))
        {
            fprintf(stderr, "Invalid name or phone (no commas, max %d and %d characters)\n",
                    NAME_SIZE - 1, PHONE_SIZE - 1);
            return 1;
        }

        // "a" = append: write at the end of the file, creating it if needed
        FILE *file = fopen(path, "a");
        if (file == NULL)
        {
            perror(path);
            return 1;
        }
        fprintf(file, "%s,%s\n", argv[3], argv[4]);
        if (fclose(file) != 0)
        {
            perror(path);
            return 1;
        }
        printf("Added %s\n", argv[3]);
        return 0;
    }

    ContactList list = {NULL, 0, 0};
    if (load(path, &list) != 0)
    {
        fprintf(stderr, "Out of memory\n");
        free(list.items);
        return 1;
    }

    int status = 0;
    if (strcmp(command, "list") == 0 && argc == 3)
    {
        printf("%zu contact(s):\n", list.count);
        for (size_t i = 0; i < list.count; i++)
        {
            printf("  %-20s %s\n", list.items[i].name, list.items[i].phone);
        }
    }
    else if (strcmp(command, "find") == 0 && argc == 4)
    {
        int found = 0;
        for (size_t i = 0; i < list.count; i++)
        {
            // strcmp returns 0 when the two strings are equal
            if (strcmp(list.items[i].name, argv[3]) == 0)
            {
                printf("%s: %s\n", list.items[i].name, list.items[i].phone);
                found = 1;
            }
        }
        if (!found)
        {
            printf("%s not found\n", argv[3]);
            status = 1;
        }
    }
    else
    {
        fprintf(stderr, "Unknown command or wrong number of arguments\n");
        status = 1;
    }

    free(list.items);
    return status;
}
