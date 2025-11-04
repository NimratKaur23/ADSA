#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 50
#define INF 99999

// Structure for SIF format: NodeA  weight  NodeB
typedef struct {
    char nodeA[20];
    int weight;
    char nodeB[20];
} Edge;

int findIndex(char labels[MAX][20], int count, char node[]) {
    for (int i = 0; i < count; i++) {
        if (strcmp(labels[i], node) == 0)
            return i;
    }
    return -1;
}

void prims(int graph[MAX][MAX], int n) {
    int selected[MAX] = {0};
    selected[0] = 1;

    int totalCost = 0, edges = 0;

    printf("\nMinimum Spanning Tree Using Prim's Algorithm:\n");
    printf("Edge \t Weight\n");

    while (edges < n - 1) {
        int min = INF, x = 0, y = 0;

        for (int i = 0; i < n; i++) {
            if (selected[i]) {
                for (int j = 0; j < n; j++) {
                    if (!selected[j] && graph[i][j] && graph[i][j] < min) {
                        min = graph[i][j];
                        x = i;
                        y = j;
                    }
                }
            }
        }

        printf("%d — %d \t %d\n", x, y, graph[x][y]);
        totalCost += graph[x][y];
        selected[y] = 1;
        edges++;
    }
    printf("\nTotal Minimum Cost: %d\n", totalCost);
}

int main() {
    Edge edges[MAX];
    char nodeA[20], nodeB[20], label[20];
    int weight;

    char labels[MAX][20];
    int labelCount = 0;

    int graph[MAX][MAX];
    for (int i = 0; i < MAX; i++)
        for (int j = 0; j < MAX; j++)
            graph[i][j] = INF;

    printf("Enter graph in SIF Format: NodeA weight NodeB\n");
    printf("Example: A 5 B\n");
    printf("Press CTRL+Z (Windows) / CTRL+D (Linux) to stop input.\n\n");

    int edgeCount = 0;
    while (scanf("%s %d %s", nodeA, &weight, nodeB) != EOF) {
        strcpy(edges[edgeCount].nodeA, nodeA);
        edges[edgeCount].weight = weight;
        strcpy(edges[edgeCount].nodeB, nodeB);
        edgeCount++;
    }

    // Extract unique labels into labels[]
    for (int i = 0; i < edgeCount; i++) {
        if (findIndex(labels, labelCount, edges[i].nodeA) == -1)
            strcpy(labels[labelCount++], edges[i].nodeA);
        if (findIndex(labels, labelCount, edges[i].nodeB) == -1)
            strcpy(labels[labelCount++], edges[i].nodeB);
    }

    // Build adjacency matrix
    for (int i = 0; i < edgeCount; i++) {
        int a = findIndex(labels, labelCount, edges[i].nodeA);
        int b = findIndex(labels, labelCount, edges[i].nodeB);
        graph[a][b] = graph[b][a] = edges[i].weight;
    }

    printf("\nNode label indexing:\n");
    for (int i = 0; i < labelCount; i++)
        printf("%d => %s\n", i, labels[i]);

    prims(graph, labelCount);

    return 0;
}