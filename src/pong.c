/* By Ethan Hughes */
/* Written 12/6/2018 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define ARCADE 0
#define PONG 4

#define EMPTY -1
#define PADDLE 0
#define BALL 1

int paddle1len, p1y, paddle2len, p2y, bx, by, bxv, byv, speedLimit, p1momentum, p2momentum;
int pong(int **grid, SDL_Event event, int game, int t, int width, int height){

	if(t == 1){
		paddle1len = 4;
		paddle2len = 4;
		p1y = height >> 1;
		p2y = height >> 1;
		bx = width >> 1;
		by = height >> 1;
		bxv = 1;
		byv = 0;
		p1momentum = 0;
		p2momentum = 0;
		speedLimit = 4;
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
	bx += bxv;
	by += byv;

	//AABB Collisions
	if(by < -byv && byv < 0){
		byv *= -1;
		by = 0;
	} else if(by+1 > height-byv && byv > 0){
		byv *= -1;
		by = height-1;
	}
	if(by < p1y+paddle1len && by+1 > p1y && bx < 1-bxv && bxv < 0){
		bxv *= -1;
		byv += p1momentum;
		if(byv > speedLimit){
			byv = speedLimit;
		} else if(byv < -speedLimit){
			byv = -speedLimit;
		}
	} else if(by < p2y+paddle2len && by+1 > p2y && bx+1 >width-1-bxv && bxv > 0){
		bxv *= -1;
		byv += p2momentum;
		if(byv > speedLimit){
			byv = speedLimit;
		} else if(byv < -speedLimit){
			byv = -speedLimit;
		}
	}

	//Win Condition
	if(bx+1 >= width){
		printf("Player 1 Wins\n");
		return ARCADE;
	} else if(bx <= 0){
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
	grid[bx][by] = BALL;

	return PONG;
}
