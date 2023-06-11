/* By Ethan Hughes */
/* Written 12/7/2018 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"arcade-defs.h"
/*
#define ARCADE 0
#define TETRIS 5

#define EMPTY -1

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
*/
//Array of size [type = 7][height = ?][directions = 4][width = ?]
const char blockshapes[7][4][4][4] = { { { { 0, 1, 0, 0 }, {0, 0, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 0} },
											  {   { 0, 1, 0, 0 }, {0, 0, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 0} }, 
											  {   { 0, 1, 0, 0 }, {1, 1, 1, 1}, {0, 0, 1, 0}, {1, 1, 1, 1} }, 
											  {   { 0, 1, 0, 0 }, {0, 0, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 0} } }, 
											  { { { 0, 1, 0, 0 }, {1, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0} },
											  {   { 0, 1, 0, 0 }, {1, 1, 1, 0}, {0, 1, 0, 0}, {1, 1, 1, 0} }, 
											  {   { 1, 1, 0, 0 }, {0, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0} }, 
											  {   { 0, 0, 0, 0 }, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} } }, 
											  { { { 1, 1, 0, 0 }, {0, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0} },
											  {   { 0, 1, 0, 0 }, {1, 1, 1, 0}, {0, 1, 0, 0}, {1, 1, 1, 0} }, 
											  {   { 0, 1, 0, 0 }, {1, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0} }, 
											  {   { 0, 0, 0, 0 }, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} } }, 
											  { { { 1, 1, 0, 0 }, {1, 1, 0, 0}, {1, 1, 0, 0}, {1, 1, 0, 0} },
											  {   { 1, 1, 0, 0 }, {1, 1, 0, 0}, {1, 1, 0, 0}, {1, 1, 0, 0} }, 
											  {   { 0, 0, 0, 0 }, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} }, 
											  {   { 0, 0, 0, 0 }, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} } }, 
											  { { { 1, 0, 0, 0 }, {0, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} },
											  {   { 1, 1, 0, 0 }, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 1, 1, 0} }, 
											  {   { 0, 1, 0, 0 }, {1, 1, 0, 0}, {0, 0, 1, 0}, {1, 1, 0, 0} }, 
											  {   { 0, 0, 0, 0 }, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} } }, 
											  { { { 0, 1, 0, 0 }, {0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0} },
											  {   { 1, 1, 0, 0 }, {1, 1, 1, 0}, {0, 1, 1, 0}, {1, 1, 1, 0} }, 
											  {   { 0, 1, 0, 0 }, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} }, 
											  {   { 0, 0, 0, 0 }, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} } }, 
											  { { { 0, 1, 0, 0 }, {0, 0, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 0} },
											  {   { 1, 1, 0, 0 }, {1, 1, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0} }, 
											  {   { 1, 0, 0, 0 }, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 1, 1, 0} }, 
											  {   { 0, 0, 0, 0 }, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} } } };

const int dimensions[7] = {4, 3, 3, 2, 3, 3, 3};

int rotationtop, delay, type, blockdir, blockx, blocky, bwidth, bheight;
int tetris(int **grid, SDL_Event event, int t, int width, int height){

	if(t == 1){
		type = rand() % 7;
		blockdir = rand() % 4;
		blockx = width/2;
		blocky = 0;
		bwidth = dimensions[type];
		bheight = dimensions[type];
		rotationtop = height;
		delay = 4;
		srand(time(NULL));
	}

	for(int i = 0; i < bwidth; i++){
		for(int j = 0; j < bheight; j++){
			if(blockx+i >= 0 && blocky+j >= 0 && blockshapes[type][j][blockdir][i]){
				grid[blockx+i][blocky+j] = EMPTY;
			}
		}
	}

	//Respond to user input
	if(event.type == SDL_KEYDOWN){
		int stopped = 0;
		switch( event.key.keysym.sym ){
		case SDLK_UP:
			if(blockx+bwidth < width && blocky+bheight < rotationtop &&
				blockx >= 0 && blocky >= 0){
				if(blockdir == LEFT){
					blockdir = UP;
				} else {
					blockdir++;
				}
			}
			break;
		case SDLK_DOWN:
			if(delay > 1){
				delay--;
			}
			break;
		case SDLK_LEFT:
			for(int i = 0; i < bwidth; i++){
				for(int j = 0; j < bheight; j++){
					if(blockshapes[type][j][blockdir][i] == 1 && 
							(blockx+i-1 < 0 || (blockx+i-1 < width && 
							grid[blockx+i-1][blocky+j] != EMPTY))){
						stopped = 1;
						break;
					}
				}
				if(stopped){
					break;
				}
			}
			if(!stopped){
				blockx--;
			}
			break;
		case SDLK_RIGHT:
			for(int i = 0; i < bwidth; i++){
				for(int j = 0; j < bheight; j++){
					if(blockshapes[type][j][blockdir][i] == 1 && 
							(blockx+i+1 >= width || (blockx+i+1 > 0 && 
							grid[blockx+i+1][blocky+j] != EMPTY))){
						stopped = 1;
						break;
					}
				}
				if(stopped){
					break;
				}
			}
			if(!stopped){
				blockx++;
			}
			break;
		}
	}

	if(delay){
		delay--;
	}

	int fallen = 0;

	for(int i = 0; i < bwidth; i++){
		for(int j = 0; j < bheight; j++){
			if(blockshapes[type][j][blockdir][i] == 1 &&
					blocky+j < height && blockx+i >= 0 && blocky+j >= 0 &&
					grid[blockx+i][blocky+j+1] != EMPTY){
				fallen = 1;
				break;
			} else if(blocky+j+1 >= height && blockshapes[type][j][blockdir][i] == 1){
				fallen = 1;
				break;
			}
		}
		if(fallen){
			break;
		}
	}

	if(!fallen && !delay){
		blocky++;

		delay = 4;
	}

	if(fallen){
		//End game if blocks reach the top
		if(blocky == 0){
			printf("YOU LOSE!\n");
			return ARCADE;
		}
		//Store block in place
		for(int i = 0; i < bwidth; i++){
			for(int j = 0; j < bheight; j++){
				if(blockx+i >= 0 && blocky+j >= 0 && blockshapes[type][j][blockdir][i]){
					grid[blockx+i][blocky+j] = type;
				}
			}
		}
		//Raise heighest block top value
		if(blocky < rotationtop){
			rotationtop = blocky;
		}
		//Check for line to erase
		for(int i = rotationtop; i < height; i++){
			int line = 0;
			for(int j = 0; j < width; j++){
				if(grid[j][i] != EMPTY){
					line++;
				}
			}
			if(line == width){
				for(int j = 0; j < width; j++){
					for(int k = i; k > rotationtop; k--){
						grid[j][k] = grid[j][k-1];
					}
					grid[j][rotationtop] = EMPTY;
				}
				i--;
				rotationtop++;
			}
		}
		//Make new block
		type = rand() % 7;
		blockdir = rand() % 4;
		blockx = width/2;
		blocky = 0;
		bwidth = dimensions[type];
		bheight = dimensions[type];

		fallen = 0;
	}

	for(int i = 0; i < bwidth; i++){
		for(int j = 0; j < bheight; j++){
			if(blockx+i >= 0 && blocky+j >= 0 && blockshapes[type][j][blockdir][i]){
				grid[blockx+i][blocky+j] = type;
			}
		}
	}

	return TETRIS;
}
