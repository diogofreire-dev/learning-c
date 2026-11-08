#include <stdio.h>

// BUG ON PURPOSE: the average is wrong. Find it with lldb, not by reading the code –
// see the lldb guide in exercises.md. (The solution is in notes.md.)

double average(const int grades[], int count)
{
    int sum = 0;
    for (int i = 1; i < count; i++)
    {
        sum += grades[i];
    }
    return (double) sum / count;
}

int main(void)
{
    int grades[] = {18, 12, 15, 9, 16};
    int count = sizeof(grades) / sizeof(grades[0]);

    printf("Average: %.2f (expected 14.00)\n", average(grades, count));
    return 0;
}
