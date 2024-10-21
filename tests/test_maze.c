#include "../include/maze.h"
#include "../include/maze_generator.h"
#include "../include/path_finding.h"

void test_maze_path(int width, int height) {
    printf("Testing maze of size %dx%d...\n", width, height);
    Maze* maze = init_maze(width, height);
    generate_maze(maze);

    int path_length = dijkstra(maze);
    
    if (path_length == -1) {
        fprintf(stderr, "Error: No path found in maze of size %dx%d\n", width, height);
        free_maze(maze);
        exit(1);
    }
    
    free_maze(maze);
    printf("Test passed for maze of size %dx%d with path length %d.\n", width, height, path_length);
}

int main() {
    // Path finding tests - Kylian

    // Liste des tailles de labyrinthes à test
    int sizes[][2] = {
        {5, 5},
        {10, 10},
        {20, 20},
        {30, 30},
        {40, 40},
        {50, 50},
        {60, 60},
        {70, 70},
        {80, 80},
        {90, 90},
        {100, 100},
        {200, 200},
        {300, 300}
    };
    int num_tests = sizeof(sizes) / sizeof(sizes[0]);
    
    for (int i = 0; i < num_tests; i++) {
        test_maze_path(sizes[i][0], sizes[i][1]);
    }

    // All other tests below :











    // End of tests
    
    printf("All tests OK.\n");
    return 0;
}