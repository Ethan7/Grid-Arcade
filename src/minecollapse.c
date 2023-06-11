/* By Ethan Hughes */
/* Written on 3/10/2022 */

/*
//Return types
#define ARCADE 0
#define COLLAPSE 19

//Tile types
#define EMPTY 0
#define MINER 1
#define BOMB 2
#define EXPLOSION 3
#define EXIT 4
#define WALL 5
#define CARTUP 6
#define CARTDOWN 7
#define CARTLEFT 8
#define CARTRIGHT 9
#define BWALL 10

//Explosion timer
#define EXTIMER -10
//Bomb timer
#define BTIMER 10
*/

#include"arcade-defs.h"

//#define SDL_MAIN_HANDLED
#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//Call to create explosion at location
int explode(int x, int y, int **grid, int **undergrid, int width, int cavelength, int size, int bsize, int csize){
	grid[x][y] = EXPLOSION;
	undergrid[x][y] = -10;
	int topwall = 1; //bool
	int botwall = 1;
	int leftwall = 1;
	int rightwall = 1;
	int lose = 0;
	//Walk along the path of every bomb trail and base action on what you find
	for(int i = 1; i < size; i++){
		if(x-i >= 0 && leftwall){
			//Blow up nothing and continue
			if(grid[x-i][y] == EMPTY || grid[x-i][y] == EXPLOSION){
				grid[x-i][y] = EXPLOSION;
				undergrid[x-i][y] = EXTIMER;
			//Stop at wall
			} else if(grid[x-i][y] == MWALL || grid[x-i][y] == EXIT){
				leftwall = 0;
			//Stop at wall and break it
			} else if(grid[x-i][y] == BWALL){
				grid[x-i][y] = EXPLOSION;
				undergrid[x-i][y] = EXTIMER;
				leftwall = 0;
			//Recursive bomb explosion
			} else if(grid[x-i][y] == BOMB){
				lose+=explode(x-i, y, grid, undergrid, width, cavelength, bsize, bsize, csize);
			//Recursive cart explosion
			} else if(grid[x-i][y] >= CARTUP && grid[x-i][y] <= CARTRIGHT){
				lose+=explode(x-i, y, grid, undergrid, width, cavelength, csize, bsize, csize);
			//End the game
			} else if(grid[x-i][y] == MINER){
				lose = 1;
			}
		}
		if(x+i < width && rightwall){
			if(grid[x+i][y] == EMPTY || grid[x+i][y] == EXPLOSION){
				grid[x+i][y] = EXPLOSION;
				undergrid[x+i][y] = EXTIMER;
			} else if(grid[x+i][y] == MWALL || grid[x+i][y] == EXIT){
				rightwall = 0;
			} else if(grid[x+i][y] == BWALL){
				grid[x+i][y] = EXPLOSION;
				undergrid[x+i][y] = EXTIMER;
				rightwall = 0;
			} else if(grid[x+i][y] == BOMB){
				lose+=explode(x+i, y, grid, undergrid, width, cavelength, bsize, bsize, csize);
			} else if(grid[x+i][y] >= CARTUP && grid[x+i][y] <= CARTRIGHT){
				lose+=explode(x+i, y, grid, undergrid, width, cavelength, csize, bsize, csize);
			} else if(grid[x+i][y] == MINER){
				lose = 1;
			}
		}
		if(y-i >= 0 && topwall){
			if(grid[x][y-i] == EMPTY || grid[x][y-i] == EXPLOSION){
				grid[x][y-i] = EXPLOSION;
				undergrid[x][y-i] = EXTIMER;
			} else if(grid[x][y-i] == MWALL || grid[x][y-i] == EXIT){
				topwall = 0;
			} else if(grid[x][y-i] == BWALL){
				grid[x][y-i] = EXPLOSION;
				undergrid[x][y-i] = EXTIMER;
				topwall = 0;
			} else if(grid[x][y-i] == BOMB){
				lose+=explode(x, y-i, grid, undergrid, width, cavelength, bsize, bsize, csize);
			} else if(grid[x][y-i] >= CARTUP && grid[x][y-i] <= CARTRIGHT){
				lose+=explode(x, y-i, grid, undergrid, width, cavelength, csize, bsize, csize);
			} else if(grid[x][y-i] == MINER){
				lose = 1;
			}
		}
		if(y+i < cavelength && botwall){
			if(grid[x][y+i] == EMPTY || grid[x][y+i] == EXPLOSION){
				grid[x][y+i] = EXPLOSION;
				undergrid[x][y+i] = EXTIMER;
			} else if(grid[x][y+i] == MWALL || grid[x][y+i] == EXIT){
				botwall = 0;
			} else if(grid[x][y+i] == BWALL){
				grid[x][y+i] = EXPLOSION;
				undergrid[x][y+i] = EXTIMER;
				botwall = 0;
			} else if(grid[x][y+i] == BOMB){
				lose+=explode(x, y+i, grid, undergrid, width, cavelength, bsize, bsize, csize);
			} else if(grid[x][y+i] >= CARTUP && grid[x][y+i] <= CARTRIGHT){
				lose+=explode(x, y+i, grid, undergrid, width, cavelength, csize, bsize, csize);
			} else if(grid[x][y+i] == MINER){
				lose = 1;
			}
		}
	}
	return lose;
}

//Global variables
int minerx, minery, bombcount, bombreload, cavey;
int **undergrid, **postgrid, **offsetgrid, **maingrid;
int collapse(int **grid, SDL_Event event, int t, int width, int height){
	int ret = COLLAPSE;
	int cavelength = 500;
	int bombtimer = BTIMER;
	int winx = 1;
	int winy = 0;
	int explodeSize = 3;
	int cartExplodeSize = 2;
	int lost = 0;
	//int minerXoffset, minerYoffset;

	if(t == 1){
		srand(time(NULL));

		//Initialize variables
		minerx = rand() % width;
		minery = cavelength-1;
		cavey = cavelength-height;
		bombcount = 3;
		bombreload = -1;
		//minerXoffset = 0;
		//minerYoffset = 0;

		//Setup grid
		undergrid = (int **) calloc(sizeof(int *), width);
		postgrid = (int **) calloc(sizeof(int *), width);
		offsetgrid = (int **) calloc(sizeof(int *), width);
		maingrid = (int **) calloc(sizeof(int *), width);
		for(int i = 0; i < width; i++){
			undergrid[i] = (int *) calloc(sizeof(int), cavelength);
			postgrid[i] = (int *) calloc(sizeof(int), cavelength);
			offsetgrid[i] = (int *) calloc(sizeof(int), cavelength);
			maingrid[i] = (int *) calloc(sizeof(int), cavelength);

			for(int j = 0; j < cavelength; j++){
				undergrid[i][j] = EMPTY;
				postgrid[i][j] = EMPTY;
				offsetgrid[i][j] = EMPTY;
				maingrid[i][j] = EMPTY;
			}
			
			//Make a pokadot of walls
			if(i % 2 == 0){
				for(int j = 0; j < cavelength; j++){
					if(j % 2 == 0){
						maingrid[i][j] = MWALL;
					}
				}
			}
		}
		//Place miner and Exit
		maingrid[minerx][minery] = MINER;
		maingrid[winx][winy] = EXIT;

		//Generate breakable wall locations
		for(int i = 0; i < width; i++){
			for(int j = 0; j < cavelength; j++){
				//Make breakable wall generate more often the further you go up the cave
				if(maingrid[i][j] == EMPTY && rand() % (j/10+1) == 0 && j < 480){
					maingrid[i][j] = BWALL;
				}
			}
		}
	}

	//Input step
	switch(event.type){
	case SDL_KEYDOWN:
		switch( event.key.keysym.sym ){
		case SDLK_UP:
			if(minery-1 >= 0 && maingrid[minerx][minery-1] <= EXPLOSION && maingrid[minerx][minery-1] >= EMPTY && maingrid[minerx][minery-1] != MWALL){ //Requires that constants be in current specific order
				maingrid[minerx][minery] = EMPTY;
				minery--;
				maingrid[minerx][minery] = MINER;
				//Linear interpolation
			}
			break;
		case SDLK_DOWN:
			if(minery+1 < cavelength && maingrid[minerx][minery+1] <= EXPLOSION && maingrid[minerx][minery+1] >= EMPTY && maingrid[minerx][minery+1] != MWALL){
				maingrid[minerx][minery] = EMPTY;
				minery++;
				maingrid[minerx][minery] = MINER;
				//Linear interpolation
			}
			break;
		case SDLK_LEFT:
			if(minerx-1 >= 0 && maingrid[minerx-1][minery] <= EXPLOSION && maingrid[minerx-1][minery] >= EMPTY && maingrid[minerx-1][minery] != MWALL){
				maingrid[minerx][minery] = EMPTY;
				minerx--;
				maingrid[minerx][minery] = MINER;
				//Linear interpolation
			}
			break;
		case SDLK_RIGHT:
			if(minerx+1 < width && maingrid[minerx+1][minery] <= EXPLOSION && maingrid[minerx+1][minery] >= EMPTY && maingrid[minerx+1][minery] != MWALL){
				maingrid[minerx][minery] = EMPTY;
				minerx++;
				maingrid[minerx][minery] = MINER;
				//Linear interpolation
			}
			break;
		case SDLK_RETURN:
			if(bombcount > 0){
				undergrid[minerx][minery] = bombtimer;
				bombcount--;
			}
			break;
		default:
			break;
		}
	default:
		break;
	}

	//Game Step
	//Scroll frame to match the player
	if(minery < cavey+height/2 && minery > height/2){
		cavey = minery-height/2;
	} else if(minery > cavey+height/2 && minery < cavelength-height/2){
		cavey = minery-height/2;
	} else if(minery < height/2){
		cavey = 0;
	} else if(minery >  cavelength-height/2){
		cavey = cavelength-height;
	}

	//Check if player needs another bomb and if so start making one
	if(bombcount < 3){
		if(bombreload > 0){
			bombreload--;
		} else if(bombreload == 0){
			bombcount++;
			bombreload--;
		} else {
			bombreload = 30;
		}
	}

	//Place a random minecart on tracks every 100 frames
	if(rand() % 10 == 0){
		int randdir = rand() % 4;
		int randx = 0;
		int randy = 0;
		int iter = 0;
		do{
			iter++;
			switch(randdir){
			//cart from the top
			case 0:
				randx = rand() % (width/2);
				randx *= 2;
				randx += 1;
				randy = cavey;
				break;
			//cart from the bottom
			case 1:
				randx = rand() % (width/2);
				randx *= 2;
				randx += 1;
				randy = cavey+height-1;
				break;
			//cart from the left
			case 2:
				randx = 0;
				randy = rand() % (height/2);
				randy*= 2;
				randy += cavey+(cavey % 2)-1;
				break;
			//cart from the right
			case 3:
				randx = width-1;
				randy = rand() % (height/2);
				randy*= 2;
				randy += cavey+(cavey % 2)-1;
				break;
			default:
				break;
			}
		} while(maingrid[randx][randy] != EMPTY && iter < 100);
		//Place cart coming from random direction
		switch(randdir){
		case 0:
			//Literals display direction of carts movement not where it's coming from
			maingrid[randx][randy] = CARTDOWN;
			break;
		case 1:
			maingrid[randx][randy] = CARTUP;
			break;
		case 2:
			maingrid[randx][randy] = CARTRIGHT;
			break;
		case 3:
			maingrid[randx][randy] = CARTLEFT;
			break;
		default:
			break;
		}

	}
	
	//Alter grid and undergrid for this timestep
	for(int i = 0; i < width; i++){
		for(int n = cavey; n < cavey+height; n++){
			//Move minecarts across the cave
			if(maingrid[i][n] == CARTUP){
				maingrid[i][n] = EMPTY;
				if(n > cavey){
					if(maingrid[i][n-1] == MINER){
						lost = 1;
					} else if(maingrid[i][n-1] >= BWALL && maingrid[i][n-1] <= CARTRIGHT){
						lost+=explode(i, n-1, maingrid, undergrid, width, cavelength, cartExplodeSize, explodeSize, cartExplodeSize);
					} else {
						maingrid[i][n-1] = CARTUP;
					}
				}
				//Linear interpolation
			}
			if(maingrid[i][n] == CARTDOWN && postgrid[i][n] == 0){
				maingrid[i][n] = EMPTY;
				if(n < cavey+height){
					if(maingrid[i][n+1] == MINER){
						lost = 1;
					} else if(maingrid[i][n+1] >= BWALL && maingrid[i][n+1] <= CARTRIGHT){
						lost+=explode(i, n+1, maingrid, undergrid, width, cavelength, cartExplodeSize, explodeSize, cartExplodeSize);
					} else {
						maingrid[i][n+1] = CARTDOWN;
						postgrid[i][n+1] = 1;
					}
				}
				//Linear interpolation
			}
			if(maingrid[i][n] == CARTLEFT){
				maingrid[i][n] = EMPTY;
				if(i > 0){
					if(maingrid[i-1][n] == MINER){
						lost = 1;
					} else if(maingrid[i-1][n] >= BWALL && maingrid[i-1][n] <= CARTRIGHT){
						lost+=explode(i-1, n, maingrid, undergrid, width, cavelength, cartExplodeSize, explodeSize, cartExplodeSize);
					} else {
						maingrid[i-1][n] = CARTLEFT;
					}
				}
				//Linear interpolation
			}
			if(maingrid[i][n] == CARTRIGHT && postgrid[i][n] == 0){
				maingrid[i][n] = EMPTY;
				if(i < width-1){
					if(maingrid[i+1][n] == MINER){
						lost = 1;
					} else if(maingrid[i+1][n] >= BWALL && maingrid[i+1][n] <= CARTRIGHT){
						lost+=explode(i+1, n, maingrid, undergrid, width, cavelength, cartExplodeSize, explodeSize, cartExplodeSize);
					} else {
						maingrid[i+1][n] = CARTRIGHT;
						postgrid[i+1][n] = 1;
					}
				}
				//Linear interpolation
			}
			//Subtract from bomb timers and create explosions
			if(undergrid[i][n] > 0){
				undergrid[i][n]--; //Subtract from bombtimer
				if(undergrid[i][n] == 0){
					lost+=explode(i, n, maingrid, undergrid, width, cavelength, explodeSize, explodeSize, cartExplodeSize);
				} else if(maingrid[i][n] == EMPTY){
					//display current bombs
					maingrid[i][n] = BOMB;
				}
			} else if(undergrid[i][n] < 0){
				undergrid[i][n]++;
				if(undergrid[i][n] == 0 && maingrid[i][n] == EXPLOSION){
					maingrid[i][n] = EMPTY;
				}
			}

			//clear postgrid
			postgrid[i][n] = 0;
		}
	}

	//Update grid
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			grid[i][j] = maingrid[i][cavey+j];
			if(grid[i][j] >= CARTUP){
				grid[i][j] = CARTUP;
			}
		}
	}

	//Win Condition
	if(minerx == winx && minery == winy){
		printf("YOU WIN!\n");
		printf("Time to beat: %d\n", t);
		ret = ARCADE;
	}

	//Lose Condition
	if(lost){
		printf("You Lose.\n");
		printf("You made it to depth %d\n", cavelength-minery);
		ret = ARCADE;
	}

	if(ret == ARCADE){
		for(int i = 0; i < width; i++){
			free(undergrid[i]);
			free(postgrid[i]);
			free(offsetgrid[i]);
			free(maingrid[i]);
		}
		free(undergrid);
		free(postgrid);
		free(offsetgrid);
		free(maingrid);
	}

	return ret;
}
