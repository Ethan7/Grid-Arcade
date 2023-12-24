/* By Ethan Hughes */
/* Written 7/21/2019 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include"arcade-defs.h"
/*
#define ARCADE 0
#define CHECKERS 12

#define EMPTY -1
#define P1CPAWN 0
#define P2CPAWN 1
#define P1CKING 2
#define P2CKING 3
#define P1MOVE 4
#define P2MOVE 5
*/
int selected_x, selected_y, player, player_king, player_move, 
opponent, opponent_king, opponent_move, p1_pieces, p2_pieces, follow_up;

//Determine all potential moves with given piece and display them
int moveset(int **grid, int width, int height, int i, int j, int recurs){
	int follow_ups = 0;
	if(grid[i][j] == player){
		if(player == P2CPAWN && j > 0){
			if(i > 0){
				if(grid[i-1][j-1] == opponent || grid[i-1][j-1] == opponent_king){
					if(i-1 > 0 && j-1 > 0 && grid[i-2][j-2] == EMPTY){
						grid[i-2][j-2] = player_move;
						follow_ups++;
					}
				} else if(grid[i-1][j-1] == EMPTY && recurs){
					grid[i-1][j-1] = player_move;
				}
			}
			if(i < width-1){
				if(grid[i+1][j-1] == opponent || grid[i+1][j-1] == opponent_king){
					if(i+1 < width-1 && j-1 > 0 && grid[i+2][j-2] == EMPTY){
						grid[i+2][j-2] = player_move;
						follow_ups++;
					}
				} else if(grid[i+1][j-1] == EMPTY && recurs){
					grid[i+1][j-1] = player_move;
				}
			}
		} else if(player == P1CPAWN && j < height-1){
			if(i > 0){
				if(grid[i-1][j+1] == opponent || grid[i-1][j+1] == opponent_king){
					if(i-1 > 0 && j+1 < height-1 && grid[i-2][j+2] == EMPTY){
						grid[i-2][j+2] = player_move;
						follow_ups++;
					}
				} else if(grid[i-1][j+1] == EMPTY && recurs){
					grid[i-1][j+1] = player_move;
				}
			}
			if(i < width-1){
				if(grid[i+1][j+1] == opponent || grid[i+1][j+1] == opponent_king){
					if(i+1 < width-1 && j+1 < height-1 && grid[i+2][j+2] == EMPTY){
						grid[i+2][j+2] = player_move;
						follow_ups++;
					}
				} else if(grid[i+1][j+1] == EMPTY && recurs){
					grid[i+1][j+1] = player_move;
				}
			}
		}
	} else if(grid[i][j] == player_king){
		if(j > 0){
			if(i > 0){
				if(grid[i-1][j-1] == opponent || grid[i-1][j-1] == opponent_king){
					if(i-1 > 0 && j-1 > 0 && grid[i-2][j-2] == EMPTY){
						grid[i-2][j-2] = player_move;
						follow_ups++;
					}
				} else if(grid[i-1][j-1] == EMPTY && recurs){
					grid[i-1][j-1] = player_move;
				}
			}
			if(i < width-1){
				if(grid[i+1][j-1] == opponent || grid[i+1][j-1] == opponent_king){
					if(i+1 < width-1 && j-1 > 0 && grid[i+2][j-2] == EMPTY){
						grid[i+2][j-2] = player_move;
						follow_ups++;
					}
				} else if(grid[i+1][j-1] == EMPTY && recurs){
					grid[i+1][j-1] = player_move;
				}
			}
		}
		if(j < height-1){
			if(i > 0){
				if(grid[i-1][j+1] == opponent || grid[i-1][j+1] == opponent_king){
					if(i-1 > 0 && j+1 < height-1 && grid[i-2][j+2] == EMPTY){
						grid[i-2][j+2] = player_move;
						follow_ups++;
					}
				} else if(grid[i-1][j+1] == EMPTY && recurs){
					grid[i-1][j+1] = player_move;
				}
			}
			if(i < width-1){
				if(grid[i+1][j+1] == opponent || grid[i+1][j+1] == opponent_king){
					if(i+1 < width-1 && j+1 < height-1 && grid[i+2][j+2] == EMPTY){
						grid[i+2][j+2] = player_move;
						follow_ups++;
					}
				} else if(grid[i+1][j+1] == EMPTY && recurs){
					grid[i+1][j+1] = player_move;
				}
			}
		}
	}
	return follow_ups;
}

int checkers(int **grid, SDL_Event event, int t, int cellsize, int width, int height){

	if(t == 1){
		follow_up = 0;
		selected_x = -1;
		selected_y = -1;
		player = P1CPAWN;
		player_king = P1CKING;
		player_move = P1MOVE;
		opponent = P2CPAWN;
		opponent_king = P2CKING;
		opponent_move = P2MOVE;
		p1_pieces = 0;
		p2_pieces = 0;
		//Setup initial board pieces
		for(int i = 0; i < width; i++){
			if(i % 2 == 0){
				grid[i][0] = P1CPAWN;
				grid[i][2] = P1CPAWN;
				grid[i][height-2] = P2CPAWN;
				p1_pieces += 2;
				p2_pieces += 1;
			} else {
				grid[i][1] = P1CPAWN;
				grid[i][height-1] = P2CPAWN;
				grid[i][height-3] = P2CPAWN;
				p1_pieces += 1;
				p2_pieces += 2;
			}
		}
	}

	//Respond to user input
	if(event.type == SDL_MOUSEBUTTONUP){
		int buttonx = event.button.x / cellsize;
		int buttony = event.button.y / cellsize;
		if(event.button.button == SDL_BUTTON_LEFT && buttonx < width && 
			buttony < height && buttonx > -1 && buttony > -1){
			//Make potential move
			if(grid[buttonx][buttony] == player_move){
				//Move piece
				grid[buttonx][buttony] = grid[selected_x][selected_y];
				grid[selected_x][selected_y] = EMPTY;
				if(grid[buttonx][buttony] == P1CPAWN && buttony == height-1){
					grid[buttonx][buttony] = P1CKING;
				} else if(grid[buttonx][buttony] == P2CPAWN && buttony == 0){
					grid[buttonx][buttony] = P2CKING;
				}
				//Erase other potential moves
				for(int i = 0; i < width; i++){
					for(int j = 0; j < height; j++){
						if(grid[i][j] == player_move || grid[i][j] == opponent_move){
							grid[i][j] = EMPTY;
						}
					}
				}
				//Take piece if you jumped over it
				follow_up = 0;
				int takenx = (buttonx+selected_x)/2;
				int takeny = (buttony+selected_y)/2;
				if(grid[takenx][takeny] == opponent || grid[takenx][takeny] == opponent_king){
					grid[takenx][takeny] = EMPTY;
					if(opponent == P1CPAWN){
						p1_pieces--;
					} else {
						p2_pieces--;
					}
					//Check for follow_up moves
					follow_up = moveset(grid, width, height, buttonx, buttony, 0);
				}

				//Switch turns if there's no follow_up moves
				if(follow_up == 0){
					selected_x = -1;
					selected_y = -1;
					int temp1 = player;
					int temp2 = player_king;
					int temp3 = player_move;
					player = opponent;
					player_king = opponent_king;
					player_move = opponent_move;
					opponent = temp1;
					opponent_king = temp2;
					opponent_move = temp3;
				} else {
					selected_x = buttonx;
					selected_y = buttony;
				}
			} else if(selected_x != -1){
				//Clear potential moves
				for(int i = 0; i < width; i++){
					for(int j = 0; j < height; j++){
						if(grid[i][j] == player_move || grid[i][j] == opponent_move){
							grid[i][j] = EMPTY;
						}
					}
				}

				selected_x = -1;
				selected_y = -1;

				//Switch turns if you decided against a follow_up move
				if(follow_up != 0){
					follow_up = 0;
					int temp1 = player;
					int temp2 = player_king;
					int temp3 = player_move;
					player = opponent;
					player_king = opponent_king;
					player_move = opponent_move;
					opponent = temp1;
					opponent_king = temp2;
					opponent_move = temp3;
				}
			}
			//Look at potential moves
			if(follow_up == 0){
				if(grid[buttonx][buttony] == player || grid[buttonx][buttony] == player_king){
					selected_x = buttonx;
					selected_y = buttony;
					moveset(grid, width, height, selected_x, selected_y, 1);
				}
			}
		}
	}

	//Player with last remaining pieces wins
	if(p1_pieces == 0){
		printf("PLAYER 2 WINS!\n");
		return ARCADE;
	} else if(p2_pieces == 0){
		printf("PLAYER 1 WINS!\n");
		return ARCADE;
	}

	return CHECKERS;
}