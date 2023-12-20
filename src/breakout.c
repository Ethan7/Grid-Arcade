/* By Ethan Hughes */
/* Written 5/12/2021 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"arcade-defs.h"
/*
#define ARCADE 0
#define BREAKOUT 18

#define EMPTY -1
#define PADDLE 0
#define BALL 1
#define BLOCK 2

#define GREEN 2
#define BLUE 3
#define YELLOW 4
*/
int paddle_length, paddle_x, bx, by, bxv, byv, p_momentum, p_momentum_life, lives, blocks;
int breakout(int **grid, SDL_Event event, int t, int width, int height){
	const int speed_limit = 4;

	if(t == 1){
		paddle_length = width/5;
		paddle_x = 4*width/10;
		bx = width/2;
		by = height/2;
		bxv = 0;
		byv = 1;
		p_momentum = 0;
		p_momentum_life = 0;
		lives = 3;
		blocks = 0;
		for(int i = 0; i < width; i++){
			for(int j = 0; j < height/3; j++){
				switch(j/(height/9+1)){
				case 0:
					grid[i][j] = YELLOW;
					break;
				case 1:
					grid[i][j] = GREEN;
					break;
				case 2:
					grid[i][j] = BLUE;
					break;
				}
				blocks++;
			}
		}
	}

	//Erase old Ball and Paddle positions
	for(int i = 0; i < paddle_length; i++){
		grid[paddle_x+i][height-1] = EMPTY;
	}
	grid[bx][by] = EMPTY;

	//Retain the paddle's momentum for 3 frames
	if(p_momentum_life == 0){
		p_momentum = 0;
	} else {
		p_momentum_life--;
	}
	//Respond to user input
	if(event.type == SDL_KEYDOWN){
		switch( event.key.keysym.sym ){
		case SDLK_LEFT:
			if(paddle_x > 0){
				paddle_x--;
				p_momentum = -1;
				p_momentum_life = 3;
			}
			break;
		case SDLK_RIGHT:
			if(paddle_x < width-1-paddle_length){
				paddle_x++;
				p_momentum = 1;
				p_momentum_life = 3;
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
	if(bx < -bxv && bxv < 0){
		//Ball bounces off wall
		bxv *= -1;
		bx = 0;
	} else if(bx+1 > width-bxv && bxv > 0){
		bxv *= -1;
		bx = width-1;
	} else if(bx-1 >= 0 && grid[bx-1][by] >= BLOCK && bxv < 0){
		//Ball breaks block and bounces only horizontally
		grid[bx-1][by] = EMPTY;
		bxv *= -1;
		blocks--;
	} else if(bx+1 < width && grid[bx+1][by] >= BLOCK && bxv > 0){
		grid[bx+1][by] = EMPTY;
		bxv *= -1;
		blocks--;
	}

	//Ball breaks block and bounces vertically
	if(grid[bx][by-1] >= BLOCK && byv < 0){
		grid[bx][by-1] = EMPTY;
		byv *= -1;
		blocks--;
	} else if(bx-1 >= 0 && grid[bx-1][by-1] >= BLOCK && byv < 0 && bxv < 0){
		grid[bx-1][by-1] = EMPTY;
		bxv *= -1;
		byv *= -1;
		blocks--;
	} else if(bx+1 < width && grid[bx+1][by-1] >= BLOCK && byv < 0 && bxv > 0){
		grid[bx+1][by-1] = EMPTY;
		bxv *= -1;
		byv *= -1;
		blocks--;
	} else if(grid[bx][by+1] >= BLOCK && byv > 0){
		grid[bx][by+1] = EMPTY;
		byv *= -1;
		blocks--;
	} else if(bx-1 >= 0 && grid[bx-1][by+1] >= BLOCK && byv > 0 && bxv < 0){
		grid[bx-1][by+1] = EMPTY;
		bxv *= -1;
		byv *= -1;
		blocks--;
	} else if(bx+1 < width && grid[bx+1][by+1] >= BLOCK && byv > 0 && bxv > 0){
		grid[bx+1][by+1] = EMPTY;
		bxv *= -1;
		byv *= -1;
		blocks--;
	}

	//Lose a life if you miss the ball
	if(by+1 >= height && byv > 0){
		lives--;
		byv *= -1;
		printf("Lives Remaining: %d\n", lives);
	} else if(bx < paddle_x+paddle_length && bx+1 > paddle_x && by+1 > height-1-byv && byv > 0){
		//Ball bounces off of paddle
		bxv += p_momentum;
		byv *= -1;
		if(bxv > speed_limit){
			bxv = speed_limit;
		} else if(bxv < -speed_limit){
			bxv = -speed_limit;
		}
	} else if(bx < paddle_x+paddle_length && bx+2 > paddle_x && by+1 > height-1-byv && byv > 0 && bxv > 0){
		//Edge case diagonal ball at corner of paddle left
		bxv += p_momentum;
		byv *= -1;
		if(bxv > speed_limit){
			bxv = speed_limit;
		} else if(bxv < -speed_limit){
			bxv = -speed_limit;
		}
	} else if(bx < paddle_x+paddle_length+1 && bx+1 > paddle_x && by+1 > height-1-byv && byv > 0 && bxv < 0){
		//Edge case diagonal ball at corner of paddle right
		bxv += p_momentum;
		byv *= -1;
		if(bxv > speed_limit){
			bxv = speed_limit;
		} else if(bxv < -speed_limit){
			bxv = -speed_limit;
		}
	} else if(by == 0 && byv < 0){
		byv *= -1;
	}

	//Win Condition
	if(blocks == 0){
		printf("YOU WIN!\n");
		return ARCADE;
	} else if(lives == 0){
		printf("YOU LOSE!\n");
		printf("%d blocks remaining.\n", blocks);
		return ARCADE;
	}

	//Draw Ball and Paddle
	for(int i = 0; i < paddle_length; i++){
		grid[paddle_x+i][height-1] = PADDLE;
	}
	grid[bx][by] = BALL;

	return BREAKOUT;
}
