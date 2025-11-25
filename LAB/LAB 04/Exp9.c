#include <stdio.h>
#include <string.h>
#include <math.h>

#define d 256  // Number of characters in input alphabet
#define q 101  // A prime number for Rabin-Karp

// ================= KMP Algorithm =================
void computeLPSArray(char* pat, int M, int* lps) {
    int len = 0;
    lps[0] = 0;
    int i = 1;
    while (i < M) {
        if (pat[i] == pat[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0)
                len = lps[len - 1];
            else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

void KMPSearch(char* pat, char* txt) {
    int M = strlen(pat);
    int N = strlen(txt);
    int lps[M];
    computeLPSArray(pat, M, lps);

    int i = 0, j = 0;
    printf("\nKMP Algorithm Matches at positions: ");
    while (i < N) {
        if (pat[j] == txt[i]) {
            i++;
            j++;
        }
        if (j == M) {
            printf("%d ", i - j);
            j = lps[j - 1];
        } else if (i < N && pat[j] != txt[i]) {
            if (j != 0)
                j = lps[j - 1];
            else
                i++;
        }
    }
}

// ================= Rabin-Karp Algorithm =================
void RabinKarpSearch(char* pat, char* txt) {
    int M = strlen(pat);
    int N = strlen(txt);
    int i, j;
    int p = 0; // hash value for pattern
    int t = 0; // hash value for txt
    int h = 1;

    for (i = 0; i < M - 1; i++)
        h = (h * d) % q;

    for (i = 0; i < M; i++) {
        p = (d * p + pat[i]) % q;
        t = (d * t + txt[i]) % q;
    }

    printf("\nRabin-Karp Algorithm Matches at positions: ");
    for (i = 0; i <= N - M; i++) {
        if (p == t) {
            for (j = 0; j < M; j++) {
                if (txt[i + j] != pat[j])
                    break;
            }
            if (j == M)
                printf("%d ", i);
        }
        if (i < N - M) {
            t = (d * (t - txt[i] * h) + txt[i + M]) % q;
            if (t < 0)
                t = (t + q);
        }
    }
}

int main() {
    char txt[1000], pat[1000];

    printf("Enter text: ");
    scanf("%s", txt);
    printf("Enter pattern to search: ");
    scanf("%s", pat);

    KMPSearch(pat, txt);
    RabinKarpSearch(pat, txt);

    printf("\n");
    return 0;
}