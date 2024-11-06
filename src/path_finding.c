#include "../include/path_finding.h"

bool is_valid(Maze* maze, bool** visited, int x, int y) {
    return (x >= 0 && x < maze->width && y >= 0 && y < maze->height && !visited[y][x]);
}

bool dfs(Maze* maze, bool** visited, Position current, Position end, Position path[], int* path_index) {
    if (current.x == end.x && current.y == end.y) {
        path[(*path_index)++] = current;
        return true;
    }

    visited[current.y][current.x] = true;
    path[(*path_index)++] = current;

    Position directions[] = {
        {current.x + 1, current.y}, // Right
        {current.x - 1, current.y}, // Left
        {current.x, current.y + 1}, // Down
        {current.x, current.y - 1}  // Up
    };

    for (int i = 0; i < 4; i++) {
        Position next = directions[i];
        if (is_valid(maze, visited, next.x, next.y)) {
            if ((i == 0 && !maze->grid[current.y][current.x].right) || 
                (i == 1 && !maze->grid[current.y][current.x].left) || 
                (i == 2 && !maze->grid[current.y][current.x].down) || 
                (i == 3 && !maze->grid[current.y][current.x].up)) {
                if (dfs(maze, visited, next, end, path, path_index)) {
                    return true;
                }
            }
        }
    }

    (*path_index)--;
    return false;
}

int solve_maze(Maze* maze, Position start, Position end) {
    bool** visited = (bool**)malloc(maze->height * sizeof(bool*));
    for (int i = 0; i < maze->height; i++) {
        visited[i] = (bool*)malloc(maze->width * sizeof(bool));
        for (int j = 0; j < maze->width; j++) {
            visited[i][j] = false;
        }
    }

    Position path[maze->width * maze->height];
    int path_index = 0;

    if (dfs(maze, visited, start, end, path, &path_index)) {
        printf("Path found:\n");
        for (int i = 0; i < path_index; i++) {
            printf("(%d, %d)\n", path[i].x, path[i].y);
        }
        for (int i = 0; i < maze->height; i++) {
            free(visited[i]);
        }
        free(visited);
        return path_index;
    } else {
        printf("No path found.\n");
        for (int i = 0; i < maze->height; i++) {
            free(visited[i]);
        }
        free(visited);
        return -1;
    }
}
