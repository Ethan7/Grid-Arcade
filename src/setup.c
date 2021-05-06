/* By Ethan Hughes */
/* Written 7/4/2019 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>

#define ARCADE 0
#define SETUP 20

#define EMPTY -1
#define FILLED 0

int setup(int **grid, SDL_Event eventbutton, int setupgame, int cellsize, int width, int height){

	//Respond to user input
	if(eventbutton.type == SDL_MOUSEBUTTONDOWN){
		if(eventbutton.button.button == SDL_BUTTON_LEFT && eventbutton.button.x / cellsize < width && eventbutton.button.y / cellsize < height && eventbutton.button.x > -1 && eventbutton.button.y > -1){
			grid[eventbutton.button.x / cellsize][eventbutton.button.y / cellsize] = FILLED;
		} else if(eventbutton.button.button == SDL_BUTTON_RIGHT){
			grid[eventbutton.button.x / cellsize][eventbutton.button.y / cellsize] = EMPTY;
		}
	} else if(eventbutton.type == SDL_KEYDOWN && eventbutton.key.keysym.sym == SDLK_RETURN){
		return setupgame;
	}

	return SETUP;
}