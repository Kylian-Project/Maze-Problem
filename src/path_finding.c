// ---------------------------------------------------------
// Implementation of path finding algorithms with ChatGPT
// This is a temporary implementation until the next session
// And is for testing only. Don't pay attention.
// ---------------------------------------------------------

#include "../include/maze.h"

bool is_valid_move(Maze* maze, int x, int y, bool** visited) {
    return (x >= 0 && x < maze->width && y >= 0 && y < maze->height && !visited[y][x]);
}

bool dfs(Maze* maze, int x, int y, bool** visited) {
    if (maze->grid[y][x].end) return true;
    visited[y][x] = true;
    if (!maze->grid[y][x].up && is_valid_move(maze, x, y - 1, visited) && dfs(maze, x, y - 1, visited)) return true;
    if (!maze->grid[y][x].down && is_valid_move(maze, x, y + 1, visited) && dfs(maze, x, y + 1, visited)) return true;
    if (!maze->grid[y][x].left && is_valid_move(maze, x - 1, y, visited) && dfs(maze, x - 1, y, visited)) return true;
    if (!maze->grid[y][x].right && is_valid_move(maze, x + 1, y, visited) && dfs(maze, x + 1, y, visited)) return true;
    return false;
}

bool has_path(Maze* maze) {
    int entry_x = -1, entry_y = -1;
    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            if (maze->grid[y][x].start) {
                entry_x = x;
                entry_y = y;
                break;
            }
        }
        if (entry_x != -1) break;
    }
    if (entry_x == -1 || entry_y == -1) return false;
    bool** visited = malloc(maze->height * sizeof(bool*));
    for (int i = 0; i < maze->height; i++) visited[i] = calloc(maze->width, sizeof(bool));
    bool path_exists = dfs(maze, entry_x, entry_y, visited);
    for (int i = 0; i < maze->height; i++) free(visited[i]);
    free(visited);
    return path_exists;
}