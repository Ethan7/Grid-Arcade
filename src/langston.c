/* By Ethan Hughes */
/* Written 7/20/2019 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include"arcade-defs.h"
/*
#define ARCADE 0
#define LANGSTON 9

#define BLACK -1
#define WHITE 0
#define ANT 1
*/
int antx, anty, antdir, standin;
int langston(int **grid, int t, int width, int height){

	if(t == 1){
		antdir = 0;
		antx = width/2;
		anty = height/2;
		standin = grid[antx][anty];
	}
	grid[antx][anty] = standin;

	if(grid[antx][anty] == WHITE){
		if(antdir == 3){
			antdir = 0;
		} else {
			antdir++;
		}
		grid[antx][anty] = BLACK;
		if(antdir == 0 && anty > 0){
			anty--;
		} else if(antdir == 1 && antx < width-1){
			antx++;
		} else if(antdir == 2 && anty < height-1){
			anty++;
		} else if(antdir == 3 && antx > 0){
			antx--;
		} else {
			return ARCADE;
		}
	} else if(grid[antx][anty] == BLACK){
		if(antdir == 0){
			antdir = 3;
		} else {
			antdir--;
		}
		grid[antx][anty] = WHITE;
		if(antdir == 0 && anty > 0){
			anty--;
		} else if(antdir == 1 && antx < width-1){
			antx++;
		} else if(antdir == 2 && anty < height-1){
			anty++;
		} else if(antdir == 3 && antx > 0){
			antx--;
		} else {
			return ARCADE;
		}
	}

	standin = grid[antx][anty];
	grid[antx][anty] = ANT;
	return LANGSTON;
}