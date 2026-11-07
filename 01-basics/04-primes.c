#include <stdio.h>

// Returns 1 if n is prime, 0 otherwise
int is_prime(int n)
{
    if (n < 2)
    {
        return 0;
    }

    // Only need to test divisors up to the square root of n:
    // if n = d * e and d > sqrt(n), then e < sqrt(n) was already tested.
    // d <= n / d means d * d <= n, but d * d could overflow for n close to INT_MAX.
    for (int d = 2; d <= n / d; d++)
    {
        if (n % d == 0)
        {
            return 0;
        }
    }
    return 1;
}

void print_primes_up_to(int max)
{
    for (int n = 2; n <= max; n++)
    {
        if (is_prime(n))
        {
            printf("%d ", n);
        }
    }
    printf("\n");
}

int main(void)
{
    int max;

    printf("Print primes up to: ");
    if (scanf("%d", &max) != 1)
    {
        printf("Invalid input\n");
        return 1;
    }

    print_primes_up_to(max);
    return 0;
}
