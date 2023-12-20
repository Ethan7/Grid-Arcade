/* By Ethan Hughes */
/* Written 12/5/2018 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
//#include<time.h>
#include"arcade-defs.h"
/*
#define ARCADE 0
#define PATH 2

#define EMPTY -1
#define WALL 0
#define CLOSED 1
#define START 2
#define END 3
#define OPEN 4
#define FINAL 5
*/
struct node{
	int stored;
	int g_cost, h_cost;
	int x, y;
	struct node *parent;
} *open, *closed;

int *priority_queue;
int queue_length;
const int start_x = 0, start_y = 0; //chance = 3, 
int end_x = 19, end_y = 19, open_length = 0, closed_length = 0;

void enqueue(int index){
	priority_queue[queue_length] = index;
    queue_length++;
}

int peek(){
    int highest_priority = 10000;
    int ind = -1;

    for (int i = 0; i <= queue_length; i++) {
		int priority = open[priority_queue[i]].g_cost+open[priority_queue[i]].h_cost;
        if (highest_priority > priority) {
            highest_priority = priority;
            ind = i;
        }
    }

    return ind;
}

int dequeue(){
    const int ind = peek();
	const int ret = priority_queue[ind];
    for (int i = ind; i < queue_length-1; i++) {
        priority_queue[i] = priority_queue[i + 1];
    }
    queue_length--;

	return ret;
}

void neighbor_function(int neighbor_x, int neighbor_y, int neighbor_cost, struct node *current, int height){
	struct node *neighbor;
	const int index = neighbor_x * height + neighbor_y;
	if(closed[index].stored){
		return;
	}
	int b = 1;
	if(open[index].stored){
		b = 0;
		neighbor = &(open[index]);
	}
	if(b){
		open[index].parent = current;
		open[index].x = neighbor_x;
		open[index].y = neighbor_y;
		open[index].g_cost = neighbor_cost;
		open[index].h_cost = (abs(neighbor_x - end_x) * 10) + 
						(abs(neighbor_y - end_y) * 10);
		open[index].stored = 1;
		enqueue(index);
		open_length++;
	} else if(neighbor_cost >= neighbor->g_cost){
		return;
	} else {
		neighbor->g_cost = neighbor_cost;
		neighbor->parent = current;
	}
}

int path(int **grid, SDL_Event event, int t, int width, int height){
	int ret = PATH;

	if(t == 1){
		//srand(time(NULL)); //Generate random numbers

		//start_x = 0;
		//start_y = 0;
		end_x = width-1;
		end_y = height-1;

		//for(int i = 0; i < width; i++){
		//	for(int n = 0; n < height; n++){
				//grid[i][n] = EMPTY;
				//if(!(i == startNodeX && n == startNodeY) && 
				//!(i == endNodeX && n == endNodeY) && rand() % chance == 0){
				//	grid[i][n] = WALL;
				//}
		//	}
		//}

		closed_length = 0;
		closed = (struct node *) calloc(width*height, sizeof(struct node));

		open_length = 1;
		const int index = start_x*height+start_y;
		open = (struct node *) calloc(width*height, sizeof(struct node));

		open[index].parent = NULL;
		open[index].x = start_x;
		open[index].y = start_y;
		open[index].g_cost = 0;
		open[index].h_cost = (abs(start_x - end_x) * 10) + 
					(abs(start_y - end_y) * 10);
		open[index].stored = 1;

		priority_queue = (int *) calloc(width*height, sizeof(int));
		enqueue(index);
	}

	//Make sure not to replace walls or the final path
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			if(i == start_x && j == start_y){
				//Place Start
				grid[i][j] = EMPTY;
			} else if (i == end_x && j == end_y){
				//Place End
				grid[i][j] = EMPTY;
			} else if(closed[i*height+j].stored){
				//Place Closed Node
				grid[i][j] = EMPTY;
			} else if(open[i*height+j].stored){
				//Place Open Node
				grid[i][j] = EMPTY;
			}
		}
	}

	int current = dequeue();
	if(open[current].x == end_x && open[current].y == end_y){
		struct node *current_node = &(open[current]);
		while(current_node){
			grid[current_node->x][current_node->y] = FINAL;
			current_node = current_node->parent;
		}
		ret = ARCADE;
	}

	//Add current to closed set
	const int closed_index = open[current].x * height + open[current].y;
	closed[closed_index] = open[current];
	const int closed_x = closed[closed_index].x;
	const int closed_y = closed[closed_index].y;
	const int closed_cost = closed[closed_index].g_cost;
	struct node *closedNew = &(closed[closed_index]);
	closed_length++;

	open[current].stored = 0;
	open_length--;

	//for upper neighbor
	if(closed_y > 0 && grid[closed_x][closed_y-1] != WALL){
		neighbor_function(closed_x, closed_y-1, closed_cost+10, closedNew, height);
	}
	//for lower neighbor
	if(closed_y < height-1 && grid[closed_x][closed_y+1] != WALL){
		neighbor_function(closed_x, closed_y+1, closed_cost+10, closedNew, height);
	}
	//for left neighbor
	if(closed_x > 0 && grid[closed_x-1][closed_y] != WALL){
		neighbor_function(closed_x-1, closed_y, closed_cost+10, closedNew, height);
	}
	//for right neighbor
	if(closed_x < width-1 && grid[closed_x+1][closed_y] != WALL){
		neighbor_function(closed_x+1, closed_y, closed_cost+10, closedNew, height);
	}
	//for upper left neighbor
	if(closed_y > 0 && closed_x > 0 && grid[closed_x-1][closed_y-1] != WALL){
		neighbor_function(closed_x-1, closed_y-1, closed_cost+14, closedNew, height);
	}
	//for upper right neighbor
	if(closed_y > 0 && closed_x < width-1 && grid[closed_x+1][closed_y-1] != WALL){
		neighbor_function(closed_x+1, closed_y-1, closed_cost+14, closedNew, height);
	}
	//for lower left neighbor
	if(closed_y < height-1 && closed_x > 0 && grid[closed_x-1][closed_y+1] != WALL){
		neighbor_function(closed_x-1, closed_y+1, closed_cost+14, closedNew, height);
	}
	//for lower right neighbor
	if(closed_y < height-1 && closed_x < width-1 && grid[closed_x+1][closed_y+1] != WALL){
		neighbor_function(closed_x+1, closed_y+1, closed_cost+14, closedNew, height);
	}

	//Fill Display
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			if(i == start_x && j == start_y){
				//Place Start
				grid[i][j] = START;
			} else if (i == end_x && j == end_y){
				//Place End
				grid[i][j] = END;
			} else if(grid[i][j] != FINAL && closed[i*height+j].stored){
				//Place Closed Node
				grid[i][j] = CLOSED;
			} else if(grid[i][j] != FINAL && open[i*height+j].stored){
				//Place Open Node
				grid[i][j] = OPEN;
			}
		}
	}

	if(open_length == 0){
		ret = ARCADE;
	}

	//Memory Cleanup
	if(ret == ARCADE){
		free(open);
		free(closed);
		free(priority_queue);
	}

	return ret;
}
