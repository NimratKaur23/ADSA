#include <stdio.h>
#include <math.h>

#define MAXN 20
#define MAXM 20

#define MU_REDUCTION 0.5
#define TOL 1e-6
#define STEP 0.01
#define MAX_ITER 20000

void matvec(double y[], double A[][MAXN], double x[], int m, int n) {
    for (int i = 0; i < m; i++) {
        y[i] = 0;
        for (int j = 0; j < n; j++)
            y[i] += A[i][j] * x[j];
    }
}

double objective(double x[], double c[], double mu, int n) {
    double val = 0;
    for (int i = 0; i < n; i++)
        val += c[i] * x[i];

    for (int i = 0; i < n; i++)
        val -= mu * log(x[i]);

    return val;
}

void gradient(double grad[], double x[], double c[], double mu, int n) {
    for (int i = 0; i < n; i++)
        grad[i] = c[i] - mu * (1.0 / x[i]);
}

int main() {
    int n, m;
    double A[MAXM][MAXN], b[MAXM], c[MAXN];
    double x[MAXN];

    printf("Enter number of variables (n): ");
    scanf("%d", &n);

    printf("Enter number of equality constraints (m): ");
    scanf("%d", &m);

    printf("\nEnter matrix A (%d x %d):\n", m, n);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            scanf("%lf", &A[i][j]);

    printf("\nEnter vector b (%d elements):\n", m);
    for (int i = 0; i < m; i++)
        scanf("%lf", &b[i]);

    printf("\nEnter cost vector c (%d elements):\n", n);
    for (int i = 0; i < n; i++)
        scanf("%lf", &c[i]);

    // Initial interior point
    for (int i = 0; i < n; i++)
        x[i] = 1.0;

    double mu = 1.0;

    printf("\nRunning Interior Point Method...\n");

    for (int iter = 0; iter < 20; iter++) {

        for (int k = 0; k < MAX_ITER; k++) {
            double grad[MAXN];
            gradient(grad, x, c, mu, n);

            // gradient descent update
            for (int i = 0; i < n; i++) {
                x[i] -= STEP * grad[i];
                if (x[i] < 1e-8) x[i] = 1e-8;
            }

            double Ax[MAXM];
            matvec(Ax, A, x, m, n);

            // projection to satisfy Ax = b
            for (int i = 0; i < m; i++) {
                double diff = Ax[i] - b[i];
                for (int j = 0; j < n; j++)
                    x[j] -= 0.01 * diff * A[i][j];
            }
        }

        printf("Iter %2d  mu = %.5f   x = [", iter, mu);
        for (int i = 0; i < n; i++)
            printf(" %.4f ", x[i]);
        printf("]   f = %.5f\n", objective(x, c, mu, n));

        mu *= MU_REDUCTION;
    }

    printf("\nOptimal x ≈ [");
    for (int i = 0; i < n; i++)
        printf(" %.6f ", x[i]);
    printf("]\n");

    double obj = 0;
    for (int i = 0; i < n; i++)
        obj += c[i] * x[i];

    printf("Optimal objective ≈ %.6f\n", obj);

    return 0;
}