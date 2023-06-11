/* By Ethan Hughes */
/* Written 12/1/2018 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include"arcade-defs.h"
/*
#define ARCADE 0
#define SNAKE 1

#define EMPTY -1
#define FOOD 0
#define SNAKE 1
*/
int tailLen, dir, snakeX, snakeY;
int **snakeTail;

int snake(int **grid, SDL_Event event, int t, int width, int height, int wrap){
	int ret = SNAKE;
	int winLen = (width*height) >> (2-wrap);
	int foodcount = sqrt(width*height)/2;

	//Setup initial state
	if(t == 1){
		tailLen = 1;
		dir = 0;
		snakeX = width/2, snakeY = height/2;

		for(int i = 0; i < width; i++){
			for(int j = 0; j < height; j++){
				grid[i][j] = EMPTY;
			}
		}

		grid[snakeX][snakeY] = SNAKE;

		srand(time(NULL));
		
		for(int i = 0; i < foodcount; i++){
			while(1){
				int rx = rand() % width;
				int ry = rand() % height;
				if(grid[rx][ry] == EMPTY){
					grid[rx][ry] = FOOD;
					break;
				}
			}
		}

		snakeTail = (int **) calloc(winLen, sizeof(int *));
		for(int i = 0; i < winLen; i++){
			snakeTail[i] = (int *) calloc(2, sizeof(int));
			snakeTail[i][0] = snakeX;
			snakeTail[i][1] = snakeY;
		}
	}

	//Respond to input
	switch( event.key.keysym.sym ){
	case SDLK_UP:
		if(tailLen == 1 || snakeTail[tailLen-2][1] != snakeY-1){
			dir = 0;
		}
		break;
	case SDLK_DOWN:
		if(tailLen == 1 || snakeTail[tailLen-2][1] != snakeY+1){
			dir = 1;
		}
		break;
	case SDLK_LEFT:
		if(tailLen == 1 || snakeTail[tailLen-2][0] != snakeX-1){
			dir = 2;
		}
		break;
	case SDLK_RIGHT:
		if(tailLen == 1 || snakeTail[tailLen-2][0] != snakeX+1){
			dir = 3;
		}
		break;
	}

	//Game Step
	switch(dir){
	case 0:
		if(snakeY != 0){
			snakeY--;
		} else {
			if(wrap){
				snakeY = height-1;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	case 1:
		if(snakeY != height-1){
			snakeY++;
		} else {
			if(wrap){
				snakeY = 0;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	case 2:
		if(snakeX != 0){
			snakeX--;
		} else {
			if(wrap){
				snakeX = width-1;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	case 3:
		if(snakeX != width-1){
			snakeX++;
		} else {
			if(wrap){
				snakeX = 0;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	}

	//Snake Movement
	if(ret == SNAKE){
		if(grid[snakeX][snakeY] == EMPTY){
			grid[snakeX][snakeY] = SNAKE;
			grid[snakeTail[0][0]][snakeTail[0][1]] = EMPTY;
			for(int i = 0; i < tailLen-1; i++){
				snakeTail[i][0] = snakeTail[i+1][0];
				snakeTail[i][1] = snakeTail[i+1][1];
			}
			snakeTail[tailLen-1][0] = snakeX;
			snakeTail[tailLen-1][1] = snakeY;

		} else if(grid[snakeX][snakeY] == FOOD){
			tailLen++;
			grid[snakeX][snakeY] = SNAKE;

			snakeTail[tailLen-1][0] = snakeX;
			snakeTail[tailLen-1][1] = snakeY;
			while(1){
				int rx = rand() % width;
				int ry = rand() % height;
				if(grid[rx][ry] == EMPTY){
					grid[rx][ry] = FOOD;
					break;
				}
			}
		} else if(grid[snakeX][snakeY] == SNAKE){
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
