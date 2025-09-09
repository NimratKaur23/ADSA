#include <stdio.h>

#define MAX 100

int LUPDecomposition(int A[MAX][MAX], int n, int L[MAX][MAX], int U[MAX][MAX], int P[MAX][MAX]) {
    // Initialize L, U, P
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            L[i][j] = 0;
            U[i][j] = 0;
            //Identity matrix
            P[i][j] = (i == j) ? 1 : 0; 
        }
    }

    // LU Decomposition
    for (int i = 0; i < n; i++) {
        // Upper triangular
        for (int k = i; k < n; k++) {
            int sum = 0;
            for (int j = 0; j < i; j++)
                sum += L[i][j] * U[j][k];
            U[i][k] = A[i][k] - sum;
        }

        // Lower triangular
        for (int k = i; k < n; k++) {
            if (i == k)
                L[i][i] = 1;
            else {
                int sum = 0;
                for (int j = 0; j < i; j++)
                    sum += L[k][j] * U[j][i];

                // check if all elements are zero
                if (U[i][i] == 0) {
                    printf("\nError: Zero pivot at row %d. Cannot continue without pivoting.\n", i);
                    return 0;
                }

                L[k][i] = (A[k][i] - sum) / U[i][i];
            }
        }
    }
    return 1; 
}

// Forward substitution
void forwardSubstitution(int L[MAX][MAX], int b[MAX], double y[MAX], int n) {
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < i; j++)
            sum += L[i][j] * y[j];
        y[i] = (b[i] - sum); 
    }
}

// Backward substitution
void backSubstitution(int U[MAX][MAX], double y[MAX], double x[MAX], int n) {
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0;
        for (int j = i + 1; j < n; j++)
            sum += U[i][j] * x[j];
        x[i] = (y[i] - sum) / U[i][i];
    }
}

int main() {
    int n;
    int A[MAX][MAX], b[MAX];
    int L[MAX][MAX], U[MAX][MAX], P[MAX][MAX];
    double y[MAX], x[MAX];

    printf("Enter size of the matrix (n x n): ");
    scanf("%d", &n);

    printf("Enter elements of matrix A (%d x %d):\n", n, n);
    for (int i = 0; i < n; i++) {
        int rowSum = 0; 
        for (int j = 0; j < n; j++) {
            scanf("%d", &A[i][j]);
            rowSum += (A[i][j] != 0); 
        }
        // Detect zero row
        if (rowSum == 0) {
            printf("\nError: Row %d of matrix is all zeros. System may be singular. Exiting.\n", i);
            return 1;
        }
    }

    printf("Enter elements of vector b (%d):\n", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &b[i]);

        if (!LUPDecomposition(A, n, L, U, P)) {
        printf("Decomposition failed.\n");
        return 1; 
    }

    forwardSubstitution(L, b, y, n);
    backSubstitution(U, y, x, n);

    printf("\nPermutation Matrix (P):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%4d ", P[i][j]);
        printf("\n");
    }

    printf("\nLower Triangular Matrix (L):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%6d ", L[i][j]);
        printf("\n");
    }

    printf("\nUpper Triangular Matrix (U):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%6d ", U[i][j]);
        printf("\n");
    }

    printf("\nSolution vector x:\n");
    for (int i = 0; i < n; i++)
        printf("x[%d] = %.6f\n", i, x[i]);

    return 0;
}
