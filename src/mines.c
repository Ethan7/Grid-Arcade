/* By Ethan Hughes */
/* Written 11/13/2019 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"arcade-defs.h"
/*
#define ARCADE 0
#define MINES 13

#define EMPTY -1
#define MINE 1
#define FLAG 4
#define CLEAR 0

//Number defines
#define ONE 8
#define TWO 9
#define THREE 10
#define FOUR 11
#define FIVE 12
#define SIX 13
#define SEVEN 14
#define EIGHT 15
*/
//Recursive Flood-fill algorithm
void sweep(int button_x, int button_y, int **mine_grid, int **grid, int width, int height){
	int mines = 0;
	if(button_x > 0){
		if(button_y > 0 && mine_grid[button_x-1][button_y-1] == MINE){
			mines++;
		}
		if(button_y < height-1 && mine_grid[button_x-1][button_y+1] == MINE){
			mines++;
		}
		if(mine_grid[button_x-1][button_y] == MINE){
			mines++;
		}
	}
	if(button_x < width-1){
		if(button_y > 0 && mine_grid[button_x+1][button_y-1] == MINE){
			mines++;
		}
		if(button_y < height-1 && mine_grid[button_x+1][button_y+1] == MINE){
			mines++;
		}
		if(mine_grid[button_x+1][button_y] == MINE){
			mines++;
		}
	}
	if(button_y > 0 && mine_grid[button_x][button_y-1] == MINE){
		mines++;
	}
	if(button_y < height-1 && mine_grid[button_x][button_y+1] == MINE){
		mines++;
	}
	if(mines == 0){
		grid[button_x][button_y] = CLEAR;
		if(button_x > 0){
			if(button_y > 0 && grid[button_x-1][button_y-1] == EMPTY){
				sweep(button_x-1, button_y-1, mine_grid, grid, width, height);
			}
			if(button_y < height-1 && grid[button_x-1][button_y+1] == EMPTY){
				sweep(button_x-1, button_y+1, mine_grid, grid, width, height);
			}
			if(grid[button_x-1][button_y] == EMPTY){
				sweep(button_x-1, button_y, mine_grid, grid, width, height);
			}
		}
		if(button_x < width-1){
			if(button_y > 0 && grid[button_x+1][button_y-1] == EMPTY){
				sweep(button_x+1, button_y-1, mine_grid, grid, width, height);
			}
			if(button_y < height-1 && grid[button_x+1][button_y+1] == EMPTY){
				sweep(button_x+1, button_y+1, mine_grid, grid, width, height);
			}
			if(grid[button_x+1][button_y] == EMPTY){
				sweep(button_x+1, button_y, mine_grid, grid, width, height);
			}
		}
		if(button_y > 0 && grid[button_x][button_y-1] == EMPTY){
			sweep(button_x, button_y-1, mine_grid, grid, width, height);
		}
		if(button_y < height-1 && grid[button_x][button_y+1] == EMPTY){
			sweep(button_x, button_y+1, mine_grid, grid, width, height);
		}
	} else {
		grid[button_x][button_y] = 7+mines;
	}
}

int **mine_grid;
int started;
int mines(int **grid, SDL_Event event, int t, int cellsize, int width, int height){
	int ret = MINES;

	if(t == 1){
		started = 0;
		srand(time(NULL));
		//Place all mines
		mine_grid = (int **) calloc(width, sizeof(int *));
		for(int i = 0; i < width; i++){
			mine_grid[i] = (int *) calloc(height, sizeof(int));
			for(int j = 0; j < height; j++){
				if(rand() % 5 == 0){
					mine_grid[i][j] = MINE;
				} else {
					mine_grid[i][j] = CLEAR;
				}
			}
		}
	}

	//Respond to user input
	if(event.type == SDL_MOUSEBUTTONUP){
		int button_x = event.button.x / cellsize;
		int button_y = event.button.y / cellsize;
		if(event.button.button == SDL_BUTTON_LEFT && button_x < width && button_y < height && button_x > -1 && button_y > -1){
			if(started == 0){
				//Clear out 3x3 area around first click to ensure you have a jumping off point
				started = 1;
				mine_grid[button_x][button_y] = CLEAR;
				if(button_x > 0){
					if(button_y > 0){
						mine_grid[button_x-1][button_y-1] = CLEAR;
					}
					if(button_y < height-1){
						mine_grid[button_x-1][button_y+1] = CLEAR;
					}
					mine_grid[button_x-1][button_y] = CLEAR;
				}
				if(button_x < width-1){
					if(button_y > 0){
						mine_grid[button_x+1][button_y-1] = CLEAR;
					}
					if(button_y < height-1){
						mine_grid[button_x+1][button_y+1] = CLEAR;
					}
					mine_grid[button_x+1][button_y] = CLEAR;
				}
				if(button_y > 0){
					mine_grid[button_x][button_y-1] = CLEAR;
				}
				if(button_y < height-1){
					mine_grid[button_x][button_y+1] = CLEAR;
				}
			}
			if(mine_grid[button_x][button_y] == MINE){
				for(int i = 0; i < width; i++){
					for(int j = 0; j < height; j++){
						if(mine_grid[i][j] == MINE){
							grid[i][j] = MINE;
						}
					}
				}
				printf("YOU LOSE!\n");
				ret = ARCADE;
			} else {
				sweep(button_x, button_y, mine_grid, grid, width, height);
				//Check if all mines have been found
				int win_flag = 1;
				for(int i = 0; i < width; i++){
					for(int j = 0; j < height; j++){
						if((grid[i][j] == EMPTY || grid[i][j] == FLAG) && mine_grid[i][j] == CLEAR){
							win_flag = 0;
						}
					}
				}
				if(win_flag == 1){
					printf("YOU WIN!\n");
					ret =  ARCADE;
				}
			}
		} else if(event.button.button == SDL_BUTTON_RIGHT && button_x < width && button_y < height && button_x > -1 && button_y > -1){
			//Place or remove flag
			if(grid[button_x][button_y] == EMPTY){
				grid[button_x][button_y] = FLAG;
			} else if(grid[button_x][button_y] == FLAG){
				grid[button_x][button_y] = EMPTY;
			}
		}
	}

	//Free Dynamic Memory
	if(ret == ARCADE){
		for(int i = 0; i < width; i++){
			free(mine_grid[i]);
		}
		free(mine_grid);
	}

	return ret;
}