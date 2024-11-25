/* 
 * Jack Porter, Purdue ECE368
 * Assignment 1
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF INT_MAX

typedef struct {
    int target;
    int *weights;  // Array of weights for this edge
} Edge;

typedef struct {
    Edge **edges;  // Array of pointers to edges
    int edge_count;
} Vertex;

typedef struct {
    int vertex;
    int weight;
} HeapNode;

typedef struct {
    HeapNode *nodes;
    int size;
    int capacity;
    int *positions;
} MinHeap;

void minHeapify(MinHeap *heap, int idx);
MinHeap *createMinHeap(int capacity);
void insertHeap(MinHeap *heap, int v, int weight);
HeapNode extractMin(MinHeap *heap);
void decreaseKey(MinHeap *heap, int v, int weight);
int isInMinHeap(MinHeap *heap, int v);

// Function to parse the graph from the input file
Vertex *parseGraph(const char *filename, int *V, int *N) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d %d", V, N);

    Vertex *graph = (Vertex *)malloc((*V) * sizeof(Vertex));
    for (int i = 0; i < *V; ++i) {
        graph[i].edges = NULL;
        graph[i].edge_count = 0;
    }

    int vs, vt;
    while (fscanf(file, "%d %d", &vs, &vt) == 2) {
        graph[vs].edges = (Edge **)realloc(graph[vs].edges, (graph[vs].edge_count + 1) * sizeof(Edge *));
        Edge *edge = (Edge *)malloc(sizeof(Edge));
        edge->target = vt;
        edge->weights = (int *)malloc((*N) * sizeof(int));

        for (int i = 0; i < *N; ++i) {
            fscanf(file, "%d", &edge->weights[i]);
        }

        graph[vs].edges[graph[vs].edge_count++] = edge;
    }

    fclose(file);
    return graph;
}

// Function to find the shortest path using Dijkstra's algorithm
void dijkstra(Vertex *graph, int V, int N, int start, int end) {
    int *dist = (int *)malloc(V * sizeof(int));
    int *prev = (int *)malloc(V * sizeof(int));
    MinHeap *heap = createMinHeap(V);

    for (int i = 0; i < V; ++i) {
        dist[i] = INF;
        prev[i] = -1;
        insertHeap(heap, i, INF);
    }

    dist[start] = 0;
    decreaseKey(heap, start, 0);

    while (heap->size > 0) {
        HeapNode node = extractMin(heap);
        int u = node.vertex;

        if (u == end) break;

        for (int i = 0; i < graph[u].edge_count; ++i) {
            Edge *edge = graph[u].edges[i];
            int v = edge->target;
            int weight = edge->weights[dist[u] % N];  // Time-dependent weight

            if (dist[u] != INF && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                decreaseKey(heap, v, dist[v]);
            }
        }
    }

    // Output the shortest path
    if (dist[end] == INF) {
        printf("No path\n");
    } else {
        int current = end;
        int* path = malloc(sizeof(int) * V);
        int path_length = 0;

        while (current != -1) {
            path[path_length++] = current;
            current = prev[current];
        }

        for (int i = path_length - 1; i >= 0; --i) {
            printf("%d", path[i]);
            if (i > 0) printf(" ");
        }
        printf("\n");
        free(path);
    }

    free(dist);
    free(prev);
    free(heap->nodes);
    free(heap->positions);
    free(heap);
}

// MinHeap utility functions
MinHeap *createMinHeap(int capacity) {
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    heap->nodes = (HeapNode *)malloc(capacity * sizeof(HeapNode));
    heap->positions = (int *)malloc(capacity * sizeof(int));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void insertHeap(MinHeap *heap, int v, int weight) {
    if (heap->size == heap->capacity) {
        fprintf(stderr, "Heap overflow\n");
        return;
    }

    // Insert the new node at the end
    heap->size++;
    int i = heap->size - 1;

    // Store the node information
    heap->nodes[i].vertex = v;
    heap->nodes[i].weight = weight;
    heap->positions[v] = i;

    // Fix the min-heap property if it's violated
    while (i && heap->nodes[i].weight < heap->nodes[(i - 1) / 2].weight) {
        // Swap this node with its parent
        heap->positions[heap->nodes[i].vertex] = (i - 1) / 2;
        heap->positions[heap->nodes[(i - 1) / 2].vertex] = i;

        HeapNode temp = heap->nodes[i];
        heap->nodes[i] = heap->nodes[(i - 1) / 2];
        heap->nodes[(i - 1) / 2] = temp;

        i = (i - 1) / 2;
    }
}

void minHeapify(MinHeap *heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->nodes[left].weight < heap->nodes[smallest].weight)
        smallest = left;

    if (right < heap->size && heap->nodes[right].weight < heap->nodes[smallest].weight)
        smallest = right;

    if (smallest != idx) {
        HeapNode smallestNode = heap->nodes[smallest];
        HeapNode idxNode = heap->nodes[idx];

        heap->positions[smallestNode.vertex] = idx;
        heap->positions[idxNode.vertex] = smallest;

        heap->nodes[smallest] = idxNode;
        heap->nodes[idx] = smallestNode;

        minHeapify(heap, smallest);
    }
}

HeapNode extractMin(MinHeap *heap) {
    if (heap->size == 0) {
        HeapNode nullNode = { -1, INF };
        return nullNode;
    }

    HeapNode root = heap->nodes[0];
    HeapNode lastNode = heap->nodes[heap->size - 1];
    heap->nodes[0] = lastNode;

    heap->positions[root.vertex] = heap->size - 1;
    heap->positions[lastNode.vertex] = 0;

    heap->size--;
    minHeapify(heap, 0);

    return root;
}

void decreaseKey(MinHeap *heap, int v, int weight) {
    int i = heap->positions[v];
    heap->nodes[i].weight = weight;

    while (i && heap->nodes[i].weight < heap->nodes[(i - 1) / 2].weight) {
        heap->positions[heap->nodes[i].vertex] = (i - 1) / 2;
        heap->positions[heap->nodes[(i - 1) / 2].vertex] = i;

        HeapNode temp = heap->nodes[i];
        heap->nodes[i] = heap->nodes[(i - 1) / 2];
        heap->nodes[(i - 1) / 2] = temp;

        i = (i - 1) / 2;
    }
}

int isInMinHeap(MinHeap *heap, int v) {
    return heap->positions[v] < heap->size;
}

// Main function
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <graph_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int V, N;
    Vertex *graph = parseGraph(argv[1], &V, &N);

    int start, end;
    while (scanf("%d %d", &start, &end) == 2) {
        dijkstra(graph, V, N, start, end);
    }

    // Free memory
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < graph[i].edge_count; ++j) {
            free(graph[i].edges[j]->weights);
            free(graph[i].edges[j]);
        }
        free(graph[i].edges);
    }
    free(graph);

    return EXIT_SUCCESS;
}
