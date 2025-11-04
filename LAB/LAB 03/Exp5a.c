
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_NAME_LEN 50

// Structure for adjacency list
struct Node {
    char name[MAX_NAME_LEN];
};

struct Graph {
    struct Node nodes[MAX_NODES];
    int adj[MAX_NODES][MAX_NODES];
    int nodeCount;
};

// Function to search for a node and return index
int findNodeIndex(struct Graph *g, char *name) {
    for (int i = 0; i < g->nodeCount; i++) {
        if (strcmp(g->nodes[i].name, name) == 0)
            return i;
    }
    return -1;
}

// Function to add node to graph
int addNode(struct Graph *g, char *name) {
    int index = findNodeIndex(g, name);
    if (index != -1) return index; // already exists

    strcpy(g->nodes[g->nodeCount].name, name);
    g->nodeCount++;
    return g->nodeCount - 1;
}

// Function to add an edge (A interacts B)
void addEdge(struct Graph *g, char *src, char *dest) {
    int u = addNode(g, src);
    int v = addNode(g, dest);
    g->adj[u][v] = 1; // directed graph
}

// Function to print graph as adjacency list
void printGraph(struct Graph *g) {
    printf("\n--- Graph Generated (Adjacency List) ---\n");

    for (int i = 0; i < g->nodeCount; i++) {
        printf("%s -> ", g->nodes[i].name);
        for (int j = 0; j < g->nodeCount; j++) {
            if (g->adj[i][j] == 1) {
                printf("%s ", g->nodes[j].name);
            }
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input_file.sif>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }

    struct Graph g = {0};

    char src[MAX_NAME_LEN], relation[MAX_NAME_LEN], dest[MAX_NAME_LEN];

    while (fscanf(fp, "%s %s %s", src, relation, dest) != EOF) {
        addEdge(&g, src, dest);
    }

    fclose(fp);
    printGraph(&g);

    return 0;
}