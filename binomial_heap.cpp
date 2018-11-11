#include <bits/stdc++.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct MinHeapNode
{
    int v;
    int dist;
};

struct MinHeap
{
    int size;
    int capacity;
    int *pos;
    struct MinHeapNode **nodes;
};

struct MinHeapNode *newMinHeapNode(int v, int dist)
{
    struct MinHeapNode *minHeapNode = (struct MinHeapNode *)malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

struct MinHeap *createMinHeap(int capacity)
{
    struct MinHeap *minHeap =
        (struct MinHeap *)malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->nodes =
        (struct MinHeapNode **)malloc(capacity * sizeof(struct MinHeapNode *));
    return minHeap;
}

void swapMinHeapNode(struct MinHeapNode **a, struct MinHeapNode **b)
{
    struct MinHeapNode *t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap *minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size &&
        minHeap->nodes[left]->dist < minHeap->nodes[smallest]->dist)
        smallest = left;

    if (right < minHeap->size &&
        minHeap->nodes[right]->dist < minHeap->nodes[smallest]->dist)
        smallest = right;

    if (smallest != idx)
    {
        MinHeapNode *smallestNode = minHeap->nodes[smallest];
        MinHeapNode *idxNode = minHeap->nodes[idx];

        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        swapMinHeapNode(&minHeap->nodes[smallest], &minHeap->nodes[idx]);

        minHeapify(minHeap, smallest);
    }
}

int isEmpty(struct MinHeap *minHeap)
{
    return minHeap->size == 0;
}

struct MinHeapNode *extractMin(struct MinHeap *minHeap)
{
    if (isEmpty(minHeap))
        return NULL;

    struct MinHeapNode *root = minHeap->nodes[0];

    struct MinHeapNode *lastNode = minHeap->nodes[minHeap->size - 1];
    minHeap->nodes[0] = lastNode;

    minHeap->pos[root->v] = minHeap->size - 1;
    minHeap->pos[lastNode->v] = 0;

    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

void decreaseKey(struct MinHeap *minHeap, int v, int dist)
{
    int i = minHeap->pos[v];

    minHeap->nodes[i]->dist = dist;

    while (i && minHeap->nodes[i]->dist < minHeap->nodes[(i - 1) / 2]->dist)
    {

        minHeap->pos[minHeap->nodes[i]->v] = (i - 1) / 2;
        minHeap->pos[minHeap->nodes[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&minHeap->nodes[i], &minHeap->nodes[(i - 1) / 2]);

        i = (i - 1) / 2;
    }
}

bool isInMinHeap(struct MinHeap *minHeap, int v)
{
    if (minHeap->pos[v] < minHeap->size)
        return true;
    return false;
}

void printArr(int dist[], int n)
{
    printf("Vertex Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
}

void dijkstra(vector<pair<int, int>> graph[], int src, int V)
{
    int dist[V];

    struct MinHeap *minHeap = createMinHeap(V);

    for (int v = 0; v < V; ++v)
    {
        dist[v] = INT_MAX;
        minHeap->nodes[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    minHeap->nodes[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);

    minHeap->size = V;

    while (!isEmpty(minHeap))
    {
        struct MinHeapNode *minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v;

        for (int i = 0; i < graph[u].size(); i++)
        {
            pair<int, int> info = graph[u][i];

            int v = info.first;
            int w = info.second;

            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX &&
                w + dist[u] < dist[v])
            {
                dist[v] = dist[u] + w;
                decreaseKey(minHeap, v, dist[v]);
            }
        }
    }
    printArr(dist, V);
}

int main()
{
    int nodes, edges, src, t, d;

    // cout << "\nEnter vertices: ";
    cin >> nodes;
    // cout << "\nEnter edges: ";
    cin >> edges;
    t = edges;

    vector<pair<int, int>> graph[nodes + 1];

    while (t--)
    {
        int a, b, w;
        // cout << "\na\tb\td";
        cin >> a >> b >> w;
        graph[a].push_back(make_pair(b, w));
        graph[b].push_back(make_pair(a, w));
    }

    // cout << "\nEnter source: ";
    cin >> src;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    dijkstra(graph, src, nodes);
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(t2 - t1).count();

    cout << "Total time taken: " << duration << endl;

    return 0;
}