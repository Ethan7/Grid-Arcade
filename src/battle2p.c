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
int **undergrid1, **undergrid2;
int cannonballs1, cannonballs2;
int shipcount;
int phase;
int shipsplaced;
int direction;
void phase1(int width, int height, int newship, int **grid, int **undergrid, int newphase, int x, int y, int dir){
	//Place all the ships
	int shipsize = shipsplaced / 2 + 2; //Place increasingly larger ships
	for(int j = 0; j < shipsize; j++){
		if(j > 0){
			if(dir == 0 && x + j >= width){
				break;
			} else if(dir == 1 && y + j >= height){
				break;
			} else if(dir == 0 && undergrid[x+j][y] == newship){
				break;
			} else if(dir == 1 && undergrid[x][y+j] == newship){
				break;
			}
		} else if(undergrid[x][y] == newship){
			break;
		}
		//Place current ship
		if(j == shipsize-1){
			for(int k = 0; k < shipsize; k++){
				if(dir == 0){
					undergrid[x+k][y] = newship;
					grid[x+k][y] = newship;
				} else {
					undergrid[x][y+k] = newship;
					grid[x][y+k] = newship;
				}
			}
			shipsplaced++;
		}
	}
	if(shipsplaced == shipcount){
		shipsplaced = 0;
		for(int i = 0; i < width; i++){
			for (int j = 0; j < height; j++){
				grid[i][j] = EMPTY;
			}
		}
		phase = newphase; //When all player 1 ships placed switch to player 2
		if(newphase == 1){
			printf("It is now Player 2's turn to place ships.\n");
		} else {
			printf("It is now Player 1's turn to attack ships.\n");
		}
	}
}

int phase2(int **grid, int x, int y, int width, int height, int player){
	//A bunch of extra lines to shrink the definition and implementation off the function
	int *cannonballs;
	int **undergrid;
	int **enemyundergrid;
	int newphase;
	int newship;
	int newshipattacked;
	int enemyattacked;
	if(player == 1){
		cannonballs = &cannonballs1;
		undergrid = undergrid2;
		enemyundergrid = undergrid1;
		newphase = 3;
		newship = P2SHIP;
		newshipattacked = P2SHIPATTACKED;
		enemyattacked = P1SHIPATTACKED;
	} else {
		cannonballs = &cannonballs2;
		undergrid = undergrid1;
		enemyundergrid = undergrid2;
		newphase = 2;
		newship = P1SHIP;
		newshipattacked = P1SHIPATTACKED;
		enemyattacked = P2SHIPATTACKED;
	}
	(*cannonballs)--;
	if(undergrid[x][y] > CLEAR){
		undergrid[x][y] = newshipattacked;
		printf("Player %d HIT! %d cannonballs left.\n", player, *cannonballs);
	} else {
		undergrid[x][y] = CLEAR;
		printf("Player %d MISS! %d cannonballs left.\n", player, *cannonballs);
	}
	//check if all ships have been found
	int winflag = 1;
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			if(undergrid[i][j] == newship){
				winflag = 0;
			}
		}
	}
	if(winflag == 1){
		printf("PLAYER %d WINS!\n", player);
		return 1;
	}
	phase = newphase; //Switch to player 2's turn
	//Display the attacked squares of next player
	for(int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){
			if(enemyundergrid[i][j] == CLEAR || enemyundergrid[i][j] == enemyattacked){
				grid[i][j] = enemyundergrid[i][j];
			} else {
				grid[i][j] = EMPTY;
			}
		}
	}
	return 0;
}

int battle2(int **grid, SDL_Event eventbutton, int t, int cellsize, int width, int height){
	int ret = BATTLE2;

	if(t == 1){
		cannonballs1 = width*height/3; //Only enough cannonballs to test 1/3 of the spaces
		cannonballs2 = width*height/3;
		//Allocate undergrids
		undergrid1 = (int **) calloc(width, sizeof(int *));
		undergrid2 = (int **) calloc(width, sizeof(int *));
		for(int i = 0; i < width; i++){
			undergrid1[i] = (int *) calloc(height, sizeof(int));
			undergrid2[i] = (int *) calloc(height, sizeof(int));
			for(int j = 0; j < height; j++){
				undergrid1[i][j] = EMPTY;
				undergrid2[i][j] = EMPTY;
			}
		}
		shipcount = sqrt(width*height)/2; //Shipcount based on gridsize, the more ships the bigger they get
		phase = 0;
		shipsplaced = 0;
		direction = 0;
	}

	//Respond to user input
	if(eventbutton.type == SDL_MOUSEBUTTONUP){
		int buttonx = eventbutton.button.x / cellsize;
		int buttony = eventbutton.button.y / cellsize;
		if(eventbutton.button.button == SDL_BUTTON_LEFT && buttonx < width && buttony < height && buttonx > -1 && buttony > -1 && grid[buttonx][buttony] == EMPTY){
			switch(phase){
			case 0:
				phase1(width, height, P1SHIP, grid, undergrid1, 1, buttonx, buttony, direction);
				break;
			case 1:
				phase1(width, height, P2SHIP, grid, undergrid2, 2, buttonx, buttony, direction);
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
					int p1points = 0;
					int p2points = 0;
					for(int i = 0; i < width; i++){
						for(int j = 0; j < height; j++){
							if(undergrid1[i][j] > CLEAR){
								grid[i][j] = undergrid1[i][j];
								if(undergrid1[i][j] == P1SHIPATTACKED){
									p2points++;
								}
							}
							if(undergrid2[i][j] > CLEAR){
								grid[i][j] = undergrid2[i][j];
								if(undergrid2[i][j] == P2SHIPATTACKED){
									p1points++;
								}
							}
						}
					}
					if(p1points > p2points){
						printf("Score: %d-%d Player 1 Wins!\n", p1points, p2points);
					} else if(p1points < p2points){
						printf("Score: %d-%d Player 2 Wins!\n", p1points, p2points);
					} else {
						printf("Score: %d-%d It's a tie!\n", p1points, p2points);
					}
					ret = ARCADE;
				}
				break;
			}
		} else if(eventbutton.button.button == SDL_BUTTON_RIGHT){
			direction = 1 - direction;
		}
	}

	//Free Dynamic Memory
	if(ret == ARCADE){
		for(int i = 0; i < width; i++){
			free(undergrid1[i]);
			free(undergrid2[i]);
		}
		free(undergrid1);
		free(undergrid2);
	}

	return ret;
}