/* By Ethan Hughes */
/* Written 7/4/2019 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include"arcade-defs.h"
/*
#define ARCADE 0
#define SETUP 21

#define EMPTY -1
#define FILLED 0
*/
int setup(int **grid, SDL_Event event, enum GAMEMODE setupgame, int cellsize, int width, int height){

	//Respond to user input
	if(event.type == SDL_MOUSEBUTTONDOWN){
		if(event.button.button == SDL_BUTTON_LEFT && event.button.x / cellsize < width && event.button.y / cellsize < height && event.button.x > -1 && event.button.y > -1){
			grid[event.button.x / cellsize][event.button.y / cellsize] = FILLED;
		} else if(event.button.button == SDL_BUTTON_RIGHT){
			grid[event.button.x / cellsize][event.button.y / cellsize] = EMPTY;
		}
	} else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN){
		return setupgame;
	}

	return SETUP;
}