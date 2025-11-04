#include <stdio.h>
#define INF 99999

void dijkstra(int graph[20][20], int n, int src) {
    int dist[20], visited[20];

    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }

    dist[src] = 0;

    for (int count = 0; count < n - 1; count++) {
        int min = INF, u;

        for (int v = 0; v < n; v++)
            if (!visited[v] && dist[v] <= min)
                min = dist[v], u = v;

        visited[u] = 1;

        for (int v = 0; v < n; v++)
            if (!visited[v] && graph[u][v] && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }

    printf("\nShortest distances from source %d:\n", src);
    for (int i = 0; i < n; i++)
        printf("Node %d ---> Distance = %d\n", i, dist[i]);
}

int main() {
    int graph[20][20], n, src;

    printf("Enter number of vertices: ");
    scanf("%d", &n);

    printf("Enter adjacency matrix (use 0 if no edge):\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &graph[i][j]);

    printf("Enter source vertex: ");
    scanf("%d", &src);

    dijkstra(graph, n, src);
    return 0;
}