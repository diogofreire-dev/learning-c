#include <stdio.h>

int main(void)
{
    int number;
    int count = 0;
    int sum = 0;

    printf("Enter integers (0 to stop):\n");

    // Stop on 0, and also if scanf fails (e.g. the user types a letter)
    while (scanf("%d", &number) == 1 && number != 0)
    {
        sum += number;
        count++;
    }

    if (count == 0)
    {
        printf("No numbers entered\n");
        return 1;
    }

    // sum / count would be an integer division (7 / 2 = 3, the .5 is thrown away).
    // Casting one side to double makes the whole division use doubles.
    double average = (double) sum / count;

    printf("Count: %d\n", count);
    printf("Sum: %d\n", sum);
    printf("Average: %.2f\n", average);

    return 0;
}
