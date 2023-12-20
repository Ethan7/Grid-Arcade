/* By Ethan Hughes */
/* Written 4/28/2021 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"arcade-defs.h"
/*
#define ARCADE 0
#define BATTLE2 16

#define EMPTY -1
#define CLEAR 0
#define P1SHIP 1
#define P1SHIPATTACKED 2
#define P2SHIP 3
#define P2SHIPATTACKED 5
*/
int **under_grid1, **under_grid2;
int cannonballs1, cannonballs2;
int ship_count;
int phase;
int ships_placed;
int direction;
void phase1(int width, int height, int new_ship, int **grid, int **under_grid, int new_phase, int x, int y, int dir){
	//Place all the ships
	const int ship_size = ships_placed / 2 + 2; //Place increasingly larger ships
	for(int j = 0; j < ship_size; j++){
		if(j > 0){
			if(dir == 0 && x + j >= width){
				break;
			} else if(dir == 1 && y + j >= height){
				break;
			} else if(dir == 0 && under_grid[x+j][y] == new_ship){
				break;
			} else if(dir == 1 && under_grid[x][y+j] == new_ship){
				break;
			}
		} else if(under_grid[x][y] == new_ship){
			break;
		}
		//Place current ship
		if(j == ship_size-1){
			for(int k = 0; k < ship_size; k++){
				if(dir == 0){
					under_grid[x+k][y] = new_ship;
					grid[x+k][y] = new_ship;
				} else {
					under_grid[x][y+k] = new_ship;
					grid[x][y+k] = new_ship;
				}
			}
			ships_placed++;
		}
	}
	if(ships_placed == ship_count){
		ships_placed = 0;
		for(int i = 0; i < width; i++){
			for (int j = 0; j < height; j++){
				grid[i][j] = EMPTY;
			}
		}
		phase = new_phase; //When all player 1 ships placed switch to player 2
		if(new_phase == 1){
			printf("It is now Player 2's turn to place ships.\n");
		} else {
			printf("It is now Player 1's turn to attack ships.\n");
		}
	}
}

int phase2(int **grid, int x, int y, int width, int height, int player){
	//A bunch of extra lines to shrink the definition and implementation off the function
	int *cannonballs;
	int **under_grid;
	int **enemy_under_grid;
	int new_phase;
	int new_ship;
	int new_shipattacked;
	int enemy_attacked;
	if(player == 1){
		cannonballs = &cannonballs1;
		under_grid = under_grid2;
		enemy_under_grid = under_grid1;
		new_phase = 3;
		new_ship = P2SHIP;
		new_shipattacked = P2SHIPATTACKED;
		enemy_attacked = P1SHIPATTACKED;
	} else {
		cannonballs = &cannonballs2;
		under_grid = under_grid1;
		enemy_under_grid = under_grid2;
		new_phase = 2;
		new_ship = P1SHIP;
		new_shipattacked = P1SHIPATTACKED;
		enemy_attacked = P2SHIPATTACKED;
	}
	(*cannonballs)--;
	if(under_grid[x][y] > CLEAR){
		under_grid[x][y] = new_shipattacked;
		printf("Player %d HIT! %d cannonballs left.\n", player, *cannonballs);
	} else {
		under_grid[x][y] = CLEAR;
		printf("Player %d MISS! %d cannonballs left.\n", player, *cannonballs);
	}
	//check if all ships have been found
	int win_flag = 1;
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			if(under_grid[i][j] == new_ship){
				win_flag = 0;
			}
		}
	}
	if(win_flag == 1){
		printf("PLAYER %d WINS!\n", player);
		return 1;
	}
	phase = new_phase; //Switch to player 2's turn
	//Display the attacked squares of next player
	for(int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){
			if(enemy_under_grid[i][j] == CLEAR || enemy_under_grid[i][j] == enemy_attacked){
				grid[i][j] = enemy_under_grid[i][j];
			} else {
				grid[i][j] = EMPTY;
			}
		}
	}
	return 0;
}

int battle2(int **grid, SDL_Event event, int t, int cellsize, int width, int height){
	int ret = BATTLE2;

	if(t == 1){
		cannonballs1 = width*height/3; //Only enough cannonballs to test 1/3 of the spaces
		cannonballs2 = width*height/3;
		//Allocate under_grids
		under_grid1 = (int **) calloc(width, sizeof(int *));
		under_grid2 = (int **) calloc(width, sizeof(int *));
		for(int i = 0; i < width; i++){
			under_grid1[i] = (int *) calloc(height, sizeof(int));
			under_grid2[i] = (int *) calloc(height, sizeof(int));
			for(int j = 0; j < height; j++){
				under_grid1[i][j] = EMPTY;
				under_grid2[i][j] = EMPTY;
			}
		}
		ship_count = sqrt(width*height)/2; //Shipcount based on gridsize, the more ships the bigger they get
		phase = 0;
		ships_placed = 0;
		direction = 0;
	}

	//Respond to user input
	if(event.type == SDL_MOUSEBUTTONUP){
		int buttonx = event.button.x / cellsize;
		int buttony = event.button.y / cellsize;
		if(event.button.button == SDL_BUTTON_LEFT && buttonx < width && buttony < height && buttonx > -1 && buttony > -1 && grid[buttonx][buttony] == EMPTY){
			switch(phase){
			case 0:
				phase1(width, height, P1SHIP, grid, under_grid1, 1, buttonx, buttony, direction);
				break;
			case 1:
				phase1(width, height, P2SHIP, grid, under_grid2, 2, buttonx, buttony, direction);
				break;
			case 2:
				if(phase2(grid, buttonx, buttony, width, height, 1) == 1){
					ret = ARCADE;
					break;
				}
				printf("It is now Player 2's turn to attack ships.\n");
				break;
			case 3:
				if(phase2(grid, buttonx, buttony, width, height, 2) == 1){
					ret = ARCADE;
					break;
				}
				if(cannonballs1 > 0){
					printf("It is now Player 1's turn to attack ships.\n");
				} else {
					int p1_points = 0;
					int p2_points = 0;
					for(int i = 0; i < width; i++){
						for(int j = 0; j < height; j++){
							if(under_grid1[i][j] > CLEAR){
								grid[i][j] = under_grid1[i][j];
								if(under_grid1[i][j] == P1SHIPATTACKED){
									p2_points++;
								}
							}
							if(under_grid2[i][j] > CLEAR){
								grid[i][j] = under_grid2[i][j];
								if(under_grid2[i][j] == P2SHIPATTACKED){
									p1_points++;
								}
							}
						}
					}
					if(p1_points > p2_points){
						printf("Score: %d-%d Player 1 Wins!\n", p1_points, p2_points);
					} else if(p1_points < p2_points){
						printf("Score: %d-%d Player 2 Wins!\n", p1_points, p2_points);
					} else {
						printf("Score: %d-%d It's a tie!\n", p1_points, p2_points);
					}
					ret = ARCADE;
				}
				break;
			}
		} else if(event.button.button == SDL_BUTTON_RIGHT){
			direction = 1 - direction;
		}
	}

	//Free Dynamic Memory
	if(ret == ARCADE){
		for(int i = 0; i < width; i++){
			free(under_grid1[i]);
			free(under_grid2[i]);
		}
		free(under_grid1);
		free(under_grid2);
	}

	return ret;
}