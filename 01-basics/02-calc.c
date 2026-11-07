#include <stdio.h>

int main(void)
{
    int a, b;
    char op;

    printf("Enter a calculation (e.g. 7 + 2): ");

    // scanf returns how many variables it filled; anything other than 3 means bad input
    if (scanf("%d %c %d", &a, &op, &b) != 3)
    {
        printf("Invalid input\n");
        return 1;
    }

    switch (op)
    {
        case '+':
            printf("= %d\n", a + b);
            break;
        case '-':
            printf("= %d\n", a - b);
            break;
        case '*':
            printf("= %d\n", a * b);
            break;
        case '/':
        case '%':
            // Integer division by zero crashes the program, so check first.
            // '/' and '%' share this check: with no break after case '/', it falls through.
            if (b == 0)
            {
                printf("Error: division by zero\n");
                return 1;
            }
            printf("= %d\n", op == '/' ? a / b : a % b);
            break;
        default:
            printf("Unknown operator: %c\n", op);
            return 1;
    }

    return 0;
}
