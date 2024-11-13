// This is a file C to find a path from a random start cell to end cell by algorithme DFS
#include "../include/maze.h"
#include "../include/path_finding.h"


int directionHeight[4] = {0, 0, -1, 1}; // Correspond à LEFT,RIGHT,UP,DOWN
int directionWidth[4] = {-1, 1, 0, 0};// Correspond à LEFT,RIGHT,UP,DOWN
bool** visited; // list of visited cells
Pile* stack; // A stack to contain cells using LIFO ( Last In First Out )
int len = -1; // current length of the stack (LIFO)

void ajout(int height, int width){ // add the cell to the stack (LIFO)
	len = len +1;
	stack[len].height = height;
    stack[len].width = width;
}

Pile supprimer(){ // delete the last cell from the stack
	len = len-1;
	return stack[len];
}

int check(int height,int width, int current_height, int current_width){ // check if the current cell is valid or not
	int a = 0;
	if(current_width >= 0 && current_width < width && current_height >= 0 && current_height < height){
		a = 1;
	}
	return a;
}

void create_visited(int height,int width){ // create the list of visited cells ( at the beginning, no cell is visited)
    visited = (bool**)malloc(height * sizeof(bool*));
    for(int i = 0;i<height;i++){
        visited[i] = (bool*)malloc(width * sizeof(bool));
        for(int j = 0;j<width;j++){
            visited[i][j] = 0;
        }
    }
}

// This function is to check if there is a wall adjacent to the current cell or not
// return false if there is no wall adjacent to the current cell
// return true if there is a wall adjacent to the current cell
bool isWall(Maze* maze, int current_height,int current_width, int direction){
    if(direction == 0){ // check if there is a wall is left to the current cell or not
        return maze->grid[current_height][current_width].left;
    }
    if(direction == 1){ // check if there is a wall is right to the current cell or not
        return maze->grid[current_height][current_width].right;
    }
    if(direction == 2){ // check if there is a wall is up to the current cell or not
        return maze->grid[current_height][current_width].up;
    }
    if(direction == 3){ // check if there is a wall is down to the current cell or not
        return maze->grid[current_height][current_width].down;
    }
    return true; // if not then there is a wall
}

// This function creates a stack of cells in the path from start cell to the end cell
// The length of stack is -1 if there is no path from start cell to the end cell
void dfs(Maze* maze){
    // create the stack
    stack = (Pile*)malloc(maze->width * maze->height * sizeof(Pile));
    int start_width;
    int start_height;
    // find the starting cell
    for(int i = 0;i<maze->height;i++){
        for(int j = 0;j<maze->width;j++){
            if(maze->grid[i][j].start == true){
                start_height = i;
                start_width = j;
            }
        }
    }
    int current_height = start_height;
    int current_width = start_width;
    create_visited(maze->height,maze->width); // generate the visiting cells 	
    ajout(current_height,current_width); // add the starting cell to the stack
    visited[current_height][current_width] = 1; // the starting cell is visited
	while(visited[maze->height-1][maze->width-1] != 1 && len != -1){ // loop to find the end cell or if (len == -1, there is no path from start cell to the end cell)
		Pile current_cell = stack[len]; // select the current cell as the last cell of the stack
		int current_cell_height = current_cell.height; // height of the current cell
		int current_cell_width = current_cell.width; // width of the current cell
		int directions[4]; // list of directions possible for the current cell to the next cell
		int len_directions = 0;
		// dir = 0 : left
		// dir = 1 : right
		// dir = 2 : up
		// dir = 3 : down
		for (int dir = 0; dir < 4; dir++){ // loop to find the possible direction for the current cell
            int new_width = current_cell_width+directionWidth[dir];
            int new_height = current_cell_height+directionHeight[dir];
            if(check(maze->height,maze->width,new_height,new_width) && visited[new_height][new_width] == 0 && isWall(maze,current_cell_height,current_cell_width,dir) == false){
                directions[len_directions] = dir;
                len_directions++;
            }
		}
		// check if there is any possible direction from the current cell to the next cell
		if(len_directions > 0){ // if it exists possible direction from the current cell to the next cell
			int direct = directions[rand()%len_directions]; // Select direction by random 
			int nouvelle_width = current_cell_width + directionWidth[direct]; // the width of next cell
			int nouvelle_height = current_cell_height + directionHeight[direct]; // the height of next cell
			visited[nouvelle_height][nouvelle_width] = 1; // select next cell as visited
			ajout(nouvelle_height,nouvelle_width); // add the next cell to the stack
		}
		else{ // if there is no possible direction from the current cell to the next cell
			supprimer(); // delete the current cell (or the last cell) from the stack
		}
	}
    // We don't free all pointers yet because we will use later for printing the maze with the path from start cell to the end cell
    return;
}

// this function prints all of the cells from the path of the start cell to the end cell
void affcher_stack(){
    if(len < 1){ 
        printf("There is no way from starting cell to the ending cell\n");
        return;
    }
    printf("{");
    for(int i = 1;i<len;i++){
        afficher_cell(stack[i].height,stack[i].width);
        printf(", ");
    }
    afficher_cell(stack[len].height,stack[len].width);
    printf("}\n");
    printf("The length of path is %d\n",len);
}

// print the cell
void afficher_cell(int height,int width){
	printf("( ");
	printf("%d, %d",height,width);
    printf(")");
	return;
}

// set relationship for cells in the path from start cell to the end cell
void set_relationship(Maze* maze){
    for(int i = 0;i<len;i++){
        if(stack[i].width-1 == stack[i+1].width){ // There is a cell in the left in the path of the current cell 
            maze->grid[stack[i].height][stack[i].width].path[0] = 1; // Set the relationship of the current cell
            maze->grid[stack[i+1].height][stack[i+1].width].path[1] = 1; // Set the relationship of the adjacent cell
        }
        else if(stack[i].width+1 == stack[i+1].width){ // There is a cell in the right in the path of the current cell 
            maze->grid[stack[i].height][stack[i].width].path[1] = 1; // Set the relationship of the current cell
            maze->grid[stack[i+1].height][stack[i+1].width].path[0] = 1; // Set the relationship of the adjacent cell
        }
        else if(stack[i].height-1 == stack[i+1].height){ // There is a cell in the up in the path of the current cell 
            maze->grid[stack[i].height][stack[i].width].path[2] = 1; // Set the relationship of the current cell
            maze->grid[stack[i+1].height][stack[i+1].width].path[3] = 1; // Set the relationship of the adjacent cell
        }
        else if(stack[i].height+1 == stack[i+1].height){ // There is a cell in the down in the path of the current cell 
            maze->grid[stack[i].height][stack[i].width].path[3] = 1; // Set the relationship of the current cell
            maze->grid[stack[i+1].height][stack[i+1].width].path[2] = 1; // Set the relationship of the adjacent cell
        }
        else{
            continue;
        }
    }
}

// Just use this function for the test cases
// The length of the path from the start cell to the end cell (-1 if there is no such path)
int pathLength(Maze* maze){
    dfs(maze);
    // free for the use of test cases
    for (int i = 0; i < maze->height; i++) {
        free(visited[i]);
    }
    free(visited);
    free(stack);
    return len; // len = -1 if there is no path from the start cell to the end cell
}

// This function is to print the path from the start cell to the end cell in the maze
void print_maze_after_dfs(Maze* maze) {
    printf("\n");
    printf("\n");	
    printf("The length of path is %d\n",len);
    printf("The maze with the shortest path is : \n\n");
    // Top wall
    for (int j = 0; j < maze->width; j++) {
        printf("+---");  // top wall of each cell
    }
    printf("+\n");  // Dernier coin top right

    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            // left wall of each cell
            if (maze->grid[i][j].left || j == 0) {
                printf("|");
            } else {
                printf(" ");
            }

            // Cell content
            if (maze->grid[i][j].start) {
                if(maze->grid[i][j].path[0] == 1){// left
                    printf("━S ");
                }
                else if(maze->grid[i][j].path[1] == 1){// right
                    printf(" S━");
                }
                else{
                    printf(" S ");
                }
            } else if (maze->grid[i][j].end) {
                if(maze->grid[i][j].path[0] == 1){// left
                    printf("━E ");
                }
                else if(maze->grid[i][j].path[1] == 1){// right
                    printf(" E━");
                }
                else{
                    printf(" E ");
                }
            } else {
                if(maze->grid[i][j].path[0] == 1 && maze->grid[i][j].path[2] == 1){// left up
                    printf("━╹ ");
                }
                else if(maze->grid[i][j].path[0] == 1 && maze->grid[i][j].path[3] == 1){// left down
                    printf("━╻ ");
                }
                else if(maze->grid[i][j].path[1] == 1 && maze->grid[i][j].path[2] == 1){// right up
                    printf(" ╹━");
                }
                else if(maze->grid[i][j].path[1] == 1 && maze->grid[i][j].path[3] == 1){// right down
                    printf(" ╻━");
                }
                else if(maze->grid[i][j].path[0] == 1 && maze->grid[i][j].path[1] == 1){ // left right
                    printf("━ ━");
                }
                else if(maze->grid[i][j].path[2] == 1 && maze->grid[i][j].path[3] == 1){ // up down
                    printf(" ┃ ");
                }
                else{
                    printf("   ");
                }
            }
        }
        printf("|\n");  // Last right wall

        // Bottom wall of each cell
        for (int j = 0; j < maze->width; j++) {
            if (maze->grid[i][j].down || i == maze->height - 1) {
                printf("+---");
            } else {
                printf("+   ");
            }
        }
        printf("+\n");  // Last corner of each row
    }
}

// This function is to generate the maze with the path from the start cell to the end cell
void afficher_maze_with_paths(Maze* maze){
    dfs(maze);
    if(len < 1){ // If there is no path from start cell to the end cell
        printf("No path found in maze of size %dx%d\n", maze->width, maze->height);
        return;
    }
    else{ // If there is a path from start cell to the end cell
        // To print the list of cells in the path from the start cell to the end cell
        //affcher_stack();  
        set_relationship(maze); // set relationship between cells in the path  
        print_maze_after_dfs(maze); // print the maze with the path from the start cell to the end cell

	// free all of the pointers lists we have used    
        for (int i = 0; i < maze->height; i++) {
            free(visited[i]);
        }
        free(visited);
        free(stack);
    }
}

