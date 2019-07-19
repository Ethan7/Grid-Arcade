/* By Ethan Hughes */
/* Written 7/4/2019 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>

#define ARCADE 0
#define SETUP 13

#define EMPTY -1
#define FILLED 0

int setup(int **grid, SDL_Event eventbutton, SDL_Event evententer, int setupgame, int cellsize, int width, int height){

	//Respond to user input
	if(eventbutton.type == SDL_MOUSEBUTTONDOWN){
		if(eventbutton.button.button == SDL_BUTTON_LEFT && eventbutton.button.x / cellsize < width && eventbutton.button.y / cellsize < height){
			grid[eventbutton.button.x / cellsize][eventbutton.button.y / cellsize] = FILLED;
		} else if(eventbutton.button.button == SDL_BUTTON_RIGHT){
			grid[eventbutton.button.x / cellsize][eventbutton.button.y / cellsize] = EMPTY;
		}
	} else if(evententer.type == SDL_KEYDOWN && evententer.key.keysym.sym == SDLK_RETURN){
		return setupgame;
	}

	return SETUP;
}