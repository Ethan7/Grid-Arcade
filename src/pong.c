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
int paddle1_length, p1y, paddle2_length, p2y, ball_x, ball_y, ball_xv, ball_yv, p1_momentum, p2_momentum;
int pong(int **grid, SDL_Event event, int t, int width, int height){
	int speed_limit = 4;

	if(t == 1){
		paddle1_length = 4;
		paddle2_length = 4;
		p1y = height >> 1;
		p2y = height >> 1;
		ball_x = width >> 1;
		ball_y = height >> 1;
		ball_xv = 1;
		ball_yv = 0;
		p1_momentum = 0;
		p2_momentum = 0;
	}

	//Respond to user input
	if(event.type == SDL_KEYDOWN){
		switch( event.key.keysym.sym ){
		case SDLK_UP:
			if(p2y > 0){
				p2y--;
				p2_momentum = -1;
			}
			break;
		case SDLK_DOWN:
			if(p2y < height-1-paddle2_length){
				p2y++;
				p2_momentum = 1;
			}
			break;
		case SDLK_w:
			if(p1y > 0){
				p1y--;
				p1_momentum = -1;
			}
			break;
		case SDLK_s:
			if(p1y < height-1-paddle1_length){
				p1y++;
				p1_momentum = 1;
			}
			break;
		}
	}

	//Update Physics

	//Apply Movement

	//Ball Movement
	ball_x += ball_xv;
	ball_y += ball_yv;

	//AABB Collisions
	if(ball_y < -ball_yv && ball_yv < 0){
		ball_yv *= -1;
		ball_y = 0;
	} else if(ball_y+1 > height-ball_yv && ball_yv > 0){
		ball_yv *= -1;
		ball_y = height-1;
	}
	if(ball_y < p1y+paddle1_length && ball_y+1 > p1y && ball_x < 1-ball_xv && ball_xv < 0){
		ball_xv *= -1;
		ball_yv += p1_momentum;
		if(ball_yv > speed_limit){
			ball_yv = speed_limit;
		} else if(ball_yv < -speed_limit){
			ball_yv = -speed_limit;
		}
	} else if(ball_y < p2y+paddle2_length && ball_y+1 > p2y && ball_x+1 >width-1-ball_xv && ball_xv > 0){
		ball_xv *= -1;
		ball_yv += p2_momentum;
		if(ball_yv > speed_limit){
			ball_yv = speed_limit;
		} else if(ball_yv < -speed_limit){
			ball_yv = -speed_limit;
		}
	}

	//Win Condition
	if(ball_x+1 >= width){
		printf("Player 1 Wins\n");
		return ARCADE;
	} else if(ball_x <= 0){
		printf("Player 2 Wins\n");
		return ARCADE;
	}

	//Draw Everything
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			grid[i][j] = EMPTY;
		}
	}
	for(int i = 0; i < paddle1_length; i++){
		grid[0][p1y+i] = PADDLE;
	}
	for(int i = 0; i < paddle2_length; i++){
		grid[width-1][p2y+i] = PADDLE;
	}
	grid[ball_x][ball_y] = BALL;

	return PONG;
}
