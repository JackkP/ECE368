/* 
 * Jack Porter, Purdue ECE368
 * Assignment 8
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF INT_MAX

// edge struct
typedef struct {
    int target;    // destination vertex
    int *weights;  // Array of weights for this edge
} Edge;

// vertex struct
typedef struct {
    Edge **edges;   // Array of pointers to edges
    int edge_count; // number of edges
} Vertex;


// using a min heap to for dijkstra's algorithm
// heap node struct
typedef struct {
    int vertex;
    int weight;
} HeapNode;

// minheap struct
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

int isInMinHeap(MinHeap *heap, int v) {
    return heap->positions[v] < heap->size;
}

void printHeap(MinHeap *heap) {
    for (int i=0; i<heap->size; i++){
        printf("%d, in pos %d, weight %d\n", heap->nodes[i].vertex, heap->positions[heap->nodes[i].vertex], heap->nodes[i].weight);
    }
}

// Function to parse the graph from the input file
Vertex *parseGraph(const char *filename, int *V, int *N) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    //scan in number of verticies and depth of edge weights
    fscanf(file, "%d %d", V, N);

    //create an array of verticies
    Vertex *graph = (Vertex *)malloc((*V) * sizeof(Vertex));
    for (int i = 0; i < *V; ++i) {
        graph[i].edges = NULL;
        graph[i].edge_count = 0;
    }

    //source vertex and target vertex for each edge
    int vs, vt;

    //for every set of verticies
    while (fscanf(file, "%d %d", &vs, &vt) == 2) {
        //add a new edge to the array of edges for that node
        graph[vs].edges = (Edge **)realloc(graph[vs].edges, (graph[vs].edge_count + 1) * sizeof(Edge *));
        Edge *edge = (Edge *)malloc(sizeof(Edge));
        edge->target = vt;
        edge->weights = (int *)malloc((*N) * sizeof(int));

        //read the edges in
        for (int i = 0; i < *N; ++i) {
            fscanf(file, "%d", &edge->weights[i]);
        }

        //increment edge count and add new edge to array
        graph[vs].edges[graph[vs].edge_count++] = edge;
    }

    fclose(file);
    return graph;
}

// Function to find the shortest path using Dijkstra's algorithm
void dijkstra(Vertex *graph, int V, int N, int start, int end) {
    // Distance array: dist[node][time]
    int **dist = (int **)malloc(V * sizeof(int *));
    for (int i = 0; i < V; ++i) {
        dist[i] = (int *)malloc(N * sizeof(int));
        for (int j = 0; j < N; ++j)
            dist[i][j] = INF;
    }

    // Priority queue for (node, time, distance)
    MinHeap *heap = createMinHeap(V * N);

    // Initialize start node at time 0
    dist[start][0] = 0;
    insertHeap(heap, start * N + 0, 0);  // Encode (node, time) as a single integer

    // Previous array to reconstruct the path
    int **prev_node = (int **)malloc(V * sizeof(int *));
    int **prev_time = (int **)malloc(V * sizeof(int *));
    for (int i = 0; i < V; ++i) {
        prev_node[i] = (int *)malloc(N * sizeof(int));
        prev_time[i] = (int *)malloc(N * sizeof(int));
        for (int j = 0; j < N; ++j) {
            prev_node[i][j] = -1;
            prev_time[i][j] = -1;
        }
    }

    while (heap->size > 0) {
        // Extract the state with the minimum distance
        HeapNode node = extractMin(heap);
        int u = node.vertex / N;  // Decode node
        int time = node.vertex % N;  // Decode time

        // If we reached the target node, reconstruct and print the path
        if (u == end) {
            int current = end, current_time = time;
            int *path = malloc(sizeof(int) * V * N);
            int path_length = 0;

            while (current != -1) {
                path[path_length++] = current;
                int temp = prev_node[current][current_time];
                current_time = prev_time[current][current_time];
                current = temp;
            }

            for (int i = path_length - 1; i >= 0; --i) {
                printf("%d", path[i]);
                if (i > 0) printf(" ");
            }
            printf("\n");

            free(path);
            break;
        }

        // Relaxation for all neighbors
        for (int i = 0; i < graph[u].edge_count; ++i) {
            Edge *edge = graph[u].edges[i];
            int v = edge->target;
            int next_time = (time + 1) % N;
            int weight = edge->weights[time];
            int new_dist = dist[u][time] + weight;

            if (new_dist < dist[v][next_time]) {
                dist[v][next_time] = new_dist;
                prev_node[v][next_time] = u;
                prev_time[v][next_time] = time;

                if (isInMinHeap(heap, v * N + next_time))
                    decreaseKey(heap, v * N + next_time, new_dist);
                else
                    insertHeap(heap, v * N + next_time, new_dist);
            }
        }
    }

    // Free memory
    for (int i = 0; i < V; ++i) {
        free(dist[i]);
        free(prev_node[i]);
        free(prev_time[i]);
    }
    free(dist);
    free(prev_node);
    free(prev_time);
    free(heap->nodes);
    free(heap->positions);
    free(heap);
}

// MinHeap utility function to create heap
MinHeap *createMinHeap(int capacity) {
    // MinHeap stores
    //      an array of HeapNodes
    //      size of the heap
    //      positions of each vertex
            
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    heap->nodes = (HeapNode *)malloc(capacity * sizeof(HeapNode));
    heap->positions = (int *)malloc(capacity * sizeof(int));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

// MinHeap utility function to insert a node into the heap
void insertHeap(MinHeap *heap, int v, int weight) {
    //heap overflow
    if (heap->size == heap->capacity) {
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

// decreases the distance to a node in the heap and adjusts its position accordingly
void decreaseKey(MinHeap *heap, int v, int weight) {
    //printf("decreasing key %d in pos %d with weight %d to %d\n", v, heap->positions[v], heap->nodes[heap->positions[v]].weight, weight);
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

// Main function
int main(int argc, char *argv[]) {
    //check to make sure that a filename is provided
    if (argc != 2) {
        return EXIT_FAILURE;
    }

    //read the graph (as well as number of verticies and edge depth) in from the file
    int V, N;
    Vertex *graph = parseGraph(argv[1], &V, &N);
    //printf("graph parsed\n");

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
