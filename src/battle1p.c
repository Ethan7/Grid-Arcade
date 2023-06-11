/* By Ethan Hughes */
/* Written 4/26/2021 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include"arcade-defs.h"
/*
#define ARCADE 0
#define BATTLE1 15

#define EMPTY -1
#define SHIP 1
#define CLEAR 0
*/
int **shipgrid;
int cannonballs;
int battle1(int **grid, SDL_Event eventbutton, int t, int cellsize, int width, int height){
	int ret = BATTLE1;

	if(t == 1){
		srand(time(NULL));
		cannonballs = width*height/3; //Only enough cannonballs to test 1/3 of the spaces
		//Allocate shipgrid
		shipgrid = (int **) calloc(width, sizeof(int *));
		for(int i = 0; i < width; i++){
			shipgrid[i] = (int *) calloc(height, sizeof(int));
		}
		int shipcount = sqrt(width*height)/2; //Shipcount based on gridsize, the more ships the bigger they get
		//Place all the ships
		for(int i = 0; i < shipcount; i++){
			int shipsize = i / 2 + 2; //Place increasingly larger ships
			int shipunplaced = 1;
			//Loop until the current ship has been placed without overlapping other ships or running offscreen
			while(shipunplaced){
				//Choose random ship position and direction
				int x = rand() % width;
				int y = rand() % height;
				int dir = rand() % 2;
				for(int j = 0; j < shipsize; j++){
					if(j > 0){
						if(dir == 0 && x + j >= width){
							break;
						} else if(dir == 1 && y + j >= height){
							break;
						} else if(dir == 0 && shipgrid[x+j][y] == SHIP){
							break;
						} else if(dir == 1 && shipgrid[x][y+j] == SHIP){
							break;
						}
					} else if(shipgrid[x][y] == SHIP){
						break;
					}
					//Place current ship
					if(j == shipsize-1){
						for(int k = 0; k < shipsize; k++){
							if(dir == 0){
								shipgrid[x+k][y] = SHIP;
							} else {
								shipgrid[x][y+k] = SHIP;
							}
						}
						shipunplaced = 0;
					}
				}
			}
		}
	}

	//Respond to user input
	if(eventbutton.type == SDL_MOUSEBUTTONUP){
		int buttonx = eventbutton.button.x / cellsize;
		int buttony = eventbutton.button.y / cellsize;
		if(eventbutton.button.button == SDL_BUTTON_LEFT && buttonx < width && buttony < height && buttonx > -1 && buttony > -1 && grid[buttonx][buttony] == EMPTY){
			grid[buttonx][buttony] = shipgrid[buttonx][buttony];
			cannonballs--;
			if(grid[buttonx][buttony] == SHIP){
				printf("HIT! %d cannonballs left.\n", cannonballs);
			} else {
				printf("MISS! %d cannonballs left.\n", cannonballs);
			}
			//check if all ships have been found
			int winflag = 1;
			for(int i = 0; i < width; i++){
				for(int j = 0; j < height; j++){
					if(shipgrid[i][j] == SHIP && grid[i][j] == EMPTY){
						winflag = 0;
					}
				}
			}
			if(winflag == 1){
				printf("YOU WIN!\n");
				ret = ARCADE;
			} else if(cannonballs == 0){
				for(int i = 0; i < width; i++){
					for(int j = 0; j < height; j++){
						if(shipgrid[i][j] == SHIP){
							grid[i][j] = SHIP;
						}
					}
				}
				printf("YOU LOSE!\n");
				ret = ARCADE;
			}
		}
	}

	//Free Dynamic Memory
	if(ret == ARCADE){
		for(int i = 0; i < width; i++){
			free(shipgrid[i]);
		}
		free(shipgrid);
	}

	return ret;
}