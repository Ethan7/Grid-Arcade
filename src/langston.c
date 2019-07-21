/* By Ethan Hughes */
/* Written 7/20/2019 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>

#define ARCADE 0
#define LANGSTON 9

#define BLACK -1
#define WHITE 0
#define ANT 1

int antx, anty, direction, placeholder;
int langston(int **grid, int game, int t, int width, int height){

	if(t == 1){
		direction = 0;
		antx = width/2;
		anty = height/2;
		placeholder = grid[antx][anty];
	}
	grid[antx][anty] = placeholder;

	if(grid[antx][anty] == WHITE){
		if(direction == 3){
			direction = 0;
		} else {
			direction++;
		}
		grid[antx][anty] = BLACK;
		if(direction == 0 && anty > 0){
			anty--;
		} else if(direction == 1 && antx < width-1){
			antx++;
		} else if(direction == 2 && anty < height-1){
			anty++;
		} else if(direction == 3 && antx > 0){
			antx--;
		} else {
			return ARCADE;
		}
	} else if(grid[antx][anty] == BLACK){
		if(direction == 0){
			direction = 3;
		} else {
			direction--;
		}
		grid[antx][anty] = WHITE;
		if(direction == 0 && anty > 0){
			anty--;
		} else if(direction == 1 && antx < width-1){
			antx++;
		} else if(direction == 2 && anty < height-1){
			anty++;
		} else if(direction == 3 && antx > 0){
			antx--;
		} else {
			return ARCADE;
		}
	}

	placeholder = grid[antx][anty];
	grid[antx][anty] = ANT;
	return LANGSTON;
}