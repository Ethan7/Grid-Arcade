/* By Ethan Hughes */
/* Written 8/12/2022 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include"arcade-defs.h"
/*
#define ARCADE 0
#define TRON 20

#define EMPTY -1
#define FOOD 0
#define TRON1 1
#define TRON2 2
*/
int tail_length1, tail_length2, dir1, dir2, current_x1, current_y1, current_x2, current_y2;
int **tron_tail1, **tron_tail2;

int tron(int **grid, SDL_Event event, int t, int width, int height, int wrap){
	int ret = TRON;
	const int win_length = width*height;
	const int food_count = sqrt(width*height)/2;

	//Setup initial state
	if(t == 1){
		tail_length1 = 1;
		dir1 = 0;
		current_x1 = width/3, current_y1 = height/2;
		
		tail_length2 = 1;
		dir2 = 0;
		current_x2 = width/3*2, current_y2 = height/2;

		for(int i = 0; i < width; i++){
			for(int j = 0; j < height; j++){
				grid[i][j] = EMPTY;
			}
		}

		grid[current_x1][current_y1] = TRON1;
		grid[current_x2][current_y2] = TRON2;

		srand(time(NULL));
		
		int rx, ry;
		for(int i = 0; i < food_count; i++){
			while(1){
				rx = rand() % width;
				ry = rand() % height;
				if(grid[rx][ry] == EMPTY){
					grid[rx][ry] = FOOD;
					break;
				}
			}
		}

		tron_tail1 = (int **) calloc(win_length, sizeof(int *));
		for(int i = 0; i < win_length; i++){
			tron_tail1[i] = (int *) calloc(2, sizeof(int));
			tron_tail1[i][0] = current_x1;
			tron_tail1[i][1] = current_y1;
		}

		tron_tail2 = (int **) calloc(win_length, sizeof(int *));
		for(int i = 0; i < win_length; i++){
			tron_tail2[i] = (int *) calloc(2, sizeof(int));
			tron_tail2[i][0] = current_x2;
			tron_tail2[i][1] = current_y2;
		}
	}

	//Respond to input
	switch( event.key.keysym.sym ){
	case SDLK_UP:
		if(tail_length1 == 1 || tron_tail1[tail_length1-2][1] != current_y1-1){
			dir1 = 0;
		}
		break;
	case SDLK_DOWN:
		if(tail_length1 == 1 || tron_tail1[tail_length1-2][1] != current_y1+1){
			dir1 = 1;
		}
		break;
	case SDLK_LEFT:
		if(tail_length1 == 1 || tron_tail1[tail_length1-2][0] != current_x1-1){
			dir1 = 2;
		}
		break;
	case SDLK_RIGHT:
		if(tail_length1 == 1 || tron_tail1[tail_length1-2][0] != current_x1+1){
			dir1 = 3;
		}
		break;
	case SDLK_w:
		if(tail_length2 == 1 || tron_tail2[tail_length2-2][1] != current_y2-1){
			dir2 = 0;
		}
		break;
	case SDLK_s:
		if(tail_length2 == 1 || tron_tail2[tail_length2-2][1] != current_y2+1){
			dir2 = 1;
		}
		break;
	case SDLK_a:
		if(tail_length2 == 1 || tron_tail2[tail_length2-2][0] != current_x2-1){
			dir2 = 2;
		}
		break;
	case SDLK_d:
		if(tail_length2 == 1 || tron_tail2[tail_length2-2][0] != current_x2+1){
			dir2 = 3;
		}
		break;
	}

	//Game Step
	switch(dir1){
	case 0:
		if(current_y1 != 0){
			current_y1--;
		} else {
			if(wrap){
				current_y1 = height-1;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	case 1:
		if(current_y1 != height-1){
			current_y1++;
		} else {
			if(wrap){
				current_y1 = 0;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	case 2:
		if(current_x1 != 0){
			current_x1--;
		} else {
			if(wrap){
				current_x1 = width-1;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	case 3:
		if(current_x1 != width-1){
			current_x1++;
		} else {
			if(wrap){
				current_x1 = 0;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	}

	switch(dir2){
	case 0:
		if(current_y2 != 0){
			current_y2--;
		} else {
			if(wrap){
				current_y2 = height-1;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	case 1:
		if(current_y2 != height-1){
			current_y2++;
		} else {
			if(wrap){
				current_y2 = 0;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	case 2:
		if(current_x2 != 0){
			current_x2--;
		} else {
			if(wrap){
				current_x2 = width-1;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	case 3:
		if(current_x2 != width-1){
			current_x2++;
		} else {
			if(wrap){
				current_x2 = 0;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	}

	//Snake Movement
	if(ret == TRON){
		if(grid[current_x1][current_y1] == EMPTY){
			grid[current_x1][current_y1] = TRON1;
			grid[tron_tail1[0][0]][tron_tail1[0][1]] = EMPTY;
			for(int i = 0; i < tail_length1-1; i++){
				tron_tail1[i][0] = tron_tail1[i+1][0];
				tron_tail1[i][1] = tron_tail1[i+1][1];
			}
			tron_tail1[tail_length1-1][0] = current_x1;
			tron_tail1[tail_length1-1][1] = current_y1;

		} else if(grid[current_x1][current_y1] == FOOD){
			tail_length1++;
			grid[current_x1][current_y1] = TRON1;

			tron_tail1[tail_length1-1][0] = current_x1;
			tron_tail1[tail_length1-1][1] = current_y1;
			int rx, ry;
			while(1){
				rx = rand() % width;
				ry = rand() % height;
				if(grid[rx][ry] == EMPTY){
					grid[rx][ry] = FOOD;
					break;
				}
			}
		} else if(grid[current_x1][current_y1] >= TRON1){
			printf("Player 1 Loses!\n");
			ret = ARCADE;
		}
		
		if(grid[current_x2][current_y2] == EMPTY){
			grid[current_x2][current_y2] = TRON2;
			grid[tron_tail2[0][0]][tron_tail2[0][1]] = EMPTY;
			for(int i = 0; i < tail_length2-1; i++){
				tron_tail2[i][0] = tron_tail2[i+1][0];
				tron_tail2[i][1] = tron_tail2[i+1][1];
			}
			tron_tail2[tail_length2-1][0] = current_x2;
			tron_tail2[tail_length2-1][1] = current_y2;

		} else if(grid[current_x2][current_y2] == FOOD){
			tail_length2++;
			grid[current_x2][current_y2] = TRON2;

			tron_tail2[tail_length2-1][0] = current_x2;
			tron_tail2[tail_length2-1][1] = current_y2;
			int rx, ry;
			while(1){
				rx = rand() % width;
				ry = rand() % height;
				if(grid[rx][ry] == EMPTY){
					grid[rx][ry] = FOOD;
					break;
				}
			}
		} else if(grid[current_x2][current_y2] >= TRON1){
			printf("Player 2 Loses!\n");
			ret = ARCADE;
		}
	}

	//Free Dynamic Memory
	if(ret == ARCADE){
		for(int i = 0; i < win_length; i++){
			free(tron_tail1[i]);
			free(tron_tail2[i]);
		}
		free(tron_tail1);
		free(tron_tail2);
	}

	return ret;
}
