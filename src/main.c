#include "../include/maze.h"
#include "../include/maze_generator.h"
#include "../include/path_finding.h"

int main(int argc, char *argv[]) {
    // Chceck Args NB
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <width> <height>\n", argv[0]);
        return 1;
    }
    
    // Convert to int
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    
    if (width <= 0 || height <= 0) {
        fprintf(stderr, "Width and height must be positive integers.\n");
        return 1;
    }
    
    // init alea gen
    srand(time(NULL));
    
    // Init maze
    Maze* maze = init_maze(width, height);
    
    // Gen maze
    generate_maze(maze);

    Position start = {0, 0};
    Position end = {maze->width - 1, maze->height - 1};
    
    // Print maze with beauti ASCII :)
    print_maze(maze);

    // Display path length
    int path_length =  solve_maze(maze, start, end);
    printf("Path length: %d\n", path_length);
    
    // Free maze
    free_maze(maze);
    return 0;
}