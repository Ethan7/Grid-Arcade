/* By Ethan Hughes */
/* Written 12/1/2018 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#define ARCADE 0
#define SNAKE 1

#define EMPTY -1
#define FOOD 0
#define SNAKE 1

static int tailLen, dir, currentX, currentY, foodcount, winLen, rx, ry;
static int **snakeTail;

int snake(int **grid, SDL_Event event, int game, int t, int width, int height, int wrap){
	int ret = SNAKE;

	//Setup initial state
	if(t == 1){
		tailLen = 1;
		dir = 0;
		currentX = width/2, currentY = height/2;
		foodcount = sqrt(width*height)/2;
		winLen = (width*height) >> (2-wrap);

		for(int i = 0; i < width; i++){
			for(int j = 0; j < height; j++){
				grid[i][j] = EMPTY;
			}
		}

		grid[currentX][currentY] = SNAKE;

		srand(time(NULL));
		
		for(int i = 0; i < foodcount; i++){
			while(1){
				rx = rand() % width;
				ry = rand() % height;
				if(grid[rx][ry] == EMPTY){
					grid[rx][ry] = FOOD;
					break;
				}
			}
		}

		snakeTail = (int **) calloc(winLen, sizeof(int *));
		for(int i = 0; i < winLen; i++){
			snakeTail[i] = (int *) calloc(2, sizeof(int));
			snakeTail[i][0] = currentX;
			snakeTail[i][1] = currentY;
		}
	}

	//Respond to input
	switch( event.key.keysym.sym ){
	case SDLK_UP:
		if(tailLen == 1 || snakeTail[tailLen-2][1] != currentY-1){
			dir = 0;
		}
		break;
	case SDLK_DOWN:
		if(tailLen == 1 || snakeTail[tailLen-2][1] != currentY+1){
			dir = 1;
		}
		break;
	case SDLK_LEFT:
		if(tailLen == 1 || snakeTail[tailLen-2][0] != currentX-1){
			dir = 2;
		}
		break;
	case SDLK_RIGHT:
		if(tailLen == 1 || snakeTail[tailLen-2][0] != currentX+1){
			dir = 3;
		}
		break;
	}

	//Game Step
	switch(dir){
	case 0:
		if(currentY != 0){
			currentY--;
		} else {
			if(wrap){
				currentY = height-1;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	case 1:
		if(currentY != height-1){
			currentY++;
		} else {
			if(wrap){
				currentY = 0;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	case 2:
		if(currentX != 0){
			currentX--;
		} else {
			if(wrap){
				currentX = width-1;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	case 3:
		if(currentX != width-1){
			currentX++;
		} else {
			if(wrap){
				currentX = 0;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	}

	//Snake Movement
	if(ret == SNAKE){
		if(grid[currentX][currentY] == EMPTY){
			grid[currentX][currentY] = SNAKE;
			grid[snakeTail[0][0]][snakeTail[0][1]] = EMPTY;
			for(int i = 0; i < tailLen-1; i++){
				snakeTail[i][0] = snakeTail[i+1][0];
				snakeTail[i][1] = snakeTail[i+1][1];
			}
			snakeTail[tailLen-1][0] = currentX;
			snakeTail[tailLen-1][1] = currentY;

		} else if(grid[currentX][currentY] == FOOD){
			tailLen++;
			grid[currentX][currentY] = SNAKE;

			snakeTail[tailLen-1][0] = currentX;
			snakeTail[tailLen-1][1] = currentY;
			while(1){
				rx = rand() % width;
				ry = rand() % height;
				if(grid[rx][ry] == EMPTY){
					grid[rx][ry] = FOOD;
					break;
				}
			}
		} else if(grid[currentX][currentY] == SNAKE){
			printf("YOU LOSE!\n");
			ret = ARCADE;
		}

		//Win Condition
		if(tailLen == winLen){
			printf("YOU WIN!\n");
			printf("Time to beat: %d\n", t);
			ret = ARCADE;
		}
	}

	//Free Dynamic Memory
	if(ret == ARCADE){
		for(int i = 0; i < winLen; i++){
			free(snakeTail[i]);
		}
		free(snakeTail);
	}

	return ret;
}
