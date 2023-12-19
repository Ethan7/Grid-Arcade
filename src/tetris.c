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
const char block_shapes[7][4][4][4] = { { { { 0, 1, 0, 0 }, {0, 0, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 0} },
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

int rotation_top, delay, type, block_dir, block_x, block_y, block_width, block_height;
int tetris(int **grid, SDL_Event event, int t, int width, int height){

	if(t == 1){
		type = rand() % 7;
		block_dir = rand() % 4;
		block_x = width/2;
		block_y = 0;
		block_width = dimensions[type];
		block_height = dimensions[type];
		rotation_top = height;
		delay = 4;
		srand(time(NULL));
	}

	for(int i = 0; i < block_width; i++){
		for(int j = 0; j < block_height; j++){
			if(block_x+i >= 0 && block_y+j >= 0 && block_shapes[type][j][block_dir][i]){
				grid[block_x+i][block_y+j] = EMPTY;
			}
		}
	}

	//Respond to user input
	if(event.type == SDL_KEYDOWN){
		int stopped = 0;
		switch( event.key.keysym.sym ){
		case SDLK_UP:
			if(block_x+block_width < width && block_y+block_height < rotation_top &&
				block_x >= 0 && block_y >= 0){
				if(block_dir == LEFT){
					block_dir = UP;
				} else {
					block_dir++;
				}
			}
			break;
		case SDLK_DOWN:
			if(delay > 1){
				delay--;
			}
			break;
		case SDLK_LEFT:
			for(int i = 0; i < block_width; i++){
				for(int j = 0; j < block_height; j++){
					if(block_shapes[type][j][block_dir][i] == 1 && 
							(block_x+i-1 < 0 || (block_x+i-1 < width && 
							grid[block_x+i-1][block_y+j] != EMPTY))){
						stopped = 1;
						break;
					}
				}
				if(stopped){
					break;
				}
			}
			if(!stopped){
				block_x--;
			}
			break;
		case SDLK_RIGHT:
			for(int i = 0; i < block_width; i++){
				for(int j = 0; j < block_height; j++){
					if(block_shapes[type][j][block_dir][i] == 1 && 
							(block_x+i+1 >= width || (block_x+i+1 > 0 && 
							grid[block_x+i+1][block_y+j] != EMPTY))){
						stopped = 1;
						break;
					}
				}
				if(stopped){
					break;
				}
			}
			if(!stopped){
				block_x++;
			}
			break;
		}
	}

	if(delay){
		delay--;
	}

	int fallen = 0;

	for(int i = 0; i < block_width; i++){
		for(int j = 0; j < block_height; j++){
			if(block_shapes[type][j][block_dir][i] == 1 &&
					block_y+j < height && block_x+i >= 0 && block_y+j >= 0 &&
					grid[block_x+i][block_y+j+1] != EMPTY){
				fallen = 1;
				break;
			} else if(block_y+j+1 >= height && block_shapes[type][j][block_dir][i] == 1){
				fallen = 1;
				break;
			}
		}
		if(fallen){
			break;
		}
	}

	if(!fallen && !delay){
		block_y++;

		delay = 4;
	}

	if(fallen){
		//End game if blocks reach the top
		if(block_y == 0){
			printf("YOU LOSE!\n");
			return ARCADE;
		}
		//Store block in place
		for(int i = 0; i < block_width; i++){
			for(int j = 0; j < block_height; j++){
				if(block_x+i >= 0 && block_y+j >= 0 && block_shapes[type][j][block_dir][i]){
					grid[block_x+i][block_y+j] = type;
				}
			}
		}
		//Raise heighest block top value
		if(block_y < rotation_top){
			rotation_top = block_y;
		}
		//Check for line to erase
		for(int i = rotation_top; i < height; i++){
			int line = 0;
			for(int j = 0; j < width; j++){
				if(grid[j][i] != EMPTY){
					line++;
				}
			}
			if(line == width){
				for(int j = 0; j < width; j++){
					for(int k = i; k > rotation_top; k--){
						grid[j][k] = grid[j][k-1];
					}
					grid[j][rotation_top] = EMPTY;
				}
				i--;
				rotation_top++;
			}
		}
		//Make new block
		type = rand() % 7;
		block_dir = rand() % 4;
		block_x = width/2;
		block_y = 0;
		block_width = dimensions[type];
		block_height = dimensions[type];

		fallen = 0;
	}

	for(int i = 0; i < block_width; i++){
		for(int j = 0; j < block_height; j++){
			if(block_x+i >= 0 && block_y+j >= 0 && block_shapes[type][j][block_dir][i]){
				grid[block_x+i][block_y+j] = type;
			}
		}
	}

	return TETRIS;
}
