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
int tail_length, dir, snake_x, snake_y;
int **snake_tail;

int snake(int **grid, SDL_Event event, int t, int width, int height, int wrap){
	int ret = SNAKE;
	const int win_length = (width*height) >> (2-wrap);
	const int food_count = sqrt(width*height)/2;

	//Setup initial state
	if(t == 1){
		tail_length = 1;
		dir = 0;
		snake_x = width/2, snake_y = height/2;

		for(int i = 0; i < width; i++){
			for(int j = 0; j < height; j++){
				grid[i][j] = EMPTY;
			}
		}

		grid[snake_x][snake_y] = SNAKE;

		srand(time(NULL));
		
		for(int i = 0; i < food_count; i++){
			while(1){
				int rx = rand() % width;
				int ry = rand() % height;
				if(grid[rx][ry] == EMPTY){
					grid[rx][ry] = FOOD;
					break;
				}
			}
		}

		snake_tail = (int **) calloc(win_length, sizeof(int *));
		for(int i = 0; i < win_length; i++){
			snake_tail[i] = (int *) calloc(2, sizeof(int));
			snake_tail[i][0] = snake_x;
			snake_tail[i][1] = snake_y;
		}
	}

	//Respond to input
	switch( event.key.keysym.sym ){
	case SDLK_UP:
		if(tail_length == 1 || snake_tail[tail_length-2][1] != snake_y-1){
			dir = 0;
		}
		break;
	case SDLK_DOWN:
		if(tail_length == 1 || snake_tail[tail_length-2][1] != snake_y+1){
			dir = 1;
		}
		break;
	case SDLK_LEFT:
		if(tail_length == 1 || snake_tail[tail_length-2][0] != snake_x-1){
			dir = 2;
		}
		break;
	case SDLK_RIGHT:
		if(tail_length == 1 || snake_tail[tail_length-2][0] != snake_x+1){
			dir = 3;
		}
		break;
	}

	//Game Step
	switch(dir){
	case 0:
		if(snake_y != 0){
			snake_y--;
		} else {
			if(wrap){
				snake_y = height-1;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	case 1:
		if(snake_y != height-1){
			snake_y++;
		} else {
			if(wrap){
				snake_y = 0;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	case 2:
		if(snake_x != 0){
			snake_x--;
		} else {
			if(wrap){
				snake_x = width-1;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	case 3:
		if(snake_x != width-1){
			snake_x++;
		} else {
			if(wrap){
				snake_x = 0;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	}

	//Snake Movement
	if(ret == SNAKE){
		if(grid[snake_x][snake_y] == EMPTY){
			grid[snake_x][snake_y] = SNAKE;
			grid[snake_tail[0][0]][snake_tail[0][1]] = EMPTY;
			for(int i = 0; i < tail_length-1; i++){
				snake_tail[i][0] = snake_tail[i+1][0];
				snake_tail[i][1] = snake_tail[i+1][1];
			}
			snake_tail[tail_length-1][0] = snake_x;
			snake_tail[tail_length-1][1] = snake_y;

		} else if(grid[snake_x][snake_y] == FOOD){
			tail_length++;
			grid[snake_x][snake_y] = SNAKE;

			snake_tail[tail_length-1][0] = snake_x;
			snake_tail[tail_length-1][1] = snake_y;
			while(1){
				int rx = rand() % width;
				int ry = rand() % height;
				if(grid[rx][ry] == EMPTY){
					grid[rx][ry] = FOOD;
					break;
				}
			}
		} else if(grid[snake_x][snake_y] == SNAKE){
			printf("YOU LOSE!\n");
			ret = ARCADE;
		}

		//Win Condition
		if(tail_length == win_length){
			printf("YOU WIN!\n");
			printf("Time to beat: %d\n", t);
			ret = ARCADE;
		}
	}

	//Free Dynamic Memory
	if(ret == ARCADE){
		for(int i = 0; i < win_length; i++){
			free(snake_tail[i]);
		}
		free(snake_tail);
	}

	return ret;
}
