#include <stdio.h>
#include <stdlib.h>

#define MAX 100

void swapRows(int mat[MAX][MAX], int row1, int row2, int n) {
    for (int i = 0; i < n; i++) {
        int temp = mat[row1][i];
        mat[row1][i] = mat[row2][i];
        mat[row2][i] = temp;
    }
}

void swapRowVec(int vec[MAX][MAX], int row1, int row2, int n) {
    for (int i = 0; i < n; i++) {
        int temp = vec[row1][i];
        vec[row1][i] = vec[row2][i];
        vec[row2][i] = temp;
    }
}

void luDecomposition(int A[MAX][MAX], int n) {
    int L[MAX][MAX] = {0}, U[MAX][MAX] = {0}, P[MAX][MAX] = {0};

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            P[i][j] = (i == j) ? 1 : 0;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            U[i][j] = A[i][j];

    for (int k = 0; k < n; k++) {
        
        int maxRow = k;
        for (int i = k + 1; i < n; i++) {
            if (abs(U[i][k]) > abs(U[maxRow][k]))
                maxRow = i;
        }

        if (maxRow != k) {
            swapRows(U, k, maxRow, n);
            swapRowVec(P, k, maxRow, n);
            swapRows(L, k, maxRow, n);
        }

        for (int i = k + 1; i < n; i++) {
            L[i][k] = U[i][k] / U[k][k];
            for (int j = k; j < n; j++) {
                U[i][j] -= L[i][k] * U[k][j];
            }
        }
    }

    for (int i = 0; i < n; i++)
        L[i][i] = 1;

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
}

int main() {
    int n;
    int A[MAX][MAX];

    printf("Enter the size of the matrix (n x n): ");
    scanf("%d", &n);

    printf("Enter elements of the matrix (%d x %d):\n", n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &A[i][j]);

    luDecomposition(A, n);

    return 0;
}

