#include "../include/path_finding.h"

// Function to find the node with the smallest distance that has not been visited
int min_distance(int** dist, bool** visited, int width, int height) {
    int min_dist = INT_MAX; // Initialize the minimum distance to infinity
    Node min_node = {-1, -1, INT_MAX}; // Initialize the node with the minimum distance

    // Find the node with the smallest distance that has not been visited
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (!visited[y][x] && dist[y][x] < min_dist) {
                min_dist = dist[y][x];
                min_node.x = x;
                min_node.y = y;
                min_node.dist = min_dist;
            }
        }
    }
    return (min_node.x == -1) ? -1 : min_node.dist; // Retourner la distance minimale ou -1 si aucun noeud n'est trouvé
}

// Function to check if the move is valid
bool is_valid_move(Maze* maze, int x, int y, bool** visited) {
    return (x >= 0 && x < maze->width && y >= 0 && y < maze->height && !visited[y][x]);
}

int dijkstra(Maze* maze) {
    int** dist = malloc(maze->height * sizeof(int*));  // Alloc memory for distances table
    bool** visited = malloc(maze->height * sizeof(bool*));  // Alloc memory for visited cells table

    int start_x = -1, start_y = -1;

    // Initialize the distances and visited cells tables
    for (int i = 0; i < maze->height; i++) {
        dist[i] = malloc(maze->width * sizeof(int));
        visited[i] = malloc(maze->width * sizeof(bool));
        for (int j = 0; j < maze->width; j++) {

            dist[i][j] = INT_MAX;  // Initialize distances to infinity
            visited[i][j] = false;  // No cells visited initially

            if (maze->grid[i][j].start) {
                start_x = j;
                start_y = i;
            }
        }
    }

    // If no start cell is found, return error
    if (start_x == -1 || start_y == -1) {
        fprintf(stderr, "Error: start cell not found\n");
        return -1;
    }

    // Initialize the distance of the start cell to 0
    dist[start_y][start_x] = 0;

    // Directions: left, right, up, down
    int dir_x[] = {-1, 1, 0, 0};
    int dir_y[] = {0, 0, -1, 1};

    // Main loop to explore the cells
    while (true) {
        // Find the cell with the smallest distance that has not been visited
        int min_dist = min_distance(dist, visited, maze->width, maze->height);
        if (min_dist == -1) break;  // If no cell is found, terminate

        // Mark the current cell as visited
        int curr_x = -1, curr_y = -1;
        for (int y = 0; y < maze->height; y++) {
            for (int x = 0; x < maze->width; x++) {
                if (dist[y][x] == min_dist && !visited[y][x]) {
                    curr_x = x;
                    curr_y = y;
                    break;
                }
            }
            if (curr_x != -1) break;
        }
        visited[curr_y][curr_x] = true;

        // If the current cell is the exit, return the distance
        if (maze->grid[curr_y][curr_x].end) {
            int result = dist[curr_y][curr_x];
            // Free memory before returning
            for (int i = 0; i < maze->height; i++) {
                free(dist[i]);
                free(visited[i]);
            }
            free(dist);
            free(visited);
            return result;
        }

        // Explore the neighbors (up, down, left, right)
        for (int d = 0; d < 4; d++) {
            int nx = curr_x + dir_x[d];
            int ny = curr_y + dir_y[d];

            // Check if the neighbor is within bounds and not visited
            if (is_valid_move(maze, nx, ny, visited)) {
                // Check for walls
                if ((d == 0 && !maze->grid[curr_y][curr_x].left) ||   // Left
                    (d == 1 && !maze->grid[curr_y][curr_x].right) ||  // Right
                    (d == 2 && !maze->grid[curr_y][curr_x].up) ||     // Up
                    (d == 3 && !maze->grid[curr_y][curr_x].down)) {   // Down

                    // If the distance can be reduced, update it
                    if (dist[curr_y][curr_x] + 1 < dist[ny][nx]) {
                        dist[ny][nx] = dist[curr_y][curr_x] + 1;
                    }
                }
            }
        }
    }

    // If no solution is found, free memory and return -1
    for (int i = 0; i < maze->height; i++) {
        free(dist[i]);
        free(visited[i]);
    }
    free(dist);
    free(visited);

    return -1;
}