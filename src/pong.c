/* By Ethan Hughes */
/* Written 12/6/2018 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"arcade-defs.h"
/*
#define ARCADE 0
#define PONG 4

#define EMPTY -1
#define PADDLE 0
#define BALL 1
*/
int paddle1len, p1y, paddle2len, p2y, ballx, bally, ballxv, ballyv, p1momentum, p2momentum;
int pong(int **grid, SDL_Event event, int t, int width, int height){
	int speedLimit = 4;

	if(t == 1){
		paddle1len = 4;
		paddle2len = 4;
		p1y = height >> 1;
		p2y = height >> 1;
		ballx = width >> 1;
		bally = height >> 1;
		ballxv = 1;
		ballyv = 0;
		p1momentum = 0;
		p2momentum = 0;
	}

	//Respond to user input
	if(event.type == SDL_KEYDOWN){
		switch( event.key.keysym.sym ){
		case SDLK_UP:
			if(p2y > 0){
				p2y--;
				p2momentum = -1;
			}
			break;
		case SDLK_DOWN:
			if(p2y < height-1-paddle2len){
				p2y++;
				p2momentum = 1;
			}
			break;
		case SDLK_w:
			if(p1y > 0){
				p1y--;
				p1momentum = -1;
			}
			break;
		case SDLK_s:
			if(p1y < height-1-paddle1len){
				p1y++;
				p1momentum = 1;
			}
			break;
		}
	}

	//Update Physics

	//Apply Movement

	//Ball Movement
	ballx += ballxv;
	bally += ballyv;

	//AABB Collisions
	if(bally < -ballyv && ballyv < 0){
		ballyv *= -1;
		bally = 0;
	} else if(bally+1 > height-ballyv && ballyv > 0){
		ballyv *= -1;
		bally = height-1;
	}
	if(bally < p1y+paddle1len && bally+1 > p1y && ballx < 1-ballxv && ballxv < 0){
		ballxv *= -1;
		ballyv += p1momentum;
		if(ballyv > speedLimit){
			ballyv = speedLimit;
		} else if(ballyv < -speedLimit){
			ballyv = -speedLimit;
		}
	} else if(bally < p2y+paddle2len && bally+1 > p2y && ballx+1 >width-1-ballxv && ballxv > 0){
		ballxv *= -1;
		ballyv += p2momentum;
		if(ballyv > speedLimit){
			ballyv = speedLimit;
		} else if(ballyv < -speedLimit){
			ballyv = -speedLimit;
		}
	}

	//Win Condition
	if(ballx+1 >= width){
		printf("Player 1 Wins\n");
		return ARCADE;
	} else if(ballx <= 0){
		printf("Player 2 Wins\n");
		return ARCADE;
	}

	//Draw Everything
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			grid[i][j] = EMPTY;
		}
	}
	for(int i = 0; i < paddle1len; i++){
		grid[0][p1y+i] = PADDLE;
	}
	for(int i = 0; i < paddle2len; i++){
		grid[width-1][p2y+i] = PADDLE;
	}
	grid[ballx][bally] = BALL;

	return PONG;
}
