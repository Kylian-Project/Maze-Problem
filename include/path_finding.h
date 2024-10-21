#ifndef PATH_FINDING_H
#define PATH_FINDING_H

#include "maze.h"

#include <limits.h>

#define INF INT_MAX // We use INT_MAX to represent infinity

typedef struct {
    int x;
    int y;
    int dist;
} Node;

int dijkstra(Maze* maze);

#endif
