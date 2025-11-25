#include <stdio.h>
#include <stdbool.h>

#define MAX 100

bool bpm(bool bpGraph[MAX][MAX], int a, bool seen[], int matchB[], int N, int M) {
    for (int b = 0; b < M; b++) {
        if (bpGraph[a][b] && !seen[b]) {
            seen[b] = true;
            if (matchB[b] < 0 || bpm(bpGraph, matchB[b], seen, matchB, N, M)) {
                matchB[b] = a;
                return true;
            }
        }
    }
    return false;
}

int maxBPM(bool bpGraph[MAX][MAX], int N, int M, int matchB[]) {
    for (int i = 0; i < M; i++)
        matchB[i] = -1;

    int result = 0;
    for (int a = 0; a < N; a++) {
        bool seen[M];
        for (int i = 0; i < M; i++)
            seen[i] = false;

        if (bpm(bpGraph, a, seen, matchB, N, M))
            result++;
    }
    return result;
}

int main() {
    int N, M;
    printf("Enter number of vertices in set A: ");
    scanf("%d", &N);
    printf("Enter number of vertices in set B: ");
    scanf("%d", &M);

    bool bpGraph[MAX][MAX];
    printf("Enter adjacency matrix (1 if edge exists, 0 otherwise):\n");
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            scanf("%d", &bpGraph[i][j]);

    int matchB[MAX];
    int maxMatch = maxBPM(bpGraph, N, M, matchB);
    printf("\nMaximum number of matching in bipartite graph: %d\n", maxMatch);

    printf("Matched pairs (A -> B):\n");
    for (int b = 0; b < M; b++) {
        if (matchB[b] != -1) {
            printf("A%d -> B%d\n", matchB[b], b);
        }
    }

    return 0;
}
