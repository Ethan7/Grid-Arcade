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
int **ship_grid;
int cannonballs;
int battle1(int **grid, SDL_Event event, int t, int cellsize, int width, int height){
	int ret = BATTLE1;

	if(t == 1){
		srand(time(NULL));
		cannonballs = width*height/3; //Only enough cannonballs to test 1/3 of the spaces
		//Allocate ship_grid
		ship_grid = (int **) calloc(width, sizeof(int *));
		for(int i = 0; i < width; i++){
			ship_grid[i] = (int *) calloc(height, sizeof(int));
		}
		const int ship_count = sqrt(width*height)/2; //Shipcount based on gridsize, the more ships the bigger they get
		int x, y, dir;
		//Place all the ships
		for(int i = 0; i < ship_count; i++){
			const int ship_size = i / 2 + 2; //Place increasingly larger ships
			int ship_unplaced = 1;
			//Loop until the current ship has been placed without overlapping other ships or running offscreen
			while(ship_unplaced){
				//Choose random ship position and direction
				x = rand() % width;
				y = rand() % height;
				dir = rand() % 2;
				for(int j = 0; j < ship_size; j++){
					if(j > 0){
						if(dir == 0 && x + j >= width){
							break;
						} else if(dir == 1 && y + j >= height){
							break;
						} else if(dir == 0 && ship_grid[x+j][y] == SHIP){
							break;
						} else if(dir == 1 && ship_grid[x][y+j] == SHIP){
							break;
						}
					} else if(ship_grid[x][y] == SHIP){
						break;
					}
					//Place current ship
					if(j == ship_size-1){
						for(int k = 0; k < ship_size; k++){
							if(dir == 0){
								ship_grid[x+k][y] = SHIP;
							} else {
								ship_grid[x][y+k] = SHIP;
							}
						}
						ship_unplaced = 0;
					}
				}
			}
		}
	}

	//Respond to user input
	if(event.type == SDL_MOUSEBUTTONUP){
		int button_x = event.button.x / cellsize;
		int button_y = event.button.y / cellsize;
		if(event.button.button == SDL_BUTTON_LEFT && button_x < width && button_y < height && button_x > -1 && button_y > -1 && grid[button_x][button_y] == EMPTY){
			grid[button_x][button_y] = ship_grid[button_x][button_y];
			cannonballs--;
			if(grid[button_x][button_y] == SHIP){
				printf("HIT! %d cannonballs left.\n", cannonballs);
			} else {
				printf("MISS! %d cannonballs left.\n", cannonballs);
			}
			//check if all ships have been found
			int winflag = 1;
			for(int i = 0; i < width; i++){
				for(int j = 0; j < height; j++){
					if(ship_grid[i][j] == SHIP && grid[i][j] == EMPTY){
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
						if(ship_grid[i][j] == SHIP){
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
			free(ship_grid[i]);
		}
		free(ship_grid);
	}

	return ret;
}