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
int ant_x, ant_y, ant_dir, stand_in;
int langston(int **grid, int t, int width, int height){

	if(t == 1){
		ant_dir = 0;
		ant_x = width/2;
		ant_y = height/2;
		stand_in = grid[ant_x][ant_y];
	}
	grid[ant_x][ant_y] = stand_in;

	if(grid[ant_x][ant_y] == WHITE){
		if(ant_dir == 3){
			ant_dir = 0;
		} else {
			ant_dir++;
		}
		grid[ant_x][ant_y] = BLACK;
		if(ant_dir == 0 && ant_y > 0){
			ant_y--;
		} else if(ant_dir == 1 && ant_x < width-1){
			ant_x++;
		} else if(ant_dir == 2 && ant_y < height-1){
			ant_y++;
		} else if(ant_dir == 3 && ant_x > 0){
			ant_x--;
		} else {
			return ARCADE;
		}
	} else if(grid[ant_x][ant_y] == BLACK){
		if(ant_dir == 0){
			ant_dir = 3;
		} else {
			ant_dir--;
		}
		grid[ant_x][ant_y] = WHITE;
		if(ant_dir == 0 && ant_y > 0){
			ant_y--;
		} else if(ant_dir == 1 && ant_x < width-1){
			ant_x++;
		} else if(ant_dir == 2 && ant_y < height-1){
			ant_y++;
		} else if(ant_dir == 3 && ant_x > 0){
			ant_x--;
		} else {
			return ARCADE;
		}
	}

	stand_in = grid[ant_x][ant_y];
	grid[ant_x][ant_y] = ANT;
	return LANGSTON;
}