#include <stdio.h>
#include <stdlib.h>

#define MAX 10   // maximum number of variables/constraints

// Function to print the current simplex table
void print_table(double table[MAX][MAX], int rows, int cols, int iteration) {
    printf("\n--- Simplex Table: Iteration %d ---\n", iteration);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            printf("%8.2lf ", table[i][j]);
        printf("\n");
    }
}

// Function to perform the Simplex Method
void simplex(double table[MAX][MAX], int rows, int cols) {
    int iteration = 0;

    while (1) {
        iteration++;
        print_table(table, rows, cols, iteration);

        // Step 1: Find the entering variable (most negative in last row)
        int pivot_col = 0;
        double min = table[rows - 1][0];
        for (int j = 1; j < cols - 1; j++) {
            if (table[rows - 1][j] < min) {
                min = table[rows - 1][j];
                pivot_col = j;
            }
        }

        if (min >= 0) { // No negative values → optimal solution reached
            printf("\nOptimal solution reached!\n");
            break;
        }

        // Step 2: Find the leaving variable using minimum ratio test
        int pivot_row = -1;
        double min_ratio = 1e9; // large number
        for (int i = 0; i < rows - 1; i++) {
            double element = table[i][pivot_col];
            if (element > 0) {
                double ratio = table[i][cols - 1] / element;
                if (ratio < min_ratio) {
                    min_ratio = ratio;
                    pivot_row = i;
                }
            }
        }

        if (pivot_row == -1) {
            printf("Unbounded solution!\n");
            return;
        }

        double pivot = table[pivot_row][pivot_col];
        printf("\nPivot Element = %.2lf (Row %d, Col %d)\n", pivot, pivot_row + 1, pivot_col + 1);

        // Step 3: Divide pivot row by pivot element
        for (int j = 0; j < cols; j++)
            table[pivot_row][j] /= pivot;

        // Step 4: Make other entries in pivot column = 0
        for (int i = 0; i < rows; i++) {
            if (i != pivot_row) {
                double factor = table[i][pivot_col];
                for (int j = 0; j < cols; j++)
                    table[i][j] -= factor * table[pivot_row][j];
            }
        }
    }

    // Step 5: Display results
    printf("\nFinal Simplex Table:\n");
    print_table(table, rows, cols, iteration);

    printf("\nMaximum value of Z = %.2lf\n", table[rows - 1][cols - 1]);
}

int main() {
    int m, n; // m = constraints, n = variables
    double table[MAX][MAX] = {0};

    printf("Enter number of variables: ");
    scanf("%d", &n);
    printf("Enter number of constraints: ");
    scanf("%d", &m);

    printf("\nEnter coefficients of each constraint (A matrix):\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("a[%d][%d]: ", i + 1, j + 1);
            scanf("%lf", &table[i][j]);
        }
    }

    printf("\nEnter RHS values (b):\n");
    for (int i = 0; i < m; i++) {
        printf("b[%d]: ", i + 1);
        scanf("%lf", &table[i][n + i]); // slack variable column
        table[i][n + i] = 1; // identity matrix for slack vars
    }

    // Adjust columns (slack + RHS)
    for (int i = 0; i < m; i++) {
        printf("RHS[%d]: ", i + 1);
        scanf("%lf", &table[i][n + m]);
    }

    printf("\nEnter coefficients of objective function (Z = c1*x1 + c2*x2 + ...):\n");
    for (int j = 0; j < n; j++) {
        printf("c[%d]: ", j + 1);
        scanf("%lf", &table[m][j]);
        table[m][j] *= -1; // For max Z, convert to Z - c1x1 - c2x2 = 0
    }

    // Set up the total columns
    int total_cols = n + m + 1;
    int total_rows = m + 1;

    simplex(table, total_rows, total_cols);

    return 0;
}