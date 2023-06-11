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
int tailLen1, tailLen2, dir1, dir2, currentX1, currentY1, currentX2, currentY2;
int **tronTail1, **tronTail2;

int tron(int **grid, SDL_Event event, int t, int width, int height, int wrap){
	int ret = TRON;
	int winLen = width*height;
	int foodcount = sqrt(width*height)/2;

	//Setup initial state
	if(t == 1){
		tailLen1 = 1;
		dir1 = 0;
		currentX1 = width/3, currentY1 = height/2;
		
		tailLen2 = 1;
		dir2 = 0;
		currentX2 = width/3*2, currentY2 = height/2;

		for(int i = 0; i < width; i++){
			for(int j = 0; j < height; j++){
				grid[i][j] = EMPTY;
			}
		}

		grid[currentX1][currentY1] = TRON1;
		grid[currentX2][currentY2] = TRON2;

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

		tronTail1 = (int **) calloc(winLen, sizeof(int *));
		for(int i = 0; i < winLen; i++){
			tronTail1[i] = (int *) calloc(2, sizeof(int));
			tronTail1[i][0] = currentX1;
			tronTail1[i][1] = currentY1;
		}

		tronTail2 = (int **) calloc(winLen, sizeof(int *));
		for(int i = 0; i < winLen; i++){
			tronTail2[i] = (int *) calloc(2, sizeof(int));
			tronTail2[i][0] = currentX2;
			tronTail2[i][1] = currentY2;
		}
	}

	//Respond to input
	switch( event.key.keysym.sym ){
	case SDLK_UP:
		if(tailLen1 == 1 || tronTail1[tailLen1-2][1] != currentY1-1){
			dir1 = 0;
		}
		break;
	case SDLK_DOWN:
		if(tailLen1 == 1 || tronTail1[tailLen1-2][1] != currentY1+1){
			dir1 = 1;
		}
		break;
	case SDLK_LEFT:
		if(tailLen1 == 1 || tronTail1[tailLen1-2][0] != currentX1-1){
			dir1 = 2;
		}
		break;
	case SDLK_RIGHT:
		if(tailLen1 == 1 || tronTail1[tailLen1-2][0] != currentX1+1){
			dir1 = 3;
		}
		break;
	case SDLK_w:
		if(tailLen2 == 1 || tronTail2[tailLen2-2][1] != currentY2-1){
			dir2 = 0;
		}
		break;
	case SDLK_s:
		if(tailLen2 == 1 || tronTail2[tailLen2-2][1] != currentY2+1){
			dir2 = 1;
		}
		break;
	case SDLK_a:
		if(tailLen2 == 1 || tronTail2[tailLen2-2][0] != currentX2-1){
			dir2 = 2;
		}
		break;
	case SDLK_d:
		if(tailLen2 == 1 || tronTail2[tailLen2-2][0] != currentX2+1){
			dir2 = 3;
		}
		break;
	}

	//Game Step
	switch(dir1){
	case 0:
		if(currentY1 != 0){
			currentY1--;
		} else {
			if(wrap){
				currentY1 = height-1;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	case 1:
		if(currentY1 != height-1){
			currentY1++;
		} else {
			if(wrap){
				currentY1 = 0;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	case 2:
		if(currentX1 != 0){
			currentX1--;
		} else {
			if(wrap){
				currentX1 = width-1;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	case 3:
		if(currentX1 != width-1){
			currentX1++;
		} else {
			if(wrap){
				currentX1 = 0;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	}

	switch(dir2){
	case 0:
		if(currentY2 != 0){
			currentY2--;
		} else {
			if(wrap){
				currentY2 = height-1;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	case 1:
		if(currentY2 != height-1){
			currentY2++;
		} else {
			if(wrap){
				currentY2 = 0;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	case 2:
		if(currentX2 != 0){
			currentX2--;
		} else {
			if(wrap){
				currentX2 = width-1;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	case 3:
		if(currentX2 != width-1){
			currentX2++;
		} else {
			if(wrap){
				currentX2 = 0;
			} else {
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
		break;
	}

	//Snake Movement
	if(ret == TRON){
		if(grid[currentX1][currentY1] == EMPTY){
			grid[currentX1][currentY1] = TRON1;
			grid[tronTail1[0][0]][tronTail1[0][1]] = EMPTY;
			for(int i = 0; i < tailLen1-1; i++){
				tronTail1[i][0] = tronTail1[i+1][0];
				tronTail1[i][1] = tronTail1[i+1][1];
			}
			tronTail1[tailLen1-1][0] = currentX1;
			tronTail1[tailLen1-1][1] = currentY1;

		} else if(grid[currentX1][currentY1] == FOOD){
			tailLen1++;
			grid[currentX1][currentY1] = TRON1;

			tronTail1[tailLen1-1][0] = currentX1;
			tronTail1[tailLen1-1][1] = currentY1;
			while(1){
				int rx = rand() % width;
				int ry = rand() % height;
				if(grid[rx][ry] == EMPTY){
					grid[rx][ry] = FOOD;
					break;
				}
			}
		} else if(grid[currentX1][currentY1] >= TRON1){
			printf("Player 1 Loses!\n");
			ret = ARCADE;
		}
		
		if(grid[currentX2][currentY2] == EMPTY){
			grid[currentX2][currentY2] = TRON2;
			grid[tronTail2[0][0]][tronTail2[0][1]] = EMPTY;
			for(int i = 0; i < tailLen2-1; i++){
				tronTail2[i][0] = tronTail2[i+1][0];
				tronTail2[i][1] = tronTail2[i+1][1];
			}
			tronTail2[tailLen2-1][0] = currentX2;
			tronTail2[tailLen2-1][1] = currentY2;

		} else if(grid[currentX2][currentY2] == FOOD){
			tailLen2++;
			grid[currentX2][currentY2] = TRON2;

			tronTail2[tailLen2-1][0] = currentX2;
			tronTail2[tailLen2-1][1] = currentY2;
			while(1){
				int rx = rand() % width;
				int ry = rand() % height;
				if(grid[rx][ry] == EMPTY){
					grid[rx][ry] = FOOD;
					break;
				}
			}
		} else if(grid[currentX2][currentY2] >= TRON1){
			printf("Player 2 Loses!\n");
			ret = ARCADE;
		}
	}

	//Free Dynamic Memory
	if(ret == ARCADE){
		for(int i = 0; i < winLen; i++){
			free(tronTail1[i]);
			free(tronTail2[i]);
		}
		free(tronTail1);
		free(tronTail2);
	}

	return ret;
}
