/* By Ethan Hughes */
/* Written 12/5/2018 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
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
	int gCost, hCost;
	int x, y;
	struct node * parent;
} **openNodes, **closedNodes;

struct queue{ //priority queue
	struct node * item;
	struct queue * next;
	struct queue * prev;
} *priorityq;

int startNodeX = 0, startNodeY = 0, //chance = 3, 
endNodeX = 19, endNodeY = 19, openNodesLen = 0, closedNodesLen = 0;

//Add to priority queue
void enqueue(struct node * new){
	struct queue * current = priorityq;
	struct queue * last = priorityq;
	while(current != NULL){
		if(current->item->gCost+current->item->hCost > 
			new->gCost+new->hCost){
			struct queue *temp = (struct queue *) 
				calloc(1, sizeof(struct queue));
			temp->item = new;
			temp->next = current;
			if(current->prev){
				temp->prev = current->prev;
				current->prev->next = temp;
			}
			current->prev = temp;
			if(current == priorityq){
				priorityq = temp;
			}
			break;
		}
		last = current;
		current = current->next;
	}
	if(last == NULL){
		priorityq = (struct queue *) calloc(1, sizeof(struct queue));
		priorityq->item = new;
	} else if(current == NULL){
		last->next = (struct queue *) calloc(1, sizeof(struct queue));
		last->next->item = new;
		last->next->prev = last;
	}
}

//Take one node off the priority queue
struct node * dequeue(){
	if(priorityq == NULL){
		return NULL;
	}
	struct queue *temp = priorityq;
	struct node *ret = priorityq->item;
	priorityq = priorityq->next;
	if(priorityq != NULL){
		priorityq->prev = NULL;
	}
	free(temp);
	return ret;
}

/*int hasCoords(int nx, int ny, struct node ** nodes, int b){
	int i = 0;
	if(b){
		for(i = 0; i < openNodesLen; i++){
			if(nodes[i]->x == nx && nodes[i]->y == ny){
				return i+1;
			}
		}
		return 0;
	} else {
		for(i = 0; i < closedNodesLen; i++){
			if(nodes[i]->x == nx && nodes[i]->y == ny){
				return i+1;
			}
		}
		return 0;
	}
}*/

void neighborFunction(int neighborx, int neighbory, double neighborCost, struct node *current, int height){
	struct node * neighbor;
	int index = neighborx * height + neighbory;
	if(closedNodes[index] != NULL){
		return;
	}
	int b = 1;
	if(openNodes[index] != NULL){
		b = 0;
		neighbor = openNodes[index];
	}
	if(b){
		openNodes[index] = (struct node *) calloc(1, sizeof(struct node));
		openNodes[index]->x = neighborx;
		openNodes[index]->y = neighbory;
		openNodes[index]->gCost = neighborCost;
		openNodes[index]->hCost = (abs(neighborx - endNodeX) * 10) + 
						(abs(neighbory - endNodeY) * 10);
		openNodes[index]->parent = current;
		enqueue(openNodes[index]);
		openNodesLen++;
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
		srand(time(NULL)); //Generate random numbers

		startNodeX = 0;
		startNodeY = 0;
		endNodeX = width-1;
		endNodeY = height-1;

		//for(int i = 0; i < width; i++){
		//	for(int n = 0; n < height; n++){
				//grid[i][n] = EMPTY;
				//if(!(i == startNodeX && n == startNodeY) && 
				//!(i == endNodeX && n == endNodeY) && rand() % chance == 0){
				//	grid[i][n] = WALL;
				//}
		//	}
		//}

		openNodesLen = 1;
		int index = startNodeX*height+startNodeY;
		openNodes = (struct node **) calloc(width*height, sizeof(struct node *));
		openNodes[index] = (struct node *) calloc(1, sizeof(struct node));

		closedNodesLen = 0;
		closedNodes = (struct node **) calloc(width*height, sizeof(struct node *));

		openNodes[index]->parent = NULL;
		openNodes[index]->x = startNodeX;
		openNodes[index]->y = startNodeY;
		openNodes[index]->gCost = 0;
		openNodes[index]->hCost = (abs(startNodeX - endNodeX) * 10) + 
					(abs(startNodeY - endNodeY) * 10);

		priorityq = (struct queue *) calloc(1, sizeof(struct queue));
		priorityq->item = openNodes[index];
	}

	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			if(i == startNodeX && j == startNodeY){
				//Place Start
				grid[i][j] = EMPTY;
			} else if (i == endNodeX && j == endNodeY){
				//Place End
				grid[i][j] = EMPTY;
			} else if(closedNodes[i*height+j]){
				//Place Closed Node
				grid[i][j] = EMPTY;
			} else if(openNodes[i*height+j]){
				//Place Open Node
				grid[i][j] = EMPTY;
			}
		}
	}

	/*int i = 0, currenti = 0;
	for(i = 0; i < openNodesLen; i++){
		if(openNodes[i]->gCost+openNodes[i]->hCost < openNodes[currenti]->gCost+openNodes[currenti]->hCost){
			currenti = i;
		}
	}*/
	struct node * currentNode = dequeue();
	if(currentNode->x == endNodeX && currentNode->y == endNodeY){
		struct node * current = currentNode;
		while(current != NULL){
			grid[current->x][current->y] = FINAL;
			current = current->parent;
		}
		ret = ARCADE;
	}

	//Add current to closed set
	int closedIndex = currentNode->x * height + currentNode->y;
	closedNodes[closedIndex] = (struct node *) calloc(1, sizeof(struct node));
	closedNodes[closedIndex]->x = currentNode->x;
	int closedx = closedNodes[closedIndex]->x;
	closedNodes[closedIndex]->y = currentNode->y;
	int closedy = closedNodes[closedIndex]->y;
	closedNodes[closedIndex]->gCost = currentNode->gCost;
	int closedcost = closedNodes[closedIndex]->gCost;
	closedNodes[closedIndex]->hCost = currentNode->hCost;
	closedNodes[closedIndex]->parent = currentNode->parent;
	closedNodesLen++;

	//Remove current from open set
	/*i = currenti;
	free(openNodes[i]);
	openNodes[i] = openNodes[i+1];
	i++;
	while(openNodes[i] != NULL){
		openNodes[i] = openNodes[i+1];
		i++;
	}*/
	free(currentNode);
	openNodesLen--;

	//for upper neighbor
	if(closedy > 0 && grid[closedx][closedy-1] != WALL){
		neighborFunction(closedx, closedy-1, closedcost+10, closedNodes[closedIndex], height);
	}
	//for lower neighbor
	if(closedy < height-1 && grid[closedx][closedy+1] != WALL){
		neighborFunction(closedx, closedy+1, closedcost+10, closedNodes[closedIndex], height);
	}
	//for left neighbor
	if(closedx > 0 && grid[closedx-1][closedy] != WALL){
		neighborFunction(closedx-1, closedy, closedcost+10, closedNodes[closedIndex], height);
	}
	//for right neighbor
	if(closedx < width-1 && grid[closedx+1][closedy] != WALL){
		neighborFunction(closedx+1, closedy, closedcost+10, closedNodes[closedIndex], height);
	}
	//for upper left neighbor
	if(closedy > 0 && closedx > 0 && grid[closedx-1][closedy-1] != WALL){
		neighborFunction(closedx-1, closedy-1, closedcost+14, closedNodes[closedIndex], height);
	}
	//for upper right neighbor
	if(closedy > 0 && closedx < width-1 && grid[closedx+1][closedy-1] != WALL){
		neighborFunction(closedx+1, closedy-1, closedcost+14, closedNodes[closedIndex], height);
	}
	//for lower left neighbor
	if(closedy < height-1 && closedx > 0 && grid[closedx-1][closedy+1] != WALL){
		neighborFunction(closedx-1, closedy+1, closedcost+14, closedNodes[closedIndex], height);
	}
	//for lower right neighbor
	if(closedy < height-1 && closedx < width-1 && grid[closedx+1][closedy+1] != WALL){
		neighborFunction(closedx+1, closedy+1, closedcost+14, closedNodes[closedIndex], height);
	}

	//Fill Display
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			if(i == startNodeX && j == startNodeY){
				//Place Start
				grid[i][j] = START;
			} else if (i == endNodeX && j == endNodeY){
				//Place End
				grid[i][j] = END;
			} else if(grid[i][j] != FINAL && closedNodes[i*height+j]){
				//Place Closed Node
				grid[i][j] = CLOSED;
			} else if(grid[i][j] != FINAL && openNodes[i*height+j]){
				//Place Open Node
				grid[i][j] = OPEN;
			}
		}
	}

	if(openNodesLen == 0){
		ret = ARCADE;
	}

	//Memory Cleanup
	if(ret == ARCADE){
		for(int i = 0; i < openNodesLen; i++){
			free(dequeue());
		}
		free(openNodes);
		for(int i = 0; i < width*height; i++){
			if(closedNodes[i] != NULL){
				free(closedNodes[i]);
			}
		}
		free(closedNodes);
	}

	return ret;
}
