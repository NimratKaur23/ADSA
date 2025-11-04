#include <stdio.h>
#include <string.h>

#define MAX 100

// Function to find length of LCS
int LCSLength(char *X, char *Y, int m, int n, int L[m+1][n+1]) {
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0 || j == 0)
                L[i][j] = 0;
            else if (X[i - 1] == Y[j - 1])
                L[i][j] = L[i - 1][j - 1] + 1;
            else
                L[i][j] = (L[i - 1][j] > L[i][j - 1]) ? L[i - 1][j] : L[i][j - 1];
        }
    }
    return L[m][n];
}

// Recursive function to print all LCS
void printAllLCS(char *X, char *Y, int m, int n, int L[m+1][n+1], char *lcs, int index) {
    if (m == 0 || n == 0) {
        lcs[index] = '\0';
        printf("%s\n", lcs);
        return;
    }

    if (X[m - 1] == Y[n - 1]) {
        lcs[index] = X[m - 1];
        printAllLCS(X, Y, m - 1, n - 1, L, lcs, index + 1);
    } else {
        if (L[m - 1][n] >= L[m][n - 1])
            printAllLCS(X, Y, m - 1, n, L, lcs, index);
        if (L[m][n - 1] >= L[m - 1][n])
            printAllLCS(X, Y, m, n - 1, L, lcs, index);
    }
}

void reverseString(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

int main() {
    char X[MAX], Y[MAX];

    printf("Enter first string: ");
    scanf("%s", X);
    printf("Enter second string: ");
    scanf("%s", Y);

    int m = strlen(X);
    int n = strlen(Y);

    int L[m+1][n+1];

    int length = LCSLength(X, Y, m, n, L);
    printf("\nLength of Longest Common Subsequence: %d\n", length);

    char lcs[MAX];
    printf("\nAll Longest Common Subsequences:\n");
    printAllLCS(X, Y, m, n, L, lcs, 0);

    return 0;
}
