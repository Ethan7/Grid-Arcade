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
#define P1MOVE 4
#define P2MOVE 5

int selectedx, selectedy, player, playerking, playermove, opponent, opponentking, opponentmove, p1pieces, p2pieces, followup;

//Determine all potential moves with given piece and display them
int moveset(int **grid, int width, int height, int i, int j, int recurs){
	int followups = 0;
	if(grid[i][j] == player){
		if(player == P2PAWN && j > 0){
			if(i > 0){
				if(grid[i-1][j-1] == opponent || grid[i-1][j-1] == opponentking){
					if(i-1 > 0 && j-1 > 0 && grid[i-2][j-2] == EMPTY){
						grid[i-2][j-2] = playermove;
						followups++;
					}
				} else if(grid[i-1][j-1] == EMPTY && recurs){
					grid[i-1][j-1] = playermove;
				}
			}
			if(i < width-1){
				if(grid[i+1][j-1] == opponent || grid[i+1][j-1] == opponentking){
					if(i+1 < width-1 && j-1 > 0 && grid[i+2][j-2] == EMPTY){
						grid[i+2][j-2] = playermove;
						followups++;
					}
				} else if(grid[i+1][j-1] == EMPTY && recurs){
					grid[i+1][j-1] = playermove;
				}
			}
		} else if(player == P1PAWN && j < height-1){
			if(i > 0){
				if(grid[i-1][j+1] == opponent || grid[i-1][j+1] == opponentking){
					if(i-1 > 0 && j+1 < height-1 && grid[i-2][j+2] == EMPTY){
						grid[i-2][j+2] = playermove;
						followups++;
					}
				} else if(grid[i-1][j+1] == EMPTY && recurs){
					grid[i-1][j+1] = playermove;
				}
			}
			if(i < width-1){
				if(grid[i+1][j+1] == opponent || grid[i+1][j+1] == opponentking){
					if(i+1 < width-1 && j+1 < height-1 && grid[i+2][j+2] == EMPTY){
						grid[i+2][j+2] = playermove;
						followups++;
					}
				} else if(grid[i+1][j+1] == EMPTY && recurs){
					grid[i+1][j+1] = playermove;
				}
			}
		}
	} else if(grid[i][j] == playerking){
		if(j > 0){
			if(i > 0){
				if(grid[i-1][j-1] == opponent || grid[i-1][j-1] == opponentking){
					if(i-1 > 0 && j-1 > 0 && grid[i-2][j-2] == EMPTY){
						grid[i-2][j-2] = playermove;
						followups++;
					}
				} else if(grid[i-1][j-1] == EMPTY && recurs){
					grid[i-1][j-1] = playermove;
				}
			}
			if(i < width-1){
				if(grid[i+1][j-1] == opponent || grid[i+1][j-1] == opponentking){
					if(i+1 < width-1 && j-1 > 0 && grid[i+2][j-2] == EMPTY){
						grid[i+2][j-2] = playermove;
						followups++;
					}
				} else if(grid[i+1][j-1] == EMPTY && recurs){
					grid[i+1][j-1] = playermove;
				}
			}
		}
		if(j < height-1){
			if(i > 0){
				if(grid[i-1][j+1] == opponent || grid[i-1][j+1] == opponentking){
					if(i-1 > 0 && j+1 < height-1 && grid[i-2][j+2] == EMPTY){
						grid[i-2][j+2] = playermove;
						followups++;
					}
				} else if(grid[i-1][j+1] == EMPTY && recurs){
					grid[i-1][j+1] = playermove;
				}
			}
			if(i < width-1){
				if(grid[i+1][j+1] == opponent || grid[i+1][j+1] == opponentking){
					if(i+1 < width-1 && j+1 < height-1 && grid[i+2][j+2] == EMPTY){
						grid[i+2][j+2] = playermove;
						followups++;
					}
				} else if(grid[i+1][j+1] == EMPTY && recurs){
					grid[i+1][j+1] = playermove;
				}
			}
		}
	}
	return followups;
}

int checkers(int **grid, SDL_Event eventbutton, int game, int t, int cellsize, int width, int height){

	if(t == 1){
		followup = 0;
		selectedx = -1;
		selectedy = -1;
		player = P1PAWN;
		playerking = P1KING;
		playermove = P1MOVE;
		opponent = P2PAWN;
		opponentking = P2KING;
		opponentmove = P2MOVE;
		p1pieces = 0;
		p2pieces = 0;
		//Setup initial board pieces
		for(int i = 0; i < width; i++){
			if(i % 2 == 0){
				grid[i][0] = P1PAWN;
				grid[i][2] = P1PAWN;
				grid[i][height-2] = P2PAWN;
				p1pieces += 2;
				p2pieces += 1;
			} else {
				grid[i][1] = P1PAWN;
				grid[i][height-1] = P2PAWN;
				grid[i][height-3] = P2PAWN;
				p1pieces += 1;
				p2pieces += 2;
			}
		}
	}

	//Respond to user input
	if(eventbutton.type == SDL_MOUSEBUTTONUP){
		int buttonx = eventbutton.button.x / cellsize;
		int buttony = eventbutton.button.y / cellsize;
		if(eventbutton.button.button == SDL_BUTTON_LEFT && buttonx < width && buttony < height && buttonx > -1 && buttony > -1){
			//Make potential move
			if(grid[buttonx][buttony] == playermove){
				//Move piece
				grid[buttonx][buttony] = grid[selectedx][selectedy];
				grid[selectedx][selectedy] = EMPTY;
				if(grid[buttonx][buttony] == P1PAWN && buttony == height-1){
					grid[buttonx][buttony] = P1KING;
				} else if(grid[buttonx][buttony] == P2PAWN && buttony == 0){
					grid[buttonx][buttony] = P2KING;
				}
				//Erase other potential moves
				for(int i = 0; i < width; i++){
					for(int j = 0; j < height; j++){
						if(grid[i][j] == playermove || grid[i][j] == opponentmove){
							grid[i][j] = EMPTY;
						}
					}
				}
				//Take piece if you jumped over it
				followup = 0;
				int takenx = (buttonx+selectedx)/2;
				int takeny = (buttony+selectedy)/2;
				if(grid[takenx][takeny] == opponent || grid[takenx][takeny] == opponentking){
					grid[takenx][takeny] = EMPTY;
					if(opponent == P1PAWN){
						p1pieces--;
					} else {
						p2pieces--;
					}
					//Check for followup moves
					followup = moveset(grid, width, height, buttonx, buttony, 0);
				}

				//Switch turns if there's no followup moves
				if(followup == 0){
					selectedx = -1;
					selectedy = -1;
					int temp1 = player;
					int temp2 = playerking;
					int temp3 = playermove;
					player = opponent;
					playerking = opponentking;
					playermove = opponentmove;
					opponent = temp1;
					opponentking = temp2;
					opponentmove = temp3;
				} else {
					selectedx = buttonx;
					selectedy = buttony;
				}
			} else if(selectedx != -1){
				//Clear potential moves
				for(int i = 0; i < width; i++){
					for(int j = 0; j < height; j++){
						if(grid[i][j] == playermove || grid[i][j] == opponentmove){
							grid[i][j] = EMPTY;
						}
					}
				}

				selectedx = -1;
				selectedy = -1;

				//Switch turns if you decided against a followup move
				if(followup != 0){
					followup = 0;
					int temp1 = player;
					int temp2 = playerking;
					int temp3 = playermove;
					player = opponent;
					playerking = opponentking;
					playermove = opponentmove;
					opponent = temp1;
					opponentking = temp2;
					opponentmove = temp3;
				}
			}
			//Look at potential moves
			if(followup == 0){
				if(grid[buttonx][buttony] == player || grid[buttonx][buttony] == playerking){
					selectedx = buttonx;
					selectedy = buttony;
					moveset(grid, width, height, selectedx, selectedy, 1);
				}
			}
		}
	}

	//Player with last remaining pieces wins
	if(p1pieces == 0){
		printf("PLAYER 2 WINS!\n");
		return ARCADE;
	} else if(p2pieces == 0){
		printf("PLAYER 1 WINS!\n");
		return ARCADE;
	}

	return CHECKERS;
}