#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define NUM_FUNCS 12


typedef double (*func_ptr)(int);


double f1(int n) { return 1; }
double f2(int n) { return log2(n); }
double f3(int n) { return 12 * sqrt(n); }
double f4(int n) { return 50 * sqrt(n); }
double f5(int n) { return pow(n, 0.51); }
double f6(int n) { return n; }
double f7(int n) { return 3 * n; }
double f8(int n) { return 232 * n; }
double f9(int n) { return n * log2(n); }
double f10(int n) { return 2 * pow(n, 3); }
double f11(int n) { return 100 * pow(n, 2) + 6 * n; }
double f12(int n) { return pow(n, 2) - 324; }


typedef struct {
    char name[20];
    double value;
} FuncVal;


int compare(const void *a, const void *b) {
    FuncVal *fa = (FuncVal *)a;
    FuncVal *fb = (FuncVal *)b;
    if (fa->value < fb->value) return -1;
    if (fa->value > fb->value) return 1;
    return 0;
}

int main() {
    int n;
    printf("Enter n: ");
    scanf("%d", &n);

    func_ptr funcs[NUM_FUNCS] = {
        f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12
    };

    char *names[NUM_FUNCS] = {
        "1", "log2(n)", "12√n", "50√n", "n^0.51", "n", "3n",
        "232n", "n log2n", "2n^3", "100n^2+6n", "n^2-324"
    };

    FuncVal arr[NUM_FUNCS];

    
    for (int i = 0; i < NUM_FUNCS; i++) {
        arr[i].value = funcs[i](n);
        strcpy(arr[i].name, names[i]);
    }

    
    qsort(arr, NUM_FUNCS, sizeof(FuncVal), compare);

    
    printf("\nFunctions in increasing order for n = %d:\n", n);
    for (int i = 0; i < NUM_FUNCS; i++) {
        printf("%-12s = %.2f\n", arr[i].name, arr[i].value);
    }

    return 0;
}