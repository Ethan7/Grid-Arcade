/* By Ethan Hughes */
/* Written 7/20/2019 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>

#define ARCADE 0
#define CONNECT4 11

#define EMPTY -1
#define PLAYER1 0
#define PLAYER2 1

//Test 4x4 diagonals, verticals, and horizontals for win condition
int wintest(int i, int j, int **grid, int player){
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

int placerx, player;
int connect4(int **grid, SDL_Event event, int game, int t, int width, int height){

	if(t == 1){
		placerx = width/2;
		player = PLAYER1;
	}
	grid[placerx][0] = EMPTY;

	int movemade = -1;
	//Respond to user input
	if(event.type == SDL_KEYDOWN){
		switch( event.key.keysym.sym ){
		case SDLK_LEFT:
			if(placerx > 0){
				placerx--;
			}
			break;
		case SDLK_RIGHT:
			if(placerx < width-1){
				placerx++;
			}
			break;
		case SDLK_DOWN:
			if(grid[placerx][1] == EMPTY){
				int flag = 0;
				for(int i = 1; i < height-1; i++){
					if(grid[placerx][i+1] != EMPTY){
						grid[placerx][i] = player;
						if(player == PLAYER1){
							player = PLAYER2;
						} else {
							player = PLAYER1;
						}
						flag = 1;
						break;
					}
				}
				if(flag == 0){
					grid[placerx][height-1] = player;
					if(player == PLAYER1){
						player = PLAYER2;
					} else {
						player = PLAYER1;
					}
				}
				movemade = CONNECT4;
			}
			break;
		}
	}

	grid[placerx][0] = player;

	//Check for a win after a player's turn
	if(movemade == CONNECT4){
		for(int i = 0; i < width-3; i++){
			for(int j = 1; j < height-3; j++){
				if(player == PLAYER1){
					movemade = wintest(i, j, grid, PLAYER2);
				} else {
					movemade = wintest(i, j, grid, PLAYER1);
				}
				if(movemade == ARCADE){
					return ARCADE;
				}
			}
		}
	}

	return CONNECT4;
}