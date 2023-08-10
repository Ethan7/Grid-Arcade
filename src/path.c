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
	int gCost, hCost;
	int x, y;
	struct node *parent;
} *open, *closed;

int *priorityq;
int qLen;
int startX = 0, startY = 0, //chance = 3, 
endX = 19, endY = 19, openLen = 0, closedLen = 0;

void enqueue(int index){
	priorityq[qLen] = index;
    qLen++;
}

int peek(){
    int highestPriority = 10000;
    int ind = -1;

    for (int i = 0; i <= qLen; i++) {
		int priority = open[priorityq[i]].gCost+open[priorityq[i]].hCost;
        if (highestPriority > priority) {
            highestPriority = priority;
            ind = i;
        }
    }

    return ind;
}

int dequeue(){
    int ind = peek();
	int ret = priorityq[ind];
    for (int i = ind; i < qLen-1; i++) {
        priorityq[i] = priorityq[i + 1];
    }
    qLen--;

	return ret;
}

void neighborFunction(int neighborx, int neighbory, int neighborCost, struct node *current, int height){
	struct node *neighbor;
	int index = neighborx * height + neighbory;
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
		open[index].x = neighborx;
		open[index].y = neighbory;
		open[index].gCost = neighborCost;
		open[index].hCost = (abs(neighborx - endX) * 10) + 
						(abs(neighbory - endY) * 10);
		open[index].stored = 1;
		enqueue(index);
		openLen++;
	} else if(neighborCost >= neighbor->gCost){
		return;
	} else {
		neighbor->gCost = neighborCost;
		neighbor->parent = current;
	}
}

int path(int **grid, SDL_Event event, int t, int width, int height){
	int ret = PATH;

	if(t == 1){
		//srand(time(NULL)); //Generate random numbers

		startX = 0;
		startY = 0;
		endX = width-1;
		endY = height-1;

		//for(int i = 0; i < width; i++){
		//	for(int n = 0; n < height; n++){
				//grid[i][n] = EMPTY;
				//if(!(i == startNodeX && n == startNodeY) && 
				//!(i == endNodeX && n == endNodeY) && rand() % chance == 0){
				//	grid[i][n] = WALL;
				//}
		//	}
		//}

		closedLen = 0;
		closed = (struct node *) calloc(width*height, sizeof(struct node));

		openLen = 1;
		int index = startX*height+startY;
		open = (struct node *) calloc(width*height, sizeof(struct node));

		open[index].parent = NULL;
		open[index].x = startX;
		open[index].y = startY;
		open[index].gCost = 0;
		open[index].hCost = (abs(startX - endX) * 10) + 
					(abs(startY - endY) * 10);
		open[index].stored = 1;

		priorityq = (int *) calloc(width*height, sizeof(int));
		enqueue(index);
	}

	//Make sure not to replace walls or the final path
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			if(i == startX && j == startY){
				//Place Start
				grid[i][j] = EMPTY;
			} else if (i == endX && j == endY){
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
	if(open[current].x == endX && open[current].y == endY){
		struct node *currentNode = &(open[current]);
		while(currentNode){
			grid[currentNode->x][currentNode->y] = FINAL;
			currentNode = currentNode->parent;
		}
		ret = ARCADE;
	}

	//Add current to closed set
	int closedIndex = open[current].x * height + open[current].y;
	closed[closedIndex] = open[current];
	int closedx = closed[closedIndex].x;
	int closedy = closed[closedIndex].y;
	int closedcost = closed[closedIndex].gCost;
	struct node *closedNew = &(closed[closedIndex]);
	closedLen++;

	open[current].stored = 0;
	openLen--;

	//for upper neighbor
	if(closedy > 0 && grid[closedx][closedy-1] != WALL){
		neighborFunction(closedx, closedy-1, closedcost+10, closedNew, height);
	}
	//for lower neighbor
	if(closedy < height-1 && grid[closedx][closedy+1] != WALL){
		neighborFunction(closedx, closedy+1, closedcost+10, closedNew, height);
	}
	//for left neighbor
	if(closedx > 0 && grid[closedx-1][closedy] != WALL){
		neighborFunction(closedx-1, closedy, closedcost+10, closedNew, height);
	}
	//for right neighbor
	if(closedx < width-1 && grid[closedx+1][closedy] != WALL){
		neighborFunction(closedx+1, closedy, closedcost+10, closedNew, height);
	}
	//for upper left neighbor
	if(closedy > 0 && closedx > 0 && grid[closedx-1][closedy-1] != WALL){
		neighborFunction(closedx-1, closedy-1, closedcost+14, closedNew, height);
	}
	//for upper right neighbor
	if(closedy > 0 && closedx < width-1 && grid[closedx+1][closedy-1] != WALL){
		neighborFunction(closedx+1, closedy-1, closedcost+14, closedNew, height);
	}
	//for lower left neighbor
	if(closedy < height-1 && closedx > 0 && grid[closedx-1][closedy+1] != WALL){
		neighborFunction(closedx-1, closedy+1, closedcost+14, closedNew, height);
	}
	//for lower right neighbor
	if(closedy < height-1 && closedx < width-1 && grid[closedx+1][closedy+1] != WALL){
		neighborFunction(closedx+1, closedy+1, closedcost+14, closedNew, height);
	}

	//Fill Display
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			if(i == startX && j == startY){
				//Place Start
				grid[i][j] = START;
			} else if (i == endX && j == endY){
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

	if(openLen == 0){
		ret = ARCADE;
	}

	//Memory Cleanup
	if(ret == ARCADE){
		free(open);
		free(closed);
		free(priorityq);
	}

	return ret;
}
