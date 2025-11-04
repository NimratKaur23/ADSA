#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50
#define INF 99999

// Structure for SIF format: NodeA weight NodeB
typedef struct {
    char nodeA[20];
    int weight;
    char nodeB[20];
} Edge;

// Structure for sorted MST edges
struct EdgeList {
    int src, dest, weight;
};

int findIndex(char labels[MAX][20], int count, char node[]) {
    for (int i = 0; i < count; i++) {
        if (strcmp(labels[i], node) == 0)
            return i;
    }
    return -1;
}

int find(int parent[], int i) {
    while (parent[i] != i)
        i = parent[i];
    return i;
}

void unionSet(int parent[], int x, int y) {
    int xRoot = find(parent, x);
    int yRoot = find(parent, y);
    parent[yRoot] = xRoot;
}

int main() {
    Edge edges[MAX];
    char nodeA[20], nodeB[20];
    int weight;

    char labels[MAX][20];
    int labelCount = 0;
    int edgeCount = 0;

    printf("Enter graph in SIF Format: NodeA weight NodeB\n");
    printf("Example: A 5 B\n");
    printf("Press CTRL+Z (Windows) / CTRL+D (Linux) to stop input.\n\n");

    while (scanf("%s %d %s", nodeA, &weight, nodeB) != EOF) {
        strcpy(edges[edgeCount].nodeA, nodeA);
        edges[edgeCount].weight = weight;
        strcpy(edges[edgeCount].nodeB, nodeB);
        edgeCount++;
    }

    for (int i = 0; i < edgeCount; i++) {
        if (findIndex(labels, labelCount, edges[i].nodeA) == -1)
            strcpy(labels[labelCount++], edges[i].nodeA);
        if (findIndex(labels, labelCount, edges[i].nodeB) == -1)
            strcpy(labels[labelCount++], edges[i].nodeB);
    }

    struct EdgeList el[MAX];
    for (int i = 0; i < edgeCount; i++) {
        el[i].src = findIndex(labels, labelCount, edges[i].nodeA);
        el[i].dest = findIndex(labels, labelCount, edges[i].nodeB);
        el[i].weight = edges[i].weight;
    }

    for (int i = 0; i < edgeCount - 1; i++) {
        for (int j = i + 1; j < edgeCount; j++) {
            if (el[i].weight > el[j].weight) {
                struct EdgeList temp = el[i];
                el[i] = el[j];
                el[j] = temp;
            }
        }
    }

    int parent[MAX];
    for (int i = 0; i < labelCount; i++)
        parent[i] = i;

    printf("\nNode label indexing:\n");
    for (int i = 0; i < labelCount; i++)
        printf("%d => %s\n", i, labels[i]);

    printf("\nMinimum Spanning Tree Using Kruskal's Algorithm:\n");
    printf("Edge (NodeA - NodeB)  Weight\n");

    int totalCost = 0, edgesIncluded = 0, i = 0;

    while (edgesIncluded < labelCount - 1 && i < edgeCount) {
        int src = el[i].src;
        int dest = el[i].dest;

        int srcRoot = find(parent, src);
        int destRoot = find(parent, dest);

        if (srcRoot != destRoot) {
            printf("%s - %s\t %d\n", labels[src], labels[dest], el[i].weight);
            totalCost += el[i].weight;
            edgesIncluded++;
            unionSet(parent, srcRoot, destRoot);
        }
        i++;
    }

    printf("\nTotal Minimum Cost: %d\n", totalCost);

    return 0;
}