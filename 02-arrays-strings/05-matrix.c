#include <stdio.h>

#define N 3

void print_matrix(const int m[N][N])
{
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            printf("%3d", m[row][col]);
        }
        printf("\n");
    }
}

int main(void)
{
    // m[row][col]
    int m[N][N] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };

    printf("Matrix:\n");
    print_matrix(m);

    // The transpose swaps rows and columns: t[row][col] = m[col][row]
    int t[N][N];
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            t[row][col] = m[col][row];
        }
    }
    printf("\nTranspose:\n");
    print_matrix(t);

    int diagonal = 0;
    for (int i = 0; i < N; i++)
    {
        diagonal += m[i][i];
    }
    printf("\nDiagonal sum: %d\n", diagonal);

    // In memory a 2D array is just 9 ints in a row: row 0, then row 1, then row 2
    // ("row-major order"). Reading it as a flat block of ints shows the real order.
    const int *flat = &m[0][0];
    printf("\nIn memory: ");
    for (int i = 0; i < N * N; i++)
    {
        printf("%d ", flat[i]);
    }
    printf("\n");

    return 0;
}
