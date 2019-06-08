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

static struct wall{
	int x, y; //Position for this wall
	int h, v; //Directional offset for this wall
	struct wall *next; //Next wall on the list
} *walls, *list, *list2, *prev, *afte;

static int r = 0;

int randomizer(){
	srand(time(NULL)+r);
	r++;
	return rand();
}

static int px = 1, py = 1, tempwidth = 3, tempheight = 3;
int mazes(int **grid, SDL_Event event, int game, int t, int width, int height){

	if(t == 1){
		px = 1;
		py = 1;
		tempwidth = width;
		tempheight = height;
		if(width % 2 == 0){
			tempwidth--;
			tempheight--;
		}

		int i = 0, j = 0, random = 0;
		for(i = 0; i < tempwidth; i++){
			for(j = 0; j < tempheight; j++){
				grid[i][j] = WALL;
			}
		}
		grid[1][1] = EMPTY;

		walls = (struct wall *) malloc(sizeof(struct wall));
		walls->next = (struct wall *) malloc(sizeof(struct wall));
		walls->next->next = 0;
		walls->x = 1;
		walls->y = 2;
		walls->h = 0;
		walls->v = 1;
		walls->next->x = 2;
		walls->next->y = 1;
		walls->next->h = 1;
		walls->next->v = 0;
		int walllist = 2;

		while(walls != 0){
			random = randomizer()%walllist;
			list = walls;
			prev = 0;
			afte = list->next;
			for(i = 0; i < random; i++){
				prev = list;
				list = list->next;
				afte = list->next;
			}
			if(grid[list->x+list->h][list->y+list->v] == WALL){
				grid[list->x][list->y] = EMPTY;
				grid[list->x+list->h][list->y+list->v] = EMPTY;
				if(list->x > 2 && grid[list->x+list->h-2][list->y+list->v] == WALL){
					list2 = walls;
					while(list2->next != 0){
						list2 = list2->next;
					}
					list2->next = (struct wall *) malloc(sizeof(struct wall));
					list2->next->x = list->x+list->h-1;
					list2->next->y = list->y+list->v;
					list2->next->h = -1;
					list2->next->v = 0;
					list2->next->next = 0;
					walllist++;
				}
				if(list->x < tempwidth-3 && grid[list->x+list->h+2][list->y+list->v] == WALL){
					list2 = walls;
					while(list2->next != 0){
						list2 = list2->next;
					}
					list2->next = (struct wall *) malloc(sizeof(struct wall));
					list2->next->x = list->x+list->h+1;
					list2->next->y = list->y+list->v;
					list2->next->h = 1;
					list2->next->v = 0;
					list2->next->next = 0;
					walllist++;
				}
				if(list->y > 2 && grid[list->x+list->h][list->y+list->v-2] == WALL){
					list2 = walls;
					while(list2->next != 0){
						list2 = list2->next;
					}
					list2->next = (struct wall *) malloc(sizeof(struct wall));
					list2->next->x = list->x+list->h;
					list2->next->y = list->y+list->v-1;
					list2->next->h = 0;
					list2->next->v = -1;
					list2->next->next = 0;
					walllist++;
				}
				if(list->y < tempheight-3 && grid[list->x+list->h][list->y+list->v+2] == WALL){
					list2 = walls;
					while(list2->next != 0){
						list2 = list2->next;
					}
					list2->next = (struct wall *) malloc(sizeof(struct wall));
					list2->next->x = list->x+list->h;
					list2->next->y = list->y+list->v+1;
					list2->next->h = 0;
					list2->next->v = 1;
					list2->next->next = 0;
					walllist++;
				}
			}
			if(prev){
				if(afte){
					prev->next = afte;
				} else {
					list2 = walls;
					while(list2 != list){
						list2 = list2->next;
					}
					prev->next = list2->next;
				}
			} else {
				walls = afte;
			}
			free(list);
			walllist--;
		}
	}

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

	grid[1][1] = START;
	grid[tempwidth-2][tempheight-2] = END;
	grid[px][py] = PLAYER;

	if(px == tempwidth-2 && py == tempheight-2){
		printf("YOU WIN!\n");
		printf("Time to beat: %d\n", t);
		return ARCADE;
	}

	return MAZES;
}
