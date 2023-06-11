/* By Ethan Hughes */
/* Written 12/2/2018 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"arcade-defs.h"
/*
#define ARCADE 0
#define MAZES 3

#define EMPTY -1
#define WALL 0
#define PLAYER 1
#define START 2
#define END 3
*/
struct wall{
	int x, y; //Position for this wall
	int h, v; //Directional offset for this wall
} *walllist;

int px = 1, py = 1, tempwidth = 3, tempheight = 3, wallcount = 2, fastmaze = 0;
int mazes(int **grid, SDL_Event event, int t, int width, int height){
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
		walllist = (struct wall *) calloc(tempwidth*tempheight/4, sizeof(struct wall));
		walllist[0].x = 1;
		walllist[0].y = 2;
		walllist[0].h = 0;
		walllist[0].v = 1;
		walllist[1].x = 2;
		walllist[1].y = 1;
		walllist[1].h = 1;
		walllist[1].v = 0;
		wallcount = 2;

		srand(time(NULL));
	}

	//Generate maze with Prim's algorithm
	do{
		if(wallcount > 0){
			//Pick random wall from wall list
			int random = rand() % (wallcount);
			if(random > 0){
				random--;
			}

			//If current wall can be placed
			if(grid[walllist[random].x+walllist[random].h][walllist[random].y+walllist[random].v] == WALL){
				//Place current wall
				grid[walllist[random].x][walllist[random].y] = EMPTY;
				grid[walllist[random].x+walllist[random].h][walllist[random].y+walllist[random].v] = EMPTY;

				//Add new potential walls to wall list
				if(walllist[random].x > 2 && grid[walllist[random].x+walllist[random].h-2][walllist[random].y+walllist[random].v] == WALL){
					walllist[wallcount].x = walllist[random].x+walllist[random].h-1;
					walllist[wallcount].y = walllist[random].y+walllist[random].v;
					walllist[wallcount].h = -1;
					walllist[wallcount].v = 0;
					wallcount++;
				}
				if(walllist[random].x < tempwidth-3 && grid[walllist[random].x+walllist[random].h+2][walllist[random].y+walllist[random].v] == WALL){
					walllist[wallcount].x = walllist[random].x+walllist[random].h+1;
					walllist[wallcount].y = walllist[random].y+walllist[random].v;
					walllist[wallcount].h = 1;
					walllist[wallcount].v = 0;
					wallcount++;
				}
				if(walllist[random].y > 2 && grid[walllist[random].x+walllist[random].h][walllist[random].y+walllist[random].v-2] == WALL){
					walllist[wallcount].x = walllist[random].x+walllist[random].h;
					walllist[wallcount].y = walllist[random].y+walllist[random].v-1;
					walllist[wallcount].h = 0;
					walllist[wallcount].v = -1;
					wallcount++;
				}
				if(walllist[random].y < tempheight-3 && grid[walllist[random].x+walllist[random].h][walllist[random].y+walllist[random].v+2] == WALL){
					walllist[wallcount].x = walllist[random].x+walllist[random].h;
					walllist[wallcount].y = walllist[random].y+walllist[random].v+1;
					walllist[wallcount].h = 0;
					walllist[wallcount].v = 1;
					wallcount++;
				}
			}

			//Clear current wall and shrink wall list
			for(int i = random; i < wallcount; i++){
				walllist[i] = walllist[i+1];
			}
			wallcount--;
		}
	} while (fastmaze && wallcount > 0);

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
		case SDLK_RETURN:
			fastmaze = 1;
		}
	}

	//Display start, player, and end point
	grid[1][1] = START;
	if(wallcount == 0){
		fastmaze = 0;
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
		free(walllist);
	}

	return ret;
}
