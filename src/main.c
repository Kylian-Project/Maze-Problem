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

    if (width == 1 && height == 1) {
        fprintf(stderr, "Width and height must be both different from 1.\n");
        return 1;
    }
    
    // init alea gen
    srand(time(NULL));
    
    // Init maze
    Maze* maze = init_maze(width, height);
    
    generate_maze(maze);
    // Print maze with beauti ASCII :)
    print_maze(maze);
    
    // print the maze with the path from start cell to the end cell (if there is no path, a text will be printed instead)
    afficher_maze_with_paths(maze);

    // Free maze
    free_maze(maze);
    return 0;
}
