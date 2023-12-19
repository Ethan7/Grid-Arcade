/* By Ethan Hughes */
/* Written 7/20/2019 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include"arcade-defs.h"
/*
#define ARCADE 0
#define CONNECT4 11

#define EMPTY -1
#define PLAYER1 0
#define PLAYER2 1
*/
//Test 4x4 diagonals, verticals, and horizontals for win condition
int win_test(int i, int j, int **grid, int player){
	if(grid[i][j] == player && grid[i+1][j+1] == player && grid[i+2][j+2] == player && grid[i+3][j+3] == player){
		printf("PLAYER %d WINS!\n", player+1);
		return ARCADE;
	} else if(grid[i][j] == player && grid[i+1][j] == player && grid[i+2][j] == player && grid[i+3][j] == player){
		printf("PLAYER %d WINS!\n", player+1);
		return ARCADE;
	} else if(grid[i][j+1] == player && grid[i+1][j+1] == player && grid[i+2][j+1] == player && grid[i+3][j+1] == player){
		printf("PLAYER %d WINS!\n", player+1);
		return ARCADE;
	} else if(grid[i][j+2] == player && grid[i+1][j+2] == player && grid[i+2][j+2] == player && grid[i+3][j+2] == player){
		printf("PLAYER %d WINS!\n", player+1);
		return ARCADE;
	} else if(grid[i][j+3] == player && grid[i+1][j+3] == player && grid[i+2][j+3] == player && grid[i+3][j+3] == player){
		printf("PLAYER %d WINS!\n", player+1);
		return ARCADE;
	} else if(grid[i][j] == player && grid[i][j+1] == player && grid[i][j+2] == player && grid[i][j+3] == player){
		printf("PLAYER %d WINS!\n", player+1);
		return ARCADE;
	} else if(grid[i+1][j] == player && grid[i+1][j+1] == player && grid[i+1][j+2] == player && grid[i+1][j+3] == player){
		printf("PLAYER %d WINS!\n", player+1);
		return ARCADE;
	} else if(grid[i+2][j] == player && grid[i+2][j+1] == player && grid[i+2][j+2] == player && grid[i+2][j+3] == player){
		printf("PLAYER %d WINS!\n", player+1);
		return ARCADE;
	} else if(grid[i+3][j] == player && grid[i+3][j+1] == player && grid[i+3][j+2] == player && grid[i+3][j+3] == player){
		printf("PLAYER %d WINS!\n", player+1);
		return ARCADE;
	} else if(grid[i+3][j] == player && grid[i+2][j+1] == player && grid[i+1][j+2] == player && grid[i][j+3] == player){
		printf("PLAYER %d WINS!\n", player+1);
		return ARCADE;
	} else {
		return CONNECT4;
	}
}

int placer_x, player_turn;
int connect4(int **grid, SDL_Event event, int t, int width, int height){

	if(t == 1){
		placer_x = width/2;
		player_turn = PLAYER1;
	}
	grid[placer_x][0] = EMPTY;

	int move_made = -1;
	//Respond to user input
	if(event.type == SDL_KEYDOWN){
		switch( event.key.keysym.sym ){
		case SDLK_LEFT:
			if(placer_x > 0){
				placer_x--;
			}
			break;
		case SDLK_RIGHT:
			if(placer_x < width-1){
				placer_x++;
			}
			break;
		case SDLK_DOWN:
			if(grid[placer_x][1] == EMPTY){
				int flag = 0;
				for(int i = 1; i < height-1; i++){
					if(grid[placer_x][i+1] != EMPTY){
						grid[placer_x][i] = player_turn;
						if(player_turn == PLAYER1){
							player_turn = PLAYER2;
						} else {
							player_turn = PLAYER1;
						}
						flag = 1;
						break;
					}
				}
				if(flag == 0){
					grid[placer_x][height-1] = player_turn;
					if(player_turn == PLAYER1){
						player_turn = PLAYER2;
					} else {
						player_turn = PLAYER1;
					}
				}
				move_made = CONNECT4;
			}
			break;
		}
	}

	grid[placer_x][0] = player_turn;

	//Check for a win after a player's turn
	if(move_made == CONNECT4){
		for(int i = 0; i < width-3; i++){
			for(int j = 1; j < height-3; j++){
				if(player_turn == PLAYER1){
					move_made = win_test(i, j, grid, PLAYER2);
				} else {
					move_made = win_test(i, j, grid, PLAYER1);
				}
				if(move_made == ARCADE){
					return ARCADE;
				}
			}
		}
	}

	return CONNECT4;
}