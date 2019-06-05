/* By Ethan Hughes */
/* Written 12/7/2018 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define ARCADE 0
#define TETRIS 5

#define EMPTY -1

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

//Array of size [type = 7][height = ?][directions = 4][width = ?]
static const char blockshapes[7][4][4][4] = { { { { 0, 1, 0, 0 }, {0, 0, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 0} },
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

static const int dimensions[7] = {4, 3, 3, 2, 3, 3, 3};

static int rotationtop, delay, type, direction, bx, by, bwidth, bheight;
int tetris(int **grid, SDL_Event event, int game, int t, int width, int height){

	if(t == 1){
		type = rand() % 7;
		direction = rand() % 4;
		bx = width/2;
		by = 0;
		bwidth = dimensions[type];
		bheight = dimensions[type];
		rotationtop = height;
		delay = 4;
		srand(time(NULL));
	}

	for(int i = 0; i < bwidth; i++){
		for(int j = 0; j < bheight; j++){
			if(bx+i >= 0 && by+j >= 0 && blockshapes[type][j][direction][i]){
				grid[bx+i][by+j] = EMPTY;
			}
		}
	}

	//Respond to user input
	if(event.type == SDL_KEYDOWN){
		int stopped = 0;
		switch( event.key.keysym.sym ){
		case SDLK_UP:
			if(bx+bwidth < width && by+bheight < rotationtop &&
				bx >= 0 && by >= 0){
				if(direction == LEFT){
					direction = UP;
				} else {
					direction++;
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
					if(blockshapes[type][j][direction][i] == 1 && 
							(bx+i-1 < 0 || (bx+i-1 < width && 
							grid[bx+i-1][by+j] != EMPTY))){
						stopped = 1;
						break;
					}
				}
				if(stopped){
					break;
				}
			}
			if(!stopped){
				bx--;
			}
			break;
		case SDLK_RIGHT:
			for(int i = 0; i < bwidth; i++){
				for(int j = 0; j < bheight; j++){
					if(blockshapes[type][j][direction][i] == 1 && 
							(bx+i+1 >= width || (bx+i+1 > 0 && 
							grid[bx+i+1][by+j] != EMPTY))){
						stopped = 1;
						break;
					}
				}
				if(stopped){
					break;
				}
			}
			if(!stopped){
				bx++;
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
			if(blockshapes[type][j][direction][i] == 1 &&
					by+j < height && bx+i >= 0 && by+j >= 0 &&
					grid[bx+i][by+j+1] != EMPTY){
				fallen = 1;
				break;
			} else if(by+j+1 >= height && blockshapes[type][j][direction][i] == 1){
				fallen = 1;
				break;
			}
		}
		if(fallen){
			break;
		}
	}

	if(!fallen && !delay){
		by++;

		delay = 4;
	}

	if(fallen){
		//End game if blocks reach the top
		if(by == 0){
			printf("YOU LOSE!\n");
			return ARCADE;
		}
		//Store block in place
		for(int i = 0; i < bwidth; i++){
			for(int j = 0; j < bheight; j++){
				if(bx+i >= 0 && by+j >= 0 && blockshapes[type][j][direction][i]){
					grid[bx+i][by+j] = type;
				}
			}
		}
		//Raise heighest block top value
		if(by < rotationtop){
			rotationtop = by;
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
		direction = rand() % 4;
		bx = width/2;
		by = 0;
		bwidth = dimensions[type];
		bheight = dimensions[type];

		fallen = 0;
	}

	for(int i = 0; i < bwidth; i++){
		for(int j = 0; j < bheight; j++){
			if(bx+i >= 0 && by+j >= 0 && blockshapes[type][j][direction][i]){
				grid[bx+i][by+j] = type;
			}
		}
	}

	return TETRIS;
}
