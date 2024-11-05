#ifndef PATH_FINDING_H
#define PATH_FINDING_H

#include "maze.h"

#include <limits.h>

#define INF INT_MAX // We use INT_MAX to represent infinity

// Structure pour représenter un nœud dans le tas
typedef struct {
    int x, y, dist;
} Node;

// Structure pour représenter le tas binaire
typedef struct {
    Node* nodes;
    int size;
    int capacity;
} MinHeap;

int dijkstra(Maze* maze);

#endif
