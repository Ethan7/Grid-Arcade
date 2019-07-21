/* By Ethan Hughes */
/* Written 7/21/2019 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>

#define ARCADE 0
#define CHECKERS 12

#define EMPTY -1
#define P1PAWN 0
#define P2PAWN 1
#define P1KING 2
#define P2KING 3


int checkers(int **grid, SDL_Event eventbutton, int game, int t, int cellsize, int width, int height){

	if(t == 1){

	}

	//Respond to user input
	if(eventbutton.type == SDL_MOUSEBUTTONDOWN){
		if(eventbutton.button.button == SDL_BUTTON_LEFT && eventbutton.button.x / cellsize < width && eventbutton.button.y / cellsize < height && eventbutton.button.x > 0 && eventbutton.button.y > 0){
			grid[eventbutton.button.x / cellsize][eventbutton.button.y / cellsize] = FILLED;
		} else if(eventbutton.button.button == SDL_BUTTON_RIGHT){
			grid[eventbutton.button.x / cellsize][eventbutton.button.y / cellsize] = EMPTY;
		}
	} else if(evententer.type == SDL_KEYDOWN && evententer.key.keysym.sym == SDLK_RETURN){
		return game;
	}

	return CHECKERS;
}