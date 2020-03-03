/* By Ethan Hughes */
/* Written 12/2/2018 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define ARCADE 0
#define MAZES 3

#define EMPTY -1
#define WALL 0
#define PLAYER 1
#define START 2
#define END 3

struct wall{
	int x, y; //Position for this wall
	int h, v; //Directional offset for this wall
	struct wall *next; //Next wall on the list
} *first, *list, *last, *prev;

int px = 1, py = 1, tempwidth = 3, tempheight = 3, walllist = 2;
int mazes(int **grid, SDL_Event event, int game, int t, int width, int height){
	int ret = MAZES;

	if(t == 1){
		px = 1;
		py = 1;
		tempwidth = width;
		tempheight = height;
		if(width % 2 == 0){
			tempwidth--;
		}
		if(height % 2 == 0){
			tempheight--;
		}

		int i = 0, j = 0;
		for(i = 0; i < tempwidth; i++){
			for(j = 0; j < tempheight; j++){
				grid[i][j] = WALL;
			}
		}
		grid[1][1] = EMPTY;

		//Generate initial two walls for wall list
		first = (struct wall *) calloc(1, sizeof(struct wall));
		first->next = (struct wall *) calloc(1, sizeof(struct wall));
		first->next->next = 0;
		first->x = 1;
		first->y = 2;
		first->h = 0;
		first->v = 1;
		first->next->x = 2;
		first->next->y = 1;
		first->next->h = 1;
		first->next->v = 0;
		last = first->next;
		walllist = 2;

		srand(time(NULL));
	}

	//Generate maze with Prim's algorithm
	if(first != 0){
		int random = rand()%walllist;
		if(random > 0){
			random--;
		}

		list = first; //Wall to build off of
		prev = 0;

		//Pick random wall from wall list
		for(int i = 0; i < random; i++){
			prev = list;
			list = list->next;
		}

		//If current wall can be placed
		if(grid[list->x+list->h][list->y+list->v] == WALL){
			//Place current wall
			grid[list->x][list->y] = EMPTY;
			grid[list->x+list->h][list->y+list->v] = EMPTY;

			//Add new potential walls to wall list
			if(list->x > 2 && grid[list->x+list->h-2][list->y+list->v] == WALL){
				last->next = (struct wall *) calloc(1, sizeof(struct wall));
				last->next->x = list->x+list->h-1;
				last->next->y = list->y+list->v;
				last->next->h = -1;
				last->next->v = 0;
				last->next->next = 0;
				walllist++;
				last = last->next;
			}
			if(list->x < tempwidth-3 && grid[list->x+list->h+2][list->y+list->v] == WALL){
				last->next = (struct wall *) calloc(1, sizeof(struct wall));
				last->next->x = list->x+list->h+1;
				last->next->y = list->y+list->v;
				last->next->h = 1;
				last->next->v = 0;
				last->next->next = 0;
				walllist++;
				last = last->next;
			}
			if(list->y > 2 && grid[list->x+list->h][list->y+list->v-2] == WALL){
				last->next = (struct wall *) calloc(1, sizeof(struct wall));
				last->next->x = list->x+list->h;
				last->next->y = list->y+list->v-1;
				last->next->h = 0;
				last->next->v = -1;
				last->next->next = 0;
				walllist++;
				last = last->next;
			}
			if(list->y < tempheight-3 && grid[list->x+list->h][list->y+list->v+2] == WALL){
				last->next = (struct wall *) calloc(1, sizeof(struct wall));
				last->next->x = list->x+list->h;
				last->next->y = list->y+list->v+1;
				last->next->h = 0;
				last->next->v = 1;
				last->next->next = 0;
				walllist++;
				last = last->next;
			}
		}

		//Clear current wall and shrink wall list
		if(prev){
			prev->next = list->next;
		} else {
			first = list->next;
		}
		free(list);
		walllist--;
	}

	//Allow player to traverse the maze
	if(event.type == SDL_KEYDOWN){
		switch( event.key.keysym.sym ){
		case SDLK_UP:
			if(grid[px][py-1] != WALL){
				grid[px][py] = EMPTY;
				py -= 1;
			}
			break;
		case SDLK_DOWN:
			if(grid[px][py+1] != WALL){
				grid[px][py] = EMPTY;
				py += 1;
			}
			break;
		case SDLK_LEFT:
			if(grid[px-1][py] != WALL){
				grid[px][py] = EMPTY;
				px -= 1;
			}
			break;
		case SDLK_RIGHT:
			if(grid[px+1][py] != WALL){
				grid[px][py] = EMPTY;
				px += 1;
			}
			break;
		}
	}

	//Display start, player, and end point
	grid[1][1] = START;
	if(first == 0){
		grid[tempwidth-2][tempheight-2] = END;
	}
	grid[px][py] = PLAYER;

	//Check if player has completed maze
	if(px == tempwidth-2 && py == tempheight-2){
		printf("YOU WIN!\n");
		printf("Time to beat: %d\n", t);
		ret = ARCADE;
	}

	//Memory Cleanup
	if(ret == ARCADE){
		while(first){
			list = first->next;
			free(first);
			first = list;
			walllist--;
		}
	}

	return ret;
}
