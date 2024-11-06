#ifndef PATH_FINDING_H
#define PATH_FINDING_H

#include "maze.h"

#include <limits.h>

#define INF INT_MAX // We use INT_MAX to represent infinity

typedef struct {
    int x;
    int y;
} Position;

bool dfs(Maze* maze, bool** visited, Position current, Position end, Position path[], int* path_index);
int  solve_maze(Maze* maze, Position start, Position end);
bool is_valid(Maze* maze, bool** visited, int x, int y);

#endif
