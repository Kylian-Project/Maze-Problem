#include "../include/maze.h"
#include "../include/path_finding.h"

bool is_valid(Maze* maze, bool** visited, int x, int y) {
    return (x >= 0 && x < maze->width && y >= 0 && y < maze->height && !visited[y][x]);
}

int solve_maze(Maze* maze, Position start, Position end) {
    bool** visited = (bool**)malloc(maze->height * sizeof(bool*));
    for (int i = 0; i < maze->height; i++) {
        visited[i] = (bool*)malloc(maze->width * sizeof(bool));
        for (int j = 0; j < maze->width; j++) {
            visited[i][j] = false;
        }
    }

    Position directions[] = {
        {1, 0}, // Right
        {-1, 0}, // Left
        {0, 1}, // Down
        {0, -1}  // Up
    };

    StackNode* stack = (StackNode*)malloc(maze->width * maze->height * sizeof(StackNode));
    int stack_size = 0;

    stack[stack_size++] = (StackNode){start, 0};
    visited[start.y][start.x] = true;

    while (stack_size > 0) {
        StackNode current = stack[--stack_size];
        Position pos = current.pos;

        if (pos.x == end.x && pos.y == end.y) {
            for (int i = 0; i < maze->height; i++) {
                free(visited[i]);
            }
            free(visited);
            free(stack);
            return stack_size + 1;
        }

        for (int i = current.direction; i < 4; i++) {
            Position next = {pos.x + directions[i].x, pos.y + directions[i].y};
            if (is_valid(maze, visited, next.x, next.y)) {
                if ((i == 0 && !maze->grid[pos.y][pos.x].right) || 
                    (i == 1 && !maze->grid[pos.y][pos.x].left) || 
                    (i == 2 && !maze->grid[pos.y][pos.x].down) || 
                    (i == 3 && !maze->grid[pos.y][pos.x].up)) {
                    stack[stack_size++] = (StackNode){pos, i + 1};
                    stack[stack_size++] = (StackNode){next, 0};
                    visited[next.y][next.x] = true;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < maze->height; i++) {
        free(visited[i]);
    }
    free(visited);
    free(stack);
    return -1;
}