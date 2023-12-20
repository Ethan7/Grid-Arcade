/* By Ethan Hughes */
/* Written 3/5/2020 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include"arcade-defs.h"
/*
#define ARCADE 0
#define CHESS 14

#define EMPTY 0
#define P1PAWN 17
#define P1ROOK 18
#define P1BISHOP 19
#define P1KNIGHT 20
#define P1QUEEN 21
#define P1KING 22
#define P2PAWN 23
#define P2ROOK 24
#define P2BISHOP 25
#define P2KNIGHT 26
#define P2QUEEN 27
#define P2KING 28
#define PMOVE 2
*/
int chosen_x, chosen_y, p1_castle1, p1_castle2, p2_castle1, p2_castle2, turn, p1_king_x, p1_king_y, p2_king_x, p2_king_y, p1_enpassant, p2_enpassant;
int move_grid[8][8];

//Deternube if after the given move your king is left exposed
int king_at_risk(int **grid, int before_x, int before_y, int after_x, int after_y, int p_turn){
	const int placemarker = grid[after_x][after_y];
	grid[after_x][after_y] = grid[before_x][before_y];
	grid[before_x][before_y] = EMPTY;
	int king_x = 0;
	int king_y = 0;
	//Get coords of king we are checking
	if(p_turn){
		king_x = p2_king_x;
		king_y = p2_king_y;
		if(grid[p2_king_x][p2_king_y] != P2KING){
			king_x = after_x;
			king_y = after_y;
		}
	} else {
		king_x = p1_king_x;
		king_y = p1_king_y;
		if(grid[p1_king_x][p1_king_y] != P1KING){
			king_x = after_x;
			king_y = after_y;
		}
	}
	for(int x = 1; x < 8; x++){
		if(king_x+x > 7 || (grid[king_x+x][king_y]-17) / 6 == p_turn){
			break;
		} else if((grid[king_x+x][king_y]-17) % 6 == P1ROOK-17 || (grid[king_x+x][king_y]-17) % 6 == P1QUEEN-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		} else if((grid[king_x+x][king_y]-17) / 6 > -1){
			break;
		}
	}
	for(int x = 1; x < 8; x++){
		if(king_x-x < 0 || (grid[king_x-x][king_y]-17) / 6 == p_turn){
			break;
		} else if((grid[king_x-x][king_y]-17) % 6 == P1ROOK-17 || (grid[king_x-x][king_y]-17) % 6 == P1QUEEN-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		} else if((grid[king_x-x][king_y]-17) / 6 > -1){
			break;
		}
	}
	for(int x = 1; x < 8; x++){
		if(king_y+x > 7 || (grid[king_x][king_y+x]-17) / 6 == p_turn){
			break;
		} else if((grid[king_x][king_y+x]-17) % 6 == P1ROOK-17 || (grid[king_x][king_y+x]-17) % 6 == P1QUEEN-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		} else if((grid[king_x][king_y+x]-17) / 6 > -1){
			break;
		}
	}
	for(int x = 1; x < 8; x++){
		if(king_y-x < 0 || (grid[king_x][king_y-x]-17) / 6 == p_turn){
			break;
		} else if((grid[king_x][king_y-x]-17) % 6 == P1ROOK-17 || (grid[king_x][king_y-x]-17) % 6 == P1QUEEN-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		} else if((grid[king_x][king_y-x]-17) / 6 > -1){
			break;
		}
	}
	for(int x = 1; x < 8; x++){
		if(king_x+x > 7 || king_y+x > 7 || (grid[king_x+x][king_y+x]-17) / 6 == p_turn){
			break;
		} else if((grid[king_x+x][king_y+x]-17) % 6 == P1BISHOP-17 || (grid[king_x+x][king_y+x]-17) % 6 == P1QUEEN-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		} else if((grid[king_x+x][king_y+x]-17) / 6 > -1){
			break;
		}
	}
	for(int x = 1; x < 8; x++){
		if(king_x-x < 0 || king_y+x > 7 || (grid[king_x-x][king_y+x]-17) / 6 == p_turn){
			break;
		} else if((grid[king_x-x][king_y+x]-17) % 6 == P1BISHOP-17 || (grid[king_x-x][king_y+x]-17) % 6 == P1QUEEN-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		} else if((grid[king_x-x][king_y+x]-17) / 6 > -1){
			break;
		}
	}
	for(int x = 1; x < 8; x++){
		if(king_x+x > 7 || king_y-x < 0 || (grid[king_x+x][king_y-x]-17) / 6 == p_turn){
			break;
		} else if((grid[king_x+x][king_y-x]-17) % 6 == P1BISHOP-17 || (grid[king_x+x][king_y-x]-17) % 6 == P1QUEEN-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		} else if((grid[king_x+x][king_y-x]-17) / 6 > -1){
			break;
		}
	}
	for(int x = 1; x < 8; x++){
		if(king_x-x < 0 || king_y-x < 0 || (grid[king_x-x][king_y-x]-17) / 6 == p_turn){
			break;
		} else if((grid[king_x-x][king_y-x]-17) % 6 == P1BISHOP-17 || (grid[king_x-x][king_y-x]-17) % 6 == P1QUEEN-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		} else if((grid[king_x-x][king_y-x]-17) / 6 > -1){
			break;
		}
	}
	if(king_x+2 < 8 && king_y+1 < 8 && (grid[king_x+2][king_y+1]-17) / 6 != p_turn){
		if((grid[king_x+2][king_y+1]-17) % 6 == P1KNIGHT-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		}
	}
	if(king_x+2 < 8 && king_y-1 > -1 && (grid[king_x+2][king_y-1]-17) / 6 != p_turn){
		if((grid[king_x+2][king_y-1]-17) % 6 == P1KNIGHT-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		}
	}
	if(king_x+1 < 8 && king_y+2 < 8 && (grid[king_x+1][king_y+2]-17) / 6 != p_turn){
		if((grid[king_x+1][king_y+2]-17) % 6 == P1KNIGHT-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		}
	}
	if(king_x+1 < 8 && king_y-2 > -1 && (grid[king_x+1][king_y-2]-17) / 6 != p_turn){
		if((grid[king_x+1][king_y-2]-17) % 6 == P1KNIGHT-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		}
	}
	if(king_x-2 > -1 && king_y+1 < 8 && (grid[king_x-2][king_y+1]-17) / 6 != p_turn){
		if((grid[king_x-2][king_y+1]-17) % 6 == P1KNIGHT-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		}
	}
	if(king_x-2 > -1 && king_y-1 > -1 && (grid[king_x-2][king_y-1]-17) / 6 != p_turn){
		if((grid[king_x-2][king_y-1]-17) % 6 == P1KNIGHT-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		}
	}
	if(king_x-1 > -1 && king_y+2 < 8 && (grid[king_x-1][king_y+2]-17) / 6 != p_turn){
		if((grid[king_x-1][king_y+2]-17) % 6 == P1KNIGHT-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		}
	}
	if(king_x-1 > -1 && king_y-2 > -1 && (grid[king_x-1][king_y-2]-17) / 6 != p_turn){
		if((grid[king_x-1][king_y-2]-17) % 6 == P1KNIGHT-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		}
	}
	if(king_x+1 < 8 && (grid[king_x+1][king_y]-17) / 6 != p_turn){
		if((grid[king_x+1][king_y]-17) % 6 == P1KING-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		}
	}
	if(king_x-1 > -1 && (grid[king_x-1][king_y]-17) / 6 != p_turn){
		if((grid[king_x-1][king_y]-17) % 6 == P1KING-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		}
	}
	if(king_y+1 < 8 && (grid[king_x][king_y+1]-17) / 6 != p_turn){
		if((grid[king_x][king_y+1]-17) % 6 == P1KING-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		}
	}
	if(king_y-1 > -1 && (grid[king_x][king_y-1]-17) / 6 != p_turn){
		if((grid[king_x][king_y-1]-17) % 6 == P1KING-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		}
	}
	if(king_x+1 < 8 && king_y+1 < 8 && (grid[king_x+1][king_y+1]-17) / 6 != p_turn){
		if((grid[king_x+1][king_y+1]-17) % 6 == P1KING-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		}
		if(p_turn == 0 && grid[king_x+1][king_y+1] == P2PAWN){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		}
	}
	if(king_x+1 < 8 && king_y-1 > -1 && (grid[king_x+1][king_y-1]-17) / 6 != p_turn){
		if((grid[king_x+1][king_y-1]-17) % 6 == P1KING-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		}
		if(p_turn == 1 && grid[king_x+1][king_y-1] == P1PAWN){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		}
	}
	if(king_x-1 > -1 && king_y+1 < 8 && (grid[king_x-1][king_y+1]-17) / 6 != p_turn){
		if((grid[king_x-1][king_y+1]-17) % 6 == P1KING-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		}
		if(p_turn == 0 && grid[king_x-1][king_y+1] == P2PAWN){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		}
	}
	if(king_x-1 > -1 && king_y-1 > -1 && (grid[king_x-1][king_y-1]-17) / 6 != p_turn){
		if((grid[king_x-1][king_y-1]-17) % 6 == P1KING-17){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		}
		if(p_turn == 1 && grid[king_x-1][king_y-1] == P1PAWN){
			grid[before_x][before_y] = grid[after_x][after_y];
			grid[after_x][after_y] = placemarker;
			return 1;
		}
	}
	grid[before_x][before_y] = grid[after_x][after_y];
	grid[after_x][after_y] = placemarker;
	return 0;
}

//Determine all potential moves with given piece and display them
int get_moves(int **grid, int i, int j, int p_turn){
	int moves = 0;

	switch(((grid[i][j]-17) % 6)+17){
		case P1PAWN:
			if(grid[i][j] == P1PAWN){
				if(grid[i][j+1] == EMPTY){
					if(!king_at_risk(grid, i, j, i, j+1, p_turn)){
						move_grid[i][j+1] = PMOVE;
						grid[i][j+1] = PMOVE;
						moves++;
					}
					if(j == 1 && grid[i][3] == EMPTY){
						if(!king_at_risk(grid, i, j, i, 3, p_turn)){
							move_grid[i][3] = PMOVE;
							grid[i][3] = PMOVE;
							moves++;
						}
					}
				}
				if(i > 0 && grid[i-1][j+1] / 23 == 1){
					if(!king_at_risk(grid, i, j, i-1, j+1, p_turn)){
						move_grid[i-1][j+1] = PMOVE;
						moves++;
					}
				}
				if(i < 7 && grid[i+1][j+1] / 23 == 1){
					if(!king_at_risk(grid, i, j, i+1, j+1, p_turn)){
						move_grid[i+1][j+1] = PMOVE;
						moves++;
					}
				}
				if(i > 0 && i-1 == p2_enpassant && j == 4){
					if(!king_at_risk(grid, i, j, i-1, 5, p_turn)){
						move_grid[i-1][5] = PMOVE;
						grid[i-1][5] = PMOVE;
						moves++;
					}
				}
				if(i+1 == p2_enpassant && j == 4){
					if(!king_at_risk(grid, i, j, i+1, 5, p_turn)){
						move_grid[i+1][5] = PMOVE;
						grid[i+1][5] = PMOVE;
						moves++;
					}
				}
			} else {
				if(grid[i][j-1] == EMPTY){
					if(!king_at_risk(grid, i, j, i, j-1, p_turn)){
						move_grid[i][j-1] = PMOVE;
						grid[i][j-1] = PMOVE;
						moves++;
					}
					if(j == 6 && grid[i][4] == EMPTY){
						if(!king_at_risk(grid, i, j, i, 4, p_turn)){
							move_grid[i][4] = PMOVE;
							grid[i][4] = PMOVE;
							moves++;
						}
					}
				}
				if(i > 0 && (grid[i-1][j-1]-17) / 6 == 0){
					if(!king_at_risk(grid, i, j, i-1, j-1, p_turn)){
						move_grid[i-1][j-1] = PMOVE;
						moves++;
					}
				}
				if(i < 7 && (grid[i+1][j-1]-17) / 6 == 0){
					if(!king_at_risk(grid, i, j, i+1, j-1, p_turn)){
						move_grid[i+1][j-1] = PMOVE;
						moves++;
					}
				}
				if(i > 0 && i-1 == p1_enpassant && j == 3){
					if(!king_at_risk(grid, i, j, i-1, 2, p_turn)){
						move_grid[i-1][2] = PMOVE;
						grid[i-1][2] = PMOVE;
						moves++;
					}
				}
				if(i+1 == p1_enpassant && j == 3){
					if(!king_at_risk(grid, i, j, i+1, 2, p_turn)){
						move_grid[i+1][2] = PMOVE;
						grid[i+1][2] = PMOVE;
						moves++;
					}
				}
			}
			break;
		case P1ROOK:
			for(int x = 1; x < 8; x++){
				if(i+x > 7 || (grid[i+x][j]-17) / 6 == p_turn){
					break;
				} else if(grid[i+x][j] != EMPTY){
					if(!king_at_risk(grid, i, j, i+x, j, p_turn)){
						move_grid[i+x][j] = PMOVE;
						moves++;
					}
					break;
				}
				if(!king_at_risk(grid, i, j, i+x, j, p_turn)){
					move_grid[i+x][j] = PMOVE;
					grid[i+x][j] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(i-x < 0 || (grid[i-x][j]-17) / 6 == p_turn){
					break;
				} else if(grid[i-x][j] != EMPTY){
					if(!king_at_risk(grid, i, j, i-x, j, p_turn)){
						move_grid[i-x][j] = PMOVE;
						moves++;
					}
					break;
				}
				if(!king_at_risk(grid, i, j, i-x, j, p_turn)){
					move_grid[i-x][j] = PMOVE;
					grid[i-x][j] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(j+x > 7 || (grid[i][j+x]-17) / 6 == p_turn){
					break;
				} else if(grid[i][j+x] != EMPTY){
					if(!king_at_risk(grid, i, j, i, j+x, p_turn)){
						move_grid[i][j+x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!king_at_risk(grid, i, j, i, j+x, p_turn)){
					move_grid[i][j+x] = PMOVE;
					grid[i][j+x] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(j-x < 0 || (grid[i][j-x]-17) / 6 == p_turn){
					break;
				} else if(grid[i][j-x] != EMPTY){
					if(!king_at_risk(grid, i, j, i, j-x, p_turn)){
						move_grid[i][j-x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!king_at_risk(grid, i, j, i, j-x, p_turn)){
					move_grid[i][j-x] = PMOVE;
					grid[i][j-x] = PMOVE;
					moves++;
				}
			}
			break;
		case P1BISHOP:
			for(int x = 1; x < 8; x++){
				if(i+x > 7 || j+x > 7 || (grid[i+x][j+x]-17) / 6 == p_turn){
					break;
				} else if(grid[i+x][j+x] != EMPTY){
					if(!king_at_risk(grid, i, j, i+x, j+x, p_turn)){
						move_grid[i+x][j+x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!king_at_risk(grid, i, j, i+x, j+x, p_turn)){
					move_grid[i+x][j+x] = PMOVE;
					grid[i+x][j+x] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(i-x < 0 || j+x > 7 || (grid[i-x][j+x]-17) / 6 == p_turn){
					break;
				} else if(grid[i-x][j+x] != EMPTY){
					if(!king_at_risk(grid, i, j, i-x, j+x, p_turn)){
						move_grid[i-x][j+x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!king_at_risk(grid, i, j, i-x, j+x, p_turn)){
					move_grid[i-x][j+x] = PMOVE;
					grid[i-x][j+x] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(i+x > 7 || j-x < 0 || (grid[i+x][j-x]-17) / 6 == p_turn){
					break;
				} else if(grid[i+x][j-x] != EMPTY){
					if(!king_at_risk(grid, i, j, i+x, j-x, p_turn)){
						move_grid[i+x][j-x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!king_at_risk(grid, i, j, i+x, j-x, p_turn)){
					move_grid[i+x][j-x] = PMOVE;
					grid[i+x][j-x] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(i-x < 0 || j-x < 0 || (grid[i-x][j-x]-17) / 6 == p_turn){
					break;
				} else if(grid[i-x][j-x] != EMPTY){
					if(!king_at_risk(grid, i, j, i-x, j-x, p_turn)){
						move_grid[i-x][j-x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!king_at_risk(grid, i, j, i-x, j-x, p_turn)){
					move_grid[i-x][j-x] = PMOVE;
					grid[i-x][j-x] = PMOVE;
					moves++;
				}
			}
			break;
		case P1KNIGHT:
			if(i+2 < 8 && j+1 < 8 && (grid[i+2][j+1]-17) / 6 != p_turn){
				if(!king_at_risk(grid, i, j, i+2, j+1, p_turn)){
					move_grid[i+2][j+1] = PMOVE;
					moves++;
					if(grid[i+2][j+1] == EMPTY){
						grid[i+2][j+1] = PMOVE;
					}
				}
			}
			if(i+2 < 8 && j-1 > -1 && (grid[i+2][j-1]-17) / 6 != p_turn){
				if(!king_at_risk(grid, i, j, i+2, j-1, p_turn)){
					move_grid[i+2][j-1] = PMOVE;
					moves++;
					if(grid[i+2][j-1] == EMPTY){
						grid[i+2][j-1] = PMOVE;
					}
				}
			}
			if(i+1 < 8 && j+2 < 8 && (grid[i+1][j+2]-17) / 6 != p_turn){
				if(!king_at_risk(grid, i, j, i+1, j+2, p_turn)){
					move_grid[i+1][j+2] = PMOVE;
					moves++;
					if(grid[i+1][j+2] == EMPTY){
						grid[i+1][j+2] = PMOVE;
					}
				}
			}
			if(i+1 < 8 && j-2 > -1 && (grid[i+1][j-2]-17) / 6 != p_turn){
				if(!king_at_risk(grid, i, j, i+1, j-2, p_turn)){
					move_grid[i+1][j-2] = PMOVE;
					moves++;
					if(grid[i+1][j-2] == EMPTY){
						grid[i+1][j-2] = PMOVE;
					}
				}
			}
			if(i-2 > -1 && j+1 < 8 && (grid[i-2][j+1]-17) / 6 != p_turn){
				if(!king_at_risk(grid, i, j, i-2, j+1, p_turn)){
					move_grid[i-2][j+1] = PMOVE;
					moves++;
					if(grid[i-2][j+1] == EMPTY){
						grid[i-2][j+1] = PMOVE;
					}
				}
			}
			if(i-2 > -1 && j-1 > -1 && (grid[i-2][j-1]-17) / 6 != p_turn){
				if(!king_at_risk(grid, i, j, i-2, j-1, p_turn)){
					move_grid[i-2][j-1] = PMOVE;
					moves++;
					if(grid[i-2][j-1] == EMPTY){
						grid[i-2][j-1] = PMOVE;
					}
				}
			}
			if(i-1 > -1 && j+2 < 8 && (grid[i-1][j+2]-17) / 6 != p_turn){
				if(!king_at_risk(grid, i, j, i-1, j+2, p_turn)){
					move_grid[i-1][j+2] = PMOVE;
					moves++;
					if(grid[i-1][j+2] == EMPTY){
						grid[i-1][j+2] = PMOVE;
					}
				}
			}
			if(i-1 > -1 && j-2 > -1 && (grid[i-1][j-2]-17) / 6 != p_turn){
				if(!king_at_risk(grid, i, j, i-1, j-2, p_turn)){
					move_grid[i-1][j-2] = PMOVE;
					moves++;
					if(grid[i-1][j-2] == EMPTY){
						grid[i-1][j-2] = PMOVE;
					}
				}
			}
			break;
		case P1QUEEN:
			for(int x = 1; x < 8; x++){
				if(i+x > 7 || (grid[i+x][j]-17) / 6 == p_turn){
					break;
				} else if(grid[i+x][j] != EMPTY){
					if(!king_at_risk(grid, i, j, i+x, j, p_turn)){
						move_grid[i+x][j] = PMOVE;
						moves++;
					}
					break;
				}
				if(!king_at_risk(grid, i, j, i+x, j, p_turn)){
					move_grid[i+x][j] = PMOVE;
					grid[i+x][j] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(i-x < 0 || (grid[i-x][j]-17) / 6 == p_turn){
					break;
				} else if(grid[i-x][j] != EMPTY){
					if(!king_at_risk(grid, i, j, i-x, j, p_turn)){
						move_grid[i-x][j] = PMOVE;
						moves++;
					}
					break;
				}
				if(!king_at_risk(grid, i, j, i-x, j, p_turn)){
					move_grid[i-x][j] = PMOVE;
					grid[i-x][j] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(j+x > 7 || (grid[i][j+x]-17) / 6 == p_turn){
					break;
				} else if(grid[i][j+x] != EMPTY){
					if(!king_at_risk(grid, i, j, i, j+x, p_turn)){
						move_grid[i][j+x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!king_at_risk(grid, i, j, i, j+x, p_turn)){
					move_grid[i][j+x] = PMOVE;
					grid[i][j+x] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(j-x < 0 || (grid[i][j-x]-17) / 6 == p_turn){
					break;
				} else if(grid[i][j-x] != EMPTY){
					if(!king_at_risk(grid, i, j, i, j-x, p_turn)){
						move_grid[i][j-x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!king_at_risk(grid, i, j, i, j-x, p_turn)){
					move_grid[i][j-x] = PMOVE;
					grid[i][j-x] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(i+x > 7 || j+x > 7 || (grid[i+x][j+x]-17) / 6 == p_turn){
					break;
				} else if(grid[i+x][j+x] != EMPTY){
					if(!king_at_risk(grid, i, j, i+x, j+x, p_turn)){
						move_grid[i+x][j+x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!king_at_risk(grid, i, j, i+x, j+x, p_turn)){
					move_grid[i+x][j+x] = PMOVE;
					grid[i+x][j+x] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(i-x < 0 || j+x > 7 || (grid[i-x][j+x]-17) / 6 == p_turn){
					break;
				} else if(grid[i-x][j+x] != EMPTY){
					if(!king_at_risk(grid, i, j, i-x, j+x, p_turn)){
						move_grid[i-x][j+x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!king_at_risk(grid, i, j, i-x, j+x, p_turn)){
					move_grid[i-x][j+x] = PMOVE;
					grid[i-x][j+x] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(i+x > 7 || j-x < 0 || (grid[i+x][j-x]-17) / 6 == p_turn){
					break;
				} else if(grid[i+x][j-x] != EMPTY){
					if(!king_at_risk(grid, i, j, i+x, j-x, p_turn)){
						move_grid[i+x][j-x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!king_at_risk(grid, i, j, i+x, j-x, p_turn)){
					move_grid[i+x][j-x] = PMOVE;
					grid[i+x][j-x] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(i-x < 0 || j-x < 0 || (grid[i-x][j-x]-17) / 6 == p_turn){
					break;
				} else if(grid[i-x][j-x] != EMPTY){
					if(!king_at_risk(grid, i, j, i-x, j-x, p_turn)){
						move_grid[i-x][j-x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!king_at_risk(grid, i, j, i-x, j-x, p_turn)){
					move_grid[i-x][j-x] = PMOVE;
					grid[i-x][j-x] = PMOVE;
					moves++;
				}
			}
			break;
		case P1KING:
			if(p_turn == 0 && p1_castle1 == 0 && grid[1][0] == EMPTY && grid[2][0] == EMPTY){
				if(!king_at_risk(grid, i, j, 1, 0, p_turn)){
					move_grid[1][0] = PMOVE;
					grid[1][0] = PMOVE;
					moves++;
				}
			} else if(p_turn == 1 && p2_castle1 == 0 && grid[1][7] == EMPTY && grid[2][7] == EMPTY){
				if(!king_at_risk(grid, i, j, 1, 7, p_turn)){
					move_grid[1][7] = PMOVE;
					grid[1][7] = PMOVE;
					moves++;
				}
			}
			if(p_turn == 0 && p1_castle2 == 0 && grid[4][0] == EMPTY && grid[5][0] == EMPTY && grid[6][0] == EMPTY){
				if(!king_at_risk(grid, i, j, 5, 0, p_turn)){
					move_grid[5][0] = PMOVE;
					grid[5][0] = PMOVE;
					moves++;
				}  
			} else if(p_turn == 1 && p2_castle2 == 0 && grid[4][7] == EMPTY && grid[5][7] == EMPTY && grid[6][7] == EMPTY){
				if(!king_at_risk(grid, i, j, 5, 7, p_turn)){
					move_grid[5][7] = PMOVE;
					grid[5][7] = PMOVE;
					moves++;
				}
			}
			if(i+1 < 8 && (grid[i+1][j]-17) / 6 != p_turn){
				if(!king_at_risk(grid, i, j, i+1, j, p_turn)){
					move_grid[i+1][j] = PMOVE;
					moves++;
					if(grid[i+1][j] == EMPTY){
						grid[i+1][j] = PMOVE;
					}
				}
			}
			if(i-1 > -1 && (grid[i-1][j]-17) / 6 != p_turn){
				if(!king_at_risk(grid, i, j, i-1, j, p_turn)){
					move_grid[i-1][j] = PMOVE;
					moves++;
					if(grid[i-1][j] == EMPTY){
						grid[i-1][j] = PMOVE;
					}
				}
			}
			if(j+1 < 8 && (grid[i][j+1]-17) / 6 != p_turn){
				if(!king_at_risk(grid, i, j, i, j+1, p_turn)){
					move_grid[i][j+1] = PMOVE;
					moves++;
					if(grid[i][j+1] == EMPTY){
						grid[i][j+1] = PMOVE;
					}
				}
			}
			if(j-1 > -1 && (grid[i][j-1]-17) / 6 != p_turn){
				if(!king_at_risk(grid, i, j, i, j-1, p_turn)){
					move_grid[i][j-1] = PMOVE;
					moves++;
					if(grid[i][j-1] == EMPTY){
						grid[i][j-1] = PMOVE;
					}
				}
			}
			if(i+1 < 8 && j+1 < 8 && (grid[i+1][j+1]-17) / 6 != p_turn){
				if(!king_at_risk(grid, i, j, i+1, j+1, p_turn)){
					move_grid[i+1][j+1] = PMOVE;
					moves++;
					if(grid[i+1][j+1] == EMPTY){
						grid[i+1][j+1] = PMOVE;
					}
				}
			}
			if(i+1 < 8 && j-1 > -1 && (grid[i+1][j-1]-17) / 6 != p_turn){
				if(!king_at_risk(grid, i, j, i+1, j-1, p_turn)){
					move_grid[i+1][j-1] = PMOVE;
					moves++;
					if(grid[i+1][j-1] == EMPTY){
						grid[i+1][j-1] = PMOVE;
					}
				}
			}
			if(i-1 > -1 && j+1 < 8 && (grid[i-1][j+1]-17) / 6 != p_turn){
				if(!king_at_risk(grid, i, j, i-1, j+1, p_turn)){
					move_grid[i-1][j+1] = PMOVE;
					moves++;
					if(grid[i-1][j+1] == EMPTY){
						grid[i-1][j+1] = PMOVE;
					}
				}
			}
			if(i-1 > -1 && j-1 > -1 && (grid[i-1][j-1]-17) / 6 != p_turn){
				if(!king_at_risk(grid, i, j, i-1, j-1, p_turn)){
					move_grid[i-1][j-1] = PMOVE;
					moves++;
					if(grid[i-1][j-1] == EMPTY){
						grid[i-1][j-1] = PMOVE;
					}
				}
			}
			break;
		default:
			break;
	}
	return moves;
}

//Check if player 1 or player 2 is in checkmate
int checkmate(int **grid, int p_turn){
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			//Check if they have any moves that don't leave them in check
			if(grid[i][j] != EMPTY && grid[i][j] / 23 == p_turn && get_moves(grid, i, j, p_turn) > 0){
				//Erase other potential moves
				for(int i = 0; i < 8; i++){
					for(int j = 0; j < 8; j++){
						if(grid[i][j] == PMOVE){
							grid[i][j] = EMPTY;
						}
						move_grid[i][j] = EMPTY;
					}
				}
				return 0;
			}
			//Erase other potential moves
			for(int i = 0; i < 8; i++){
				for(int j = 0; j < 8; j++){
					if(grid[i][j] == PMOVE){
						grid[i][j] = EMPTY;
					}
					move_grid[i][j] = EMPTY;
				}
			}
		}
	}
	return p_turn+1;
}

int chess(int **grid, SDL_Event event, int t, int cellsize){
	int ret = CHESS;

	if(t == 1){
		for(int i = 0; i < 8; i++){
			for(int j = 0; j < 8; j++){
				move_grid[i][j] = EMPTY;
				grid[i][j] = EMPTY;
			}
		}

		chosen_x = -1;
		chosen_y = -1;
		turn = 0;
		p1_enpassant = -1;
		p2_enpassant = -1;

		//Setup initial board pieces
		for(int i = 0; i < 8; i++){
			grid[i][1] = P1PAWN;
			grid[i][6] = P2PAWN;
		}
		grid[0][0] = P1ROOK;
		grid[1][0] = P1KNIGHT;
		grid[2][0] = P1BISHOP;
		grid[3][0] = P1KING;
		p1_king_x = 3;
		p1_king_y = 0;
		p1_castle1 = 0;
		p1_castle2 = 0;
		grid[4][0] = P1QUEEN;
		grid[5][0] = P1BISHOP;
		grid[6][0] = P1KNIGHT;
		grid[7][0] = P1ROOK;
		
		grid[0][7] = P2ROOK;
		grid[1][7] = P2KNIGHT;
		grid[2][7] = P2BISHOP;
		grid[3][7] = P2KING;
		p2_king_x = 3;
		p2_king_y = 7;
		p2_castle1 = 0;
		p2_castle2 = 0;
		grid[4][7] = P2QUEEN;
		grid[5][7] = P2BISHOP;
		grid[6][7] = P2KNIGHT;
		grid[7][7] = P2ROOK;
	}

	//Respond to user input
	if(event.type == SDL_MOUSEBUTTONUP){
		int button_x = event.button.x / cellsize;
		int button_y = event.button.y / cellsize;
		if(event.button.button == SDL_BUTTON_LEFT && button_x < 8 && button_y < 8 && button_x > -1 && button_y > -1){
			//Make potential move
			if(move_grid[button_x][button_y] == PMOVE){
				//Move piece
				grid[button_x][button_y] = grid[chosen_x][chosen_y];
				grid[chosen_x][chosen_y] = EMPTY;
				if(turn == 0){
					if(grid[button_x][button_y] == P1KING){
						p1_king_x = button_x;
						p1_king_y = button_y;
						p1_castle1 = 1;
						p1_castle2 = 1;
						if(button_x-chosen_x == 2){
							grid[7][0] = EMPTY;
							grid[4][0] = P1ROOK;
						} else if(button_x-chosen_x == -2){
							grid[0][0] = EMPTY;
							grid[2][0] = P1ROOK;
						}
					} else if (chosen_x == 0 && chosen_y == 0){
						p1_castle1 = 1;
					} else if (chosen_x == 7 && chosen_y == 0){
						p1_castle2 = 1;
					} else if (button_x == 0 && button_y == 7){
						p2_castle1 = 1;
					} else if (button_x == 7 && button_y == 7){
						p2_castle2 = 1;
					}
					if(grid[button_x][button_y] == P1PAWN && button_y-chosen_y == 2){
						p1_enpassant = button_x;
					} else {
						p1_enpassant = -1;
					}
					if(grid[button_x][button_y] == P1PAWN && button_x == p2_enpassant && button_y == 5){
						grid[button_x][4] = EMPTY;
					}
					if(button_y == 7 && grid[button_x][button_y] == P1PAWN){
						grid[button_x][button_y] = P1QUEEN; //Change this to where it could be any piece
					}
				} else {
					if(grid[button_x][button_y] == P2KING){
						p2_king_x = button_x;
						p2_king_y = button_y;
						p2_castle1 = 1;
						p2_castle2 = 1;
						if(button_x-chosen_x == 2){
							grid[7][7] = EMPTY;
							grid[4][7] = P2ROOK;
						} else if(button_x-chosen_x == -2){
							grid[0][7] = EMPTY;
							grid[2][7] = P2ROOK;
						}
					} else if (chosen_x == 0 && chosen_y == 7){
						p2_castle1 = 1;
					} else if (chosen_x == 7 && chosen_y == 7){
						p2_castle2 = 1;
					} else if (button_x == 0 && button_y == 0){
						p1_castle1 = 1;
					} else if (button_x == 7 && button_y == 0){
						p1_castle2 = 1;
					}
					if(grid[button_x][button_y] == P2PAWN && chosen_y-button_y == 2){
						p2_enpassant = button_x;
					} else {
						p2_enpassant = -1;
					}
					if(grid[button_x][button_y] == P2PAWN && button_x == p1_enpassant && button_y == 2){
						grid[button_x][3] = EMPTY;
					}
					if(button_y == 0 && grid[button_x][button_y] == P2PAWN){
						grid[button_x][button_y] = P2QUEEN; //Change this to where it could be any piece
					}
				}

				//Erase other potential moves
				for(int i = 0; i < 8; i++){
					for(int j = 0; j < 8; j++){
						if(grid[i][j] == PMOVE){
							grid[i][j] = EMPTY;
						}
						move_grid[i][j] = EMPTY;
					}
				}

				chosen_x = -1;
				chosen_y = -1;

				//Checkmate decides winner
				if(turn == 0 && checkmate(grid, 1)){
					printf("PLAYER 1 WINS!\n");
					ret = ARCADE;
				} else if(turn == 1 && checkmate(grid, 0)){
					printf("PLAYER 2 WINS!\n");
					ret = ARCADE;
				}

				turn = 1-turn; //Swap to next player's turn

			} else if(chosen_x != -1){

				//Clear other potential moves
				for(int i = 0; i < 8; i++){
					for(int j = 0; j < 8; j++){
						if(grid[i][j] == PMOVE){
							grid[i][j] = EMPTY;
						}
						move_grid[i][j] = EMPTY;
					}
				}

				chosen_x = -1;
				chosen_y = -1;

				//Look at potential moves
				if(grid[button_x][button_y] != EMPTY && grid[button_x][button_y] / 23 == turn){
					chosen_x = button_x;
					chosen_y = button_y;
					get_moves(grid, chosen_x, chosen_y, turn);
				}
			} else {
				//Look at potential moves
				if(grid[button_x][button_y] != EMPTY && grid[button_x][button_y] / 23 == turn){
					chosen_x = button_x;
					chosen_y = button_y;
					get_moves(grid, chosen_x, chosen_y, turn);
				}
			}
		}
	}

	return ret;
}