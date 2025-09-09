#include <stdio.h>
#include <stdlib.h>


int** MatrixAllocation(int r, int c) {
    int** mat = (int**)malloc(r * sizeof(int*));
    for (int i = 0; i < r; i++)
        mat[i] = (int*)calloc(c, sizeof(int));
    return mat;
}


void freeMatrix(int** mat, int r) {
    for (int i = 0; i < r; i++)
        free(mat[i]);
    free(mat);
}


void MatrixAddition(int** A, int** B, int** C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
}


void MatrixSubtraction(int** A, int** B, int** C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
}


void strassen(int** A, int** B, int** C, int n) {
    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    int k = n / 2;

    // Submatrices
    int** A11 = MatrixAllocation(k, k);
    int** A12 = MatrixAllocation(k, k);
    int** A21 = MatrixAllocation(k, k);
    int** A22 = MatrixAllocation(k, k);

    int** B11 = MatrixAllocation(k, k);
    int** B12 = MatrixAllocation(k, k);
    int** B21 = MatrixAllocation(k, k);
    int** B22 = MatrixAllocation(k, k);

    // Splitting
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + k];
            A21[i][j] = A[i + k][j];
            A22[i][j] = A[i + k][j + k];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + k];
            B21[i][j] = B[i + k][j];
            B22[i][j] = B[i + k][j + k];
        }
    }

    // Temporary matrices
    int** M1 = MatrixAllocation(k, k);
    int** M2 = MatrixAllocation(k, k);
    int** M3 = MatrixAllocation(k, k);
    int** M4 = MatrixAllocation(k, k);
    int** M5 = MatrixAllocation(k, k);
    int** M6 = MatrixAllocation(k, k);
    int** M7 = MatrixAllocation(k, k);
    int** T1 = MatrixAllocation(k, k);
    int** T2 = MatrixAllocation(k, k);

    // M1 = (A11 + A22)(B11 + B22)
    MatrixAddition(A11, A22, T1, k);
    MatrixAddition(B11, B22, T2, k);
    strassen(T1, T2, M1, k);

    // M2 = (A21 + A22)B11
    MatrixAddition(A21, A22, T1, k);
    strassen(T1, B11, M2, k);

    // M3 = A11(B12 - B22)
    MatrixSubtraction(B12, B22, T2, k);
    strassen(A11, T2, M3, k);

    // M4 = A22(B21 - B11)
    MatrixSubtraction(B21, B11, T2, k);
    strassen(A22, T2, M4, k);

    // M5 = (A11 + A12)B22
    MatrixAddition(A11, A12, T1, k);
    strassen(T1, B22, M5, k);

    // M6 = (A21 - A11)(B11 + B12)
    MatrixSubtraction(A21, A11, T1, k);
    MatrixAddition(B11, B12, T2, k);
    strassen(T1, T2, M6, k);

    // M7 = (A12 - A22)(B21 + B22)
    MatrixSubtraction(A12, A22, T1, k);
    MatrixAddition(B21, B22, T2, k);
    strassen(T1, T2, M7, k);

    
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            C[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
            C[i][j + k] = M3[i][j] + M5[i][j];
            C[i + k][j] = M2[i][j] + M4[i][j];
            C[i + k][j + k] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
        }
    }

    // free the matrices
    freeMatrix(A11, k); freeMatrix(A12, k); freeMatrix(A21, k); freeMatrix(A22, k);
    freeMatrix(B11, k); freeMatrix(B12, k); freeMatrix(B21, k); freeMatrix(B22, k);
    freeMatrix(M1, k); freeMatrix(M2, k); freeMatrix(M3, k); freeMatrix(M4, k);
    freeMatrix(M5, k); freeMatrix(M6, k); freeMatrix(M7, k);
    freeMatrix(T1, k); freeMatrix(T2, k);
}

int nextPowerOf2(int n) {
    int p = 1;
    while (p < n) p <<= 1;
    return p;
}

void printMatrix(int** mat, int r, int c) {
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++)
            printf("%d ", mat[i][j]);
        printf("\n");
    }
}

int main() {
    int r1, c1, r2, c2;
    printf("Enter rows and columns of Matrix A: ");
    scanf("%d %d", &r1, &c1);
    printf("Enter rows and columns of Matrix B: ");
    scanf("%d %d", &r2, &c2);

    if (c1 != r2) {
        printf("Matrix multiplication not possible! (c1 != r2)\n");
        return 0;
    }

    int** A = MatrixAllocation(r1, c1);
    int** B = MatrixAllocation(r2, c2);

    printf("Enter elements of Matrix A:\n");
    for (int i = 0; i < r1; i++)
        for (int j = 0; j < c1; j++)
            scanf("%d", &A[i][j]);

    printf("Enter elements of Matrix B:\n");
    for (int i = 0; i < r2; i++)
        for (int j = 0; j < c2; j++)
            scanf("%d", &B[i][j]);

    int n = nextPowerOf2(r1 > c1 ? (r1 > c2 ? r1 : c2) : (c1 > c2 ? c1 : c2));

    int** Apad = MatrixAllocation(n, n);
    int** Bpad = MatrixAllocation(n, n);
    int** Cpad = MatrixAllocation(n, n);

    for (int i = 0; i < r1; i++)
        for (int j = 0; j < c1; j++)
            Apad[i][j] = A[i][j];

    for (int i = 0; i < r2; i++)
        for (int j = 0; j < c2; j++)
            Bpad[i][j] = B[i][j];

    strassen(Apad, Bpad, Cpad, n);

    printf("Resultant Matrix C:\n");
    printMatrix(Cpad, r1, c2);

    //FREE MEMORY
    freeMatrix(A, r1);
    freeMatrix(B, r2);
    freeMatrix(Apad, n);
    freeMatrix(Bpad, n);
    freeMatrix(Cpad, n);

    return 0;
}

