#include <stdio.h>
#include <stdlib.h>

// Each node holds a value and the address of the next node. The last node's next is NULL.
//
//   head -> [3 | *] -> [1 | *] -> [4 | NULL]
typedef struct node
{
    int value;
    struct node *next;   // the typedef name does not exist yet here, so use struct node
} Node;

// The functions that can change the first node receive Node **head: the address of the
// caller's head pointer, so they can make it point somewhere else (like swap in topic 03)

// Returns 0 on success, -1 if out of memory
int push_front(Node **head, int value)
{
    Node *node = malloc(sizeof(*node));
    if (node == NULL)
    {
        return -1;
    }
    node->value = value;
    node->next = *head;   // the new node points to the old first node
    *head = node;         // and becomes the first node
    return 0;
}

int push_back(Node **head, int value)
{
    Node *node = malloc(sizeof(*node));
    if (node == NULL)
    {
        return -1;
    }
    node->value = value;
    node->next = NULL;

    if (*head == NULL)
    {
        *head = node;
        return 0;
    }

    // Walk to the last node: O(n), there is no shortcut to the end
    Node *current = *head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = node;
    return 0;
}

void print_list(const Node *head)
{
    for (const Node *n = head; n != NULL; n = n->next)
    {
        printf("[%d] -> ", n->value);
    }
    printf("NULL\n");
}

// Returns the first node with this value, or NULL
Node *find(Node *head, int value)
{
    for (Node *n = head; n != NULL; n = n->next)
    {
        if (n->value == value)
        {
            return n;
        }
    }
    return NULL;
}

// Removes the first node with this value. Returns 1 if one was removed, 0 if not found.
int remove_value(Node **head, int value)
{
    // link points to the pointer that points to the current node (head, or a previous
    // node's next): unlinking is then the same for the first node and for any other
    Node **link = head;
    while (*link != NULL)
    {
        if ((*link)->value == value)
        {
            Node *to_free = *link;
            *link = to_free->next;   // skip over the node
            free(to_free);
            return 1;
        }
        link = &(*link)->next;
    }
    return 0;
}

void free_list(Node **head)
{
    Node *current = *head;
    while (current != NULL)
    {
        // Save next BEFORE freeing: after free(current), current->next is a use-after-free
        Node *next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}

int main(void)
{
    Node *list = NULL;   // an empty list is just a NULL pointer

    if (push_back(&list, 1) != 0 || push_back(&list, 4) != 0 ||
        push_front(&list, 3) != 0 || push_back(&list, 5) != 0)
    {
        free_list(&list);
        return 1;
    }
    printf("List:            ");
    print_list(list);

    Node *found = find(list, 4);
    printf("find(4):         %s\n", found != NULL ? "found" : "not found");
    printf("find(9):         %s\n", find(list, 9) != NULL ? "found" : "not found");

    remove_value(&list, 3);
    printf("remove 3 (first): ");
    print_list(list);

    remove_value(&list, 5);
    printf("remove 5 (last):  ");
    print_list(list);

    free_list(&list);
    printf("After free_list: ");
    print_list(list);

    return 0;
}
