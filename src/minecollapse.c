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
int explode(int x, int y, int **grid, int **under_grid, int width, int cave_length, int size, int b_size, int c_size){
	grid[x][y] = EXPLOSION;
	under_grid[x][y] = -10;
	int top_wall = 1; //bool
	int bottom_wall = 1;
	int left_wall = 1;
	int right_wall = 1;
	int lose = 0;
	//Walk along the path of every bomb trail and base action on what you find
	for(int i = 1; i < size; i++){
		if(x-i >= 0 && left_wall){
			//Blow up nothing and continue
			if(grid[x-i][y] == EMPTY || grid[x-i][y] == EXPLOSION){
				grid[x-i][y] = EXPLOSION;
				under_grid[x-i][y] = EXTIMER;
			//Stop at wall
			} else if(grid[x-i][y] == MWALL || grid[x-i][y] == EXIT){
				left_wall = 0;
			//Stop at wall and break it
			} else if(grid[x-i][y] == BWALL){
				grid[x-i][y] = EXPLOSION;
				under_grid[x-i][y] = EXTIMER;
				left_wall = 0;
			//Recursive bomb explosion
			} else if(grid[x-i][y] == BOMB){
				lose+=explode(x-i, y, grid, under_grid, width, cave_length, b_size, b_size, c_size);
			//Recursive cart explosion
			} else if(grid[x-i][y] >= CARTUP && grid[x-i][y] <= CARTRIGHT){
				lose+=explode(x-i, y, grid, under_grid, width, cave_length, c_size, b_size, c_size);
			//End the game
			} else if(grid[x-i][y] == MINER){
				lose = 1;
			}
		}
		if(x+i < width && right_wall){
			if(grid[x+i][y] == EMPTY || grid[x+i][y] == EXPLOSION){
				grid[x+i][y] = EXPLOSION;
				under_grid[x+i][y] = EXTIMER;
			} else if(grid[x+i][y] == MWALL || grid[x+i][y] == EXIT){
				right_wall = 0;
			} else if(grid[x+i][y] == BWALL){
				grid[x+i][y] = EXPLOSION;
				under_grid[x+i][y] = EXTIMER;
				right_wall = 0;
			} else if(grid[x+i][y] == BOMB){
				lose+=explode(x+i, y, grid, under_grid, width, cave_length, b_size, b_size, c_size);
			} else if(grid[x+i][y] >= CARTUP && grid[x+i][y] <= CARTRIGHT){
				lose+=explode(x+i, y, grid, under_grid, width, cave_length, c_size, b_size, c_size);
			} else if(grid[x+i][y] == MINER){
				lose = 1;
			}
		}
		if(y-i >= 0 && top_wall){
			if(grid[x][y-i] == EMPTY || grid[x][y-i] == EXPLOSION){
				grid[x][y-i] = EXPLOSION;
				under_grid[x][y-i] = EXTIMER;
			} else if(grid[x][y-i] == MWALL || grid[x][y-i] == EXIT){
				top_wall = 0;
			} else if(grid[x][y-i] == BWALL){
				grid[x][y-i] = EXPLOSION;
				under_grid[x][y-i] = EXTIMER;
				top_wall = 0;
			} else if(grid[x][y-i] == BOMB){
				lose+=explode(x, y-i, grid, under_grid, width, cave_length, b_size, b_size, c_size);
			} else if(grid[x][y-i] >= CARTUP && grid[x][y-i] <= CARTRIGHT){
				lose+=explode(x, y-i, grid, under_grid, width, cave_length, c_size, b_size, c_size);
			} else if(grid[x][y-i] == MINER){
				lose = 1;
			}
		}
		if(y+i < cave_length && bottom_wall){
			if(grid[x][y+i] == EMPTY || grid[x][y+i] == EXPLOSION){
				grid[x][y+i] = EXPLOSION;
				under_grid[x][y+i] = EXTIMER;
			} else if(grid[x][y+i] == MWALL || grid[x][y+i] == EXIT){
				bottom_wall = 0;
			} else if(grid[x][y+i] == BWALL){
				grid[x][y+i] = EXPLOSION;
				under_grid[x][y+i] = EXTIMER;
				bottom_wall = 0;
			} else if(grid[x][y+i] == BOMB){
				lose+=explode(x, y+i, grid, under_grid, width, cave_length, b_size, b_size, c_size);
			} else if(grid[x][y+i] >= CARTUP && grid[x][y+i] <= CARTRIGHT){
				lose+=explode(x, y+i, grid, under_grid, width, cave_length, c_size, b_size, c_size);
			} else if(grid[x][y+i] == MINER){
				lose = 1;
			}
		}
	}
	return lose;
}

//Global variables
int miner_x, miner_y, bomb_count, bomb_reload, cave_y;
int **under_grid, **post_grid, **offset_grid, **main_grid;
int collapse(int **grid, SDL_Event event, int t, int width, int height){
	int ret = COLLAPSE;
	int cave_length = 500;
	int bomb_timer = BTIMER;
	int win_x = 1;
	int win_y = 0;
	int explode_size = 3;
	int cart_explode_size = 2;
	int lost = 0;
	//int minerXoffset, minerYoffset;

	if(t == 1){
		srand(time(NULL));

		//Initialize variables
		miner_x = rand() % width;
		miner_y = cave_length-1;
		cave_y = cave_length-height;
		bomb_count = 3;
		bomb_reload = -1;
		//minerXoffset = 0;
		//minerYoffset = 0;

		//Setup grid
		under_grid = (int **) calloc(sizeof(int *), width);
		post_grid = (int **) calloc(sizeof(int *), width);
		offset_grid = (int **) calloc(sizeof(int *), width);
		main_grid = (int **) calloc(sizeof(int *), width);
		for(int i = 0; i < width; i++){
			under_grid[i] = (int *) calloc(sizeof(int), cave_length);
			post_grid[i] = (int *) calloc(sizeof(int), cave_length);
			offset_grid[i] = (int *) calloc(sizeof(int), cave_length);
			main_grid[i] = (int *) calloc(sizeof(int), cave_length);

			for(int j = 0; j < cave_length; j++){
				under_grid[i][j] = EMPTY;
				post_grid[i][j] = EMPTY;
				offset_grid[i][j] = EMPTY;
				main_grid[i][j] = EMPTY;
			}
			
			//Make a pokadot of walls
			if(i % 2 == 0){
				for(int j = 0; j < cave_length; j++){
					if(j % 2 == 0){
						main_grid[i][j] = MWALL;
					}
				}
			}
		}
		//Place miner and Exit
		main_grid[miner_x][miner_y] = MINER;
		main_grid[win_x][win_y] = EXIT;

		//Generate breakable wall locations
		for(int i = 0; i < width; i++){
			for(int j = 0; j < cave_length; j++){
				//Make breakable wall generate more often the further you go up the cave
				if(main_grid[i][j] == EMPTY && rand() % (j/10+1) == 0 && j < 480){
					main_grid[i][j] = BWALL;
				}
			}
		}
	}

	//Input step
	switch(event.type){
	case SDL_KEYDOWN:
		switch( event.key.keysym.sym ){
		case SDLK_UP:
			if(miner_y-1 >= 0 && main_grid[miner_x][miner_y-1] <= EXPLOSION && main_grid[miner_x][miner_y-1] >= EMPTY && main_grid[miner_x][miner_y-1] != MWALL){ //Requires that constants be in current specific order
				main_grid[miner_x][miner_y] = EMPTY;
				miner_y--;
				main_grid[miner_x][miner_y] = MINER;
				//Linear interpolation
			}
			break;
		case SDLK_DOWN:
			if(miner_y+1 < cave_length && main_grid[miner_x][miner_y+1] <= EXPLOSION && main_grid[miner_x][miner_y+1] >= EMPTY && main_grid[miner_x][miner_y+1] != MWALL){
				main_grid[miner_x][miner_y] = EMPTY;
				miner_y++;
				main_grid[miner_x][miner_y] = MINER;
				//Linear interpolation
			}
			break;
		case SDLK_LEFT:
			if(miner_x-1 >= 0 && main_grid[miner_x-1][miner_y] <= EXPLOSION && main_grid[miner_x-1][miner_y] >= EMPTY && main_grid[miner_x-1][miner_y] != MWALL){
				main_grid[miner_x][miner_y] = EMPTY;
				miner_x--;
				main_grid[miner_x][miner_y] = MINER;
				//Linear interpolation
			}
			break;
		case SDLK_RIGHT:
			if(miner_x+1 < width && main_grid[miner_x+1][miner_y] <= EXPLOSION && main_grid[miner_x+1][miner_y] >= EMPTY && main_grid[miner_x+1][miner_y] != MWALL){
				main_grid[miner_x][miner_y] = EMPTY;
				miner_x++;
				main_grid[miner_x][miner_y] = MINER;
				//Linear interpolation
			}
			break;
		case SDLK_RETURN:
			if(bomb_count > 0){
				under_grid[miner_x][miner_y] = bomb_timer;
				bomb_count--;
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
	if(miner_y < cave_y+height/2 && miner_y > height/2){
		cave_y = miner_y-height/2;
	} else if(miner_y > cave_y+height/2 && miner_y < cave_length-height/2){
		cave_y = miner_y-height/2;
	} else if(miner_y < height/2){
		cave_y = 0;
	} else if(miner_y >  cave_length-height/2){
		cave_y = cave_length-height;
	}

	//Check if player needs another bomb and if so start making one
	if(bomb_count < 3){
		if(bomb_reload > 0){
			bomb_reload--;
		} else if(bomb_reload == 0){
			bomb_count++;
			bomb_reload--;
		} else {
			bomb_reload = 30;
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
				randy = cave_y;
				break;
			//cart from the bottom
			case 1:
				randx = rand() % (width/2);
				randx *= 2;
				randx += 1;
				randy = cave_y+height-1;
				break;
			//cart from the left
			case 2:
				randx = 0;
				randy = rand() % (height/2);
				randy*= 2;
				randy += cave_y+(cave_y % 2)-1;
				break;
			//cart from the right
			case 3:
				randx = width-1;
				randy = rand() % (height/2);
				randy*= 2;
				randy += cave_y+(cave_y % 2)-1;
				break;
			default:
				break;
			}
		} while(main_grid[randx][randy] != EMPTY && iter < 100);
		//Place cart coming from random direction
		switch(randdir){
		case 0:
			//Literals display direction of carts movement not where it's coming from
			main_grid[randx][randy] = CARTDOWN;
			break;
		case 1:
			main_grid[randx][randy] = CARTUP;
			break;
		case 2:
			main_grid[randx][randy] = CARTRIGHT;
			break;
		case 3:
			main_grid[randx][randy] = CARTLEFT;
			break;
		default:
			break;
		}

	}
	
	//Alter grid and under_grid for this timestep
	for(int i = 0; i < width; i++){
		for(int n = cave_y; n < cave_y+height; n++){
			//Move minecarts across the cave
			if(main_grid[i][n] == CARTUP){
				main_grid[i][n] = EMPTY;
				if(n > cave_y){
					if(main_grid[i][n-1] == MINER){
						lost = 1;
					} else if(main_grid[i][n-1] >= BWALL && main_grid[i][n-1] <= CARTRIGHT){
						lost+=explode(i, n-1, main_grid, under_grid, width, cave_length, cart_explode_size, explode_size, cart_explode_size);
					} else {
						main_grid[i][n-1] = CARTUP;
					}
				}
				//Linear interpolation
			}
			if(main_grid[i][n] == CARTDOWN && post_grid[i][n] == 0){
				main_grid[i][n] = EMPTY;
				if(n < cave_y+height){
					if(main_grid[i][n+1] == MINER){
						lost = 1;
					} else if(main_grid[i][n+1] >= BWALL && main_grid[i][n+1] <= CARTRIGHT){
						lost+=explode(i, n+1, main_grid, under_grid, width, cave_length, cart_explode_size, explode_size, cart_explode_size);
					} else {
						main_grid[i][n+1] = CARTDOWN;
						post_grid[i][n+1] = 1;
					}
				}
				//Linear interpolation
			}
			if(main_grid[i][n] == CARTLEFT){
				main_grid[i][n] = EMPTY;
				if(i > 0){
					if(main_grid[i-1][n] == MINER){
						lost = 1;
					} else if(main_grid[i-1][n] >= BWALL && main_grid[i-1][n] <= CARTRIGHT){
						lost+=explode(i-1, n, main_grid, under_grid, width, cave_length, cart_explode_size, explode_size, cart_explode_size);
					} else {
						main_grid[i-1][n] = CARTLEFT;
					}
				}
				//Linear interpolation
			}
			if(main_grid[i][n] == CARTRIGHT && post_grid[i][n] == 0){
				main_grid[i][n] = EMPTY;
				if(i < width-1){
					if(main_grid[i+1][n] == MINER){
						lost = 1;
					} else if(main_grid[i+1][n] >= BWALL && main_grid[i+1][n] <= CARTRIGHT){
						lost+=explode(i+1, n, main_grid, under_grid, width, cave_length, cart_explode_size, explode_size, cart_explode_size);
					} else {
						main_grid[i+1][n] = CARTRIGHT;
						post_grid[i+1][n] = 1;
					}
				}
				//Linear interpolation
			}
			//Subtract from bomb timers and create explosions
			if(under_grid[i][n] > 0){
				under_grid[i][n]--; //Subtract from bomb_timer
				if(under_grid[i][n] == 0){
					lost+=explode(i, n, main_grid, under_grid, width, cave_length, explode_size, explode_size, cart_explode_size);
				} else if(main_grid[i][n] == EMPTY){
					//display current bombs
					main_grid[i][n] = BOMB;
				}
			} else if(under_grid[i][n] < 0){
				under_grid[i][n]++;
				if(under_grid[i][n] == 0 && main_grid[i][n] == EXPLOSION){
					main_grid[i][n] = EMPTY;
				}
			}

			//clear post_grid
			post_grid[i][n] = 0;
		}
	}

	//Update grid
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			grid[i][j] = main_grid[i][cave_y+j];
			if(grid[i][j] >= CARTUP){
				grid[i][j] = CARTUP;
			}
		}
	}

	//Win Condition
	if(miner_x == win_x && miner_y == win_y){
		printf("YOU WIN!\n");
		printf("Time to beat: %d\n", t);
		ret = ARCADE;
	}

	//Lose Condition
	if(lost){
		printf("You Lose.\n");
		printf("You made it to depth %d\n", cave_length-miner_y);
		ret = ARCADE;
	}

	if(ret == ARCADE){
		for(int i = 0; i < width; i++){
			free(under_grid[i]);
			free(post_grid[i]);
			free(offset_grid[i]);
			free(main_grid[i]);
		}
		free(under_grid);
		free(post_grid);
		free(offset_grid);
		free(main_grid);
	}

	return ret;
}
