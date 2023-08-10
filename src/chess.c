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
int chosenx, choseny, p1castle1, p1castle2, p2castle1, p2castle2, turn, p1kingx, p1kingy, p2kingx, p2kingy, p1enpassant, p2enpassant;
int **movegrid;

//Deternube if after the given move your king is left exposed
int kingatrisk(int **grid, int beforex, int beforey, int afterx, int aftery, int pturn){
	int placemarker = grid[afterx][aftery];
	grid[afterx][aftery] = grid[beforex][beforey];
	grid[beforex][beforey] = EMPTY;
	int kingx = 0;
	int kingy = 0;
	//Get coords of king we are checking
	if(pturn){
		kingx = p2kingx;
		kingy = p2kingy;
		if(grid[p2kingx][p2kingy] != P2KING){
			kingx = afterx;
			kingy = aftery;
		}
	} else {
		kingx = p1kingx;
		kingy = p1kingy;
		if(grid[p1kingx][p1kingy] != P1KING){
			kingx = afterx;
			kingy = aftery;
		}
	}
	for(int x = 1; x < 8; x++){
		if(kingx+x > 7 || (grid[kingx+x][kingy]-17) / 6 == pturn){
			break;
		} else if((grid[kingx+x][kingy]-17) % 6 == P1ROOK-17 || (grid[kingx+x][kingy]-17) % 6 == P1QUEEN-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		} else if((grid[kingx+x][kingy]-17) / 6 > -1){
			break;
		}
	}
	for(int x = 1; x < 8; x++){
		if(kingx-x < 0 || (grid[kingx-x][kingy]-17) / 6 == pturn){
			break;
		} else if((grid[kingx-x][kingy]-17) % 6 == P1ROOK-17 || (grid[kingx-x][kingy]-17) % 6 == P1QUEEN-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		} else if((grid[kingx-x][kingy]-17) / 6 > -1){
			break;
		}
	}
	for(int x = 1; x < 8; x++){
		if(kingy+x > 7 || (grid[kingx][kingy+x]-17) / 6 == pturn){
			break;
		} else if((grid[kingx][kingy+x]-17) % 6 == P1ROOK-17 || (grid[kingx][kingy+x]-17) % 6 == P1QUEEN-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		} else if((grid[kingx][kingy+x]-17) / 6 > -1){
			break;
		}
	}
	for(int x = 1; x < 8; x++){
		if(kingy-x < 0 || (grid[kingx][kingy-x]-17) / 6 == pturn){
			break;
		} else if((grid[kingx][kingy-x]-17) % 6 == P1ROOK-17 || (grid[kingx][kingy-x]-17) % 6 == P1QUEEN-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		} else if((grid[kingx][kingy-x]-17) / 6 > -1){
			break;
		}
	}
	for(int x = 1; x < 8; x++){
		if(kingx+x > 7 || kingy+x > 7 || (grid[kingx+x][kingy+x]-17) / 6 == pturn){
			break;
		} else if((grid[kingx+x][kingy+x]-17) % 6 == P1BISHOP-17 || (grid[kingx+x][kingy+x]-17) % 6 == P1QUEEN-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		} else if((grid[kingx+x][kingy+x]-17) / 6 > -1){
			break;
		}
	}
	for(int x = 1; x < 8; x++){
		if(kingx-x < 0 || kingy+x > 7 || (grid[kingx-x][kingy+x]-17) / 6 == pturn){
			break;
		} else if((grid[kingx-x][kingy+x]-17) % 6 == P1BISHOP-17 || (grid[kingx-x][kingy+x]-17) % 6 == P1QUEEN-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		} else if((grid[kingx-x][kingy+x]-17) / 6 > -1){
			break;
		}
	}
	for(int x = 1; x < 8; x++){
		if(kingx+x > 7 || kingy-x < 0 || (grid[kingx+x][kingy-x]-17) / 6 == pturn){
			break;
		} else if((grid[kingx+x][kingy-x]-17) % 6 == P1BISHOP-17 || (grid[kingx+x][kingy-x]-17) % 6 == P1QUEEN-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		} else if((grid[kingx+x][kingy-x]-17) / 6 > -1){
			break;
		}
	}
	for(int x = 1; x < 8; x++){
		if(kingx-x < 0 || kingy-x < 0 || (grid[kingx-x][kingy-x]-17) / 6 == pturn){
			break;
		} else if((grid[kingx-x][kingy-x]-17) % 6 == P1BISHOP-17 || (grid[kingx-x][kingy-x]-17) % 6 == P1QUEEN-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		} else if((grid[kingx-x][kingy-x]-17) / 6 > -1){
			break;
		}
	}
	if(kingx+2 < 8 && kingy+1 < 8 && (grid[kingx+2][kingy+1]-17) / 6 != pturn){
		if((grid[kingx+2][kingy+1]-17) % 6 == P1KNIGHT-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		}
	}
	if(kingx+2 < 8 && kingy-1 > -1 && (grid[kingx+2][kingy-1]-17) / 6 != pturn){
		if((grid[kingx+2][kingy-1]-17) % 6 == P1KNIGHT-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		}
	}
	if(kingx+1 < 8 && kingy+2 < 8 && (grid[kingx+1][kingy+2]-17) / 6 != pturn){
		if((grid[kingx+1][kingy+2]-17) % 6 == P1KNIGHT-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		}
	}
	if(kingx+1 < 8 && kingy-2 > -1 && (grid[kingx+1][kingy-2]-17) / 6 != pturn){
		if((grid[kingx+1][kingy-2]-17) % 6 == P1KNIGHT-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		}
	}
	if(kingx-2 > -1 && kingy+1 < 8 && (grid[kingx-2][kingy+1]-17) / 6 != pturn){
		if((grid[kingx-2][kingy+1]-17) % 6 == P1KNIGHT-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		}
	}
	if(kingx-2 > -1 && kingy-1 > -1 && (grid[kingx-2][kingy-1]-17) / 6 != pturn){
		if((grid[kingx-2][kingy-1]-17) % 6 == P1KNIGHT-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		}
	}
	if(kingx-1 > -1 && kingy+2 < 8 && (grid[kingx-1][kingy+2]-17) / 6 != pturn){
		if((grid[kingx-1][kingy+2]-17) % 6 == P1KNIGHT-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		}
	}
	if(kingx-1 > -1 && kingy-2 > -1 && (grid[kingx-1][kingy-2]-17) / 6 != pturn){
		if((grid[kingx-1][kingy-2]-17) % 6 == P1KNIGHT-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		}
	}
	if(kingx+1 < 8 && (grid[kingx+1][kingy]-17) / 6 != pturn){
		if((grid[kingx+1][kingy]-17) % 6 == P1KING-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		}
	}
	if(kingx-1 > -1 && (grid[kingx-1][kingy]-17) / 6 != pturn){
		if((grid[kingx-1][kingy]-17) % 6 == P1KING-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		}
	}
	if(kingy+1 < 8 && (grid[kingx][kingy+1]-17) / 6 != pturn){
		if((grid[kingx][kingy+1]-17) % 6 == P1KING-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		}
	}
	if(kingy-1 > -1 && (grid[kingx][kingy-1]-17) / 6 != pturn){
		if((grid[kingx][kingy-1]-17) % 6 == P1KING-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		}
	}
	if(kingx+1 < 8 && kingy+1 < 8 && (grid[kingx+1][kingy+1]-17) / 6 != pturn){
		if((grid[kingx+1][kingy+1]-17) % 6 == P1KING-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		}
		if(pturn == 0 && grid[kingx+1][kingy+1] == P2PAWN){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		}
	}
	if(kingx+1 < 8 && kingy-1 > -1 && (grid[kingx+1][kingy-1]-17) / 6 != pturn){
		if((grid[kingx+1][kingy-1]-17) % 6 == P1KING-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		}
		if(pturn == 1 && grid[kingx+1][kingy-1] == P1PAWN){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		}
	}
	if(kingx-1 > -1 && kingy+1 < 8 && (grid[kingx-1][kingy+1]-17) / 6 != pturn){
		if((grid[kingx-1][kingy+1]-17) % 6 == P1KING-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		}
		if(pturn == 0 && grid[kingx-1][kingy+1] == P2PAWN){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		}
	}
	if(kingx-1 > -1 && kingy-1 > -1 && (grid[kingx-1][kingy-1]-17) / 6 != pturn){
		if((grid[kingx-1][kingy-1]-17) % 6 == P1KING-17){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		}
		if(pturn == 1 && grid[kingx-1][kingy-1] == P1PAWN){
			grid[beforex][beforey] = grid[afterx][aftery];
			grid[afterx][aftery] = placemarker;
			return 1;
		}
	}
	grid[beforex][beforey] = grid[afterx][aftery];
	grid[afterx][aftery] = placemarker;
	return 0;
}

//Determine all potential moves with given piece and display them
int getmoves(int **grid, int i, int j, int pturn){
	int moves = 0;

	switch(((grid[i][j]-17) % 6)+17){
		case P1PAWN:
			if(grid[i][j] == P1PAWN){
				if(grid[i][j+1] == EMPTY){
					if(!kingatrisk(grid, i, j, i, j+1, pturn)){
						movegrid[i][j+1] = PMOVE;
						grid[i][j+1] = PMOVE;
						moves++;
					}
					if(j == 1 && grid[i][3] == EMPTY){
						if(!kingatrisk(grid, i, j, i, 3, pturn)){
							movegrid[i][3] = PMOVE;
							grid[i][3] = PMOVE;
							moves++;
						}
					}
				}
				if(i > 0 && grid[i-1][j+1] / 23 == 1){
					if(!kingatrisk(grid, i, j, i-1, j+1, pturn)){
						movegrid[i-1][j+1] = PMOVE;
						moves++;
					}
				}
				if(i < 7 && grid[i+1][j+1] / 23 == 1){
					if(!kingatrisk(grid, i, j, i+1, j+1, pturn)){
						movegrid[i+1][j+1] = PMOVE;
						moves++;
					}
				}
				if(i > 0 && i-1 == p2enpassant && j == 4){
					if(!kingatrisk(grid, i, j, i-1, 5, pturn)){
						movegrid[i-1][5] = PMOVE;
						grid[i-1][5] = PMOVE;
						moves++;
					}
				}
				if(i+1 == p2enpassant && j == 4){
					if(!kingatrisk(grid, i, j, i+1, 5, pturn)){
						movegrid[i+1][5] = PMOVE;
						grid[i+1][5] = PMOVE;
						moves++;
					}
				}
			} else {
				if(grid[i][j-1] == EMPTY){
					if(!kingatrisk(grid, i, j, i, j-1, pturn)){
						movegrid[i][j-1] = PMOVE;
						grid[i][j-1] = PMOVE;
						moves++;
					}
					if(j == 6 && grid[i][4] == EMPTY){
						if(!kingatrisk(grid, i, j, i, 4, pturn)){
							movegrid[i][4] = PMOVE;
							grid[i][4] = PMOVE;
							moves++;
						}
					}
				}
				if(i > 0 && (grid[i-1][j-1]-17) / 6 == 0){
					if(!kingatrisk(grid, i, j, i-1, j-1, pturn)){
						movegrid[i-1][j-1] = PMOVE;
						moves++;
					}
				}
				if(i < 7 && (grid[i+1][j-1]-17) / 6 == 0){
					if(!kingatrisk(grid, i, j, i+1, j-1, pturn)){
						movegrid[i+1][j-1] = PMOVE;
						moves++;
					}
				}
				if(i > 0 && i-1 == p1enpassant && j == 3){
					if(!kingatrisk(grid, i, j, i-1, 2, pturn)){
						movegrid[i-1][2] = PMOVE;
						grid[i-1][2] = PMOVE;
						moves++;
					}
				}
				if(i+1 == p1enpassant && j == 3){
					if(!kingatrisk(grid, i, j, i+1, 2, pturn)){
						movegrid[i+1][2] = PMOVE;
						grid[i+1][2] = PMOVE;
						moves++;
					}
				}
			}
			break;
		case P1ROOK:
			for(int x = 1; x < 8; x++){
				if(i+x > 7 || (grid[i+x][j]-17) / 6 == pturn){
					break;
				} else if(grid[i+x][j] != EMPTY){
					if(!kingatrisk(grid, i, j, i+x, j, pturn)){
						movegrid[i+x][j] = PMOVE;
						moves++;
					}
					break;
				}
				if(!kingatrisk(grid, i, j, i+x, j, pturn)){
					movegrid[i+x][j] = PMOVE;
					grid[i+x][j] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(i-x < 0 || (grid[i-x][j]-17) / 6 == pturn){
					break;
				} else if(grid[i-x][j] != EMPTY){
					if(!kingatrisk(grid, i, j, i-x, j, pturn)){
						movegrid[i-x][j] = PMOVE;
						moves++;
					}
					break;
				}
				if(!kingatrisk(grid, i, j, i-x, j, pturn)){
					movegrid[i-x][j] = PMOVE;
					grid[i-x][j] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(j+x > 7 || (grid[i][j+x]-17) / 6 == pturn){
					break;
				} else if(grid[i][j+x] != EMPTY){
					if(!kingatrisk(grid, i, j, i, j+x, pturn)){
						movegrid[i][j+x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!kingatrisk(grid, i, j, i, j+x, pturn)){
					movegrid[i][j+x] = PMOVE;
					grid[i][j+x] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(j-x < 0 || (grid[i][j-x]-17) / 6 == pturn){
					break;
				} else if(grid[i][j-x] != EMPTY){
					if(!kingatrisk(grid, i, j, i, j-x, pturn)){
						movegrid[i][j-x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!kingatrisk(grid, i, j, i, j-x, pturn)){
					movegrid[i][j-x] = PMOVE;
					grid[i][j-x] = PMOVE;
					moves++;
				}
			}
			break;
		case P1BISHOP:
			for(int x = 1; x < 8; x++){
				if(i+x > 7 || j+x > 7 || (grid[i+x][j+x]-17) / 6 == pturn){
					break;
				} else if(grid[i+x][j+x] != EMPTY){
					if(!kingatrisk(grid, i, j, i+x, j+x, pturn)){
						movegrid[i+x][j+x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!kingatrisk(grid, i, j, i+x, j+x, pturn)){
					movegrid[i+x][j+x] = PMOVE;
					grid[i+x][j+x] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(i-x < 0 || j+x > 7 || (grid[i-x][j+x]-17) / 6 == pturn){
					break;
				} else if(grid[i-x][j+x] != EMPTY){
					if(!kingatrisk(grid, i, j, i-x, j+x, pturn)){
						movegrid[i-x][j+x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!kingatrisk(grid, i, j, i-x, j+x, pturn)){
					movegrid[i-x][j+x] = PMOVE;
					grid[i-x][j+x] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(i+x > 7 || j-x < 0 || (grid[i+x][j-x]-17) / 6 == pturn){
					break;
				} else if(grid[i+x][j-x] != EMPTY){
					if(!kingatrisk(grid, i, j, i+x, j-x, pturn)){
						movegrid[i+x][j-x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!kingatrisk(grid, i, j, i+x, j-x, pturn)){
					movegrid[i+x][j-x] = PMOVE;
					grid[i+x][j-x] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(i-x < 0 || j-x < 0 || (grid[i-x][j-x]-17) / 6 == pturn){
					break;
				} else if(grid[i-x][j-x] != EMPTY){
					if(!kingatrisk(grid, i, j, i-x, j-x, pturn)){
						movegrid[i-x][j-x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!kingatrisk(grid, i, j, i-x, j-x, pturn)){
					movegrid[i-x][j-x] = PMOVE;
					grid[i-x][j-x] = PMOVE;
					moves++;
				}
			}
			break;
		case P1KNIGHT:
			if(i+2 < 8 && j+1 < 8 && (grid[i+2][j+1]-17) / 6 != pturn){
				if(!kingatrisk(grid, i, j, i+2, j+1, pturn)){
					movegrid[i+2][j+1] = PMOVE;
					moves++;
					if(grid[i+2][j+1] == EMPTY){
						grid[i+2][j+1] = PMOVE;
					}
				}
			}
			if(i+2 < 8 && j-1 > -1 && (grid[i+2][j-1]-17) / 6 != pturn){
				if(!kingatrisk(grid, i, j, i+2, j-1, pturn)){
					movegrid[i+2][j-1] = PMOVE;
					moves++;
					if(grid[i+2][j-1] == EMPTY){
						grid[i+2][j-1] = PMOVE;
					}
				}
			}
			if(i+1 < 8 && j+2 < 8 && (grid[i+1][j+2]-17) / 6 != pturn){
				if(!kingatrisk(grid, i, j, i+1, j+2, pturn)){
					movegrid[i+1][j+2] = PMOVE;
					moves++;
					if(grid[i+1][j+2] == EMPTY){
						grid[i+1][j+2] = PMOVE;
					}
				}
			}
			if(i+1 < 8 && j-2 > -1 && (grid[i+1][j-2]-17) / 6 != pturn){
				if(!kingatrisk(grid, i, j, i+1, j-2, pturn)){
					movegrid[i+1][j-2] = PMOVE;
					moves++;
					if(grid[i+1][j-2] == EMPTY){
						grid[i+1][j-2] = PMOVE;
					}
				}
			}
			if(i-2 > -1 && j+1 < 8 && (grid[i-2][j+1]-17) / 6 != pturn){
				if(!kingatrisk(grid, i, j, i-2, j+1, pturn)){
					movegrid[i-2][j+1] = PMOVE;
					moves++;
					if(grid[i-2][j+1] == EMPTY){
						grid[i-2][j+1] = PMOVE;
					}
				}
			}
			if(i-2 > -1 && j-1 > -1 && (grid[i-2][j-1]-17) / 6 != pturn){
				if(!kingatrisk(grid, i, j, i-2, j-1, pturn)){
					movegrid[i-2][j-1] = PMOVE;
					moves++;
					if(grid[i-2][j-1] == EMPTY){
						grid[i-2][j-1] = PMOVE;
					}
				}
			}
			if(i-1 > -1 && j+2 < 8 && (grid[i-1][j+2]-17) / 6 != pturn){
				if(!kingatrisk(grid, i, j, i-1, j+2, pturn)){
					movegrid[i-1][j+2] = PMOVE;
					moves++;
					if(grid[i-1][j+2] == EMPTY){
						grid[i-1][j+2] = PMOVE;
					}
				}
			}
			if(i-1 > -1 && j-2 > -1 && (grid[i-1][j-2]-17) / 6 != pturn){
				if(!kingatrisk(grid, i, j, i-1, j-2, pturn)){
					movegrid[i-1][j-2] = PMOVE;
					moves++;
					if(grid[i-1][j-2] == EMPTY){
						grid[i-1][j-2] = PMOVE;
					}
				}
			}
			break;
		case P1QUEEN:
			for(int x = 1; x < 8; x++){
				if(i+x > 7 || (grid[i+x][j]-17) / 6 == pturn){
					break;
				} else if(grid[i+x][j] != EMPTY){
					if(!kingatrisk(grid, i, j, i+x, j, pturn)){
						movegrid[i+x][j] = PMOVE;
						moves++;
					}
					break;
				}
				if(!kingatrisk(grid, i, j, i+x, j, pturn)){
					movegrid[i+x][j] = PMOVE;
					grid[i+x][j] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(i-x < 0 || (grid[i-x][j]-17) / 6 == pturn){
					break;
				} else if(grid[i-x][j] != EMPTY){
					if(!kingatrisk(grid, i, j, i-x, j, pturn)){
						movegrid[i-x][j] = PMOVE;
						moves++;
					}
					break;
				}
				if(!kingatrisk(grid, i, j, i-x, j, pturn)){
					movegrid[i-x][j] = PMOVE;
					grid[i-x][j] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(j+x > 7 || (grid[i][j+x]-17) / 6 == pturn){
					break;
				} else if(grid[i][j+x] != EMPTY){
					if(!kingatrisk(grid, i, j, i, j+x, pturn)){
						movegrid[i][j+x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!kingatrisk(grid, i, j, i, j+x, pturn)){
					movegrid[i][j+x] = PMOVE;
					grid[i][j+x] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(j-x < 0 || (grid[i][j-x]-17) / 6 == pturn){
					break;
				} else if(grid[i][j-x] != EMPTY){
					if(!kingatrisk(grid, i, j, i, j-x, pturn)){
						movegrid[i][j-x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!kingatrisk(grid, i, j, i, j-x, pturn)){
					movegrid[i][j-x] = PMOVE;
					grid[i][j-x] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(i+x > 7 || j+x > 7 || (grid[i+x][j+x]-17) / 6 == pturn){
					break;
				} else if(grid[i+x][j+x] != EMPTY){
					if(!kingatrisk(grid, i, j, i+x, j+x, pturn)){
						movegrid[i+x][j+x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!kingatrisk(grid, i, j, i+x, j+x, pturn)){
					movegrid[i+x][j+x] = PMOVE;
					grid[i+x][j+x] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(i-x < 0 || j+x > 7 || (grid[i-x][j+x]-17) / 6 == pturn){
					break;
				} else if(grid[i-x][j+x] != EMPTY){
					if(!kingatrisk(grid, i, j, i-x, j+x, pturn)){
						movegrid[i-x][j+x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!kingatrisk(grid, i, j, i-x, j+x, pturn)){
					movegrid[i-x][j+x] = PMOVE;
					grid[i-x][j+x] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(i+x > 7 || j-x < 0 || (grid[i+x][j-x]-17) / 6 == pturn){
					break;
				} else if(grid[i+x][j-x] != EMPTY){
					if(!kingatrisk(grid, i, j, i+x, j-x, pturn)){
						movegrid[i+x][j-x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!kingatrisk(grid, i, j, i+x, j-x, pturn)){
					movegrid[i+x][j-x] = PMOVE;
					grid[i+x][j-x] = PMOVE;
					moves++;
				}
			}
			for(int x = 1; x < 8; x++){
				if(i-x < 0 || j-x < 0 || (grid[i-x][j-x]-17) / 6 == pturn){
					break;
				} else if(grid[i-x][j-x] != EMPTY){
					if(!kingatrisk(grid, i, j, i-x, j-x, pturn)){
						movegrid[i-x][j-x] = PMOVE;
						moves++;
					}
					break;
				}
				if(!kingatrisk(grid, i, j, i-x, j-x, pturn)){
					movegrid[i-x][j-x] = PMOVE;
					grid[i-x][j-x] = PMOVE;
					moves++;
				}
			}
			break;
		case P1KING:
			if(pturn == 0 && p1castle1 == 0 && grid[1][0] == EMPTY && grid[2][0] == EMPTY){
				if(!kingatrisk(grid, i, j, 1, 0, pturn)){
					movegrid[1][0] = PMOVE;
					grid[1][0] = PMOVE;
					moves++;
				}
			} else if(pturn == 1 && p2castle1 == 0 && grid[1][7] == EMPTY && grid[2][7] == EMPTY){
				if(!kingatrisk(grid, i, j, 1, 7, pturn)){
					movegrid[1][7] = PMOVE;
					grid[1][7] = PMOVE;
					moves++;
				}
			}
			if(pturn == 0 && p1castle2 == 0 && grid[4][0] == EMPTY && grid[5][0] == EMPTY && grid[6][0] == EMPTY){
				if(!kingatrisk(grid, i, j, 5, 0, pturn)){
					movegrid[5][0] = PMOVE;
					grid[5][0] = PMOVE;
					moves++;
				}  
			} else if(pturn == 1 && p2castle2 == 0 && grid[4][7] == EMPTY && grid[5][7] == EMPTY && grid[6][7] == EMPTY){
				if(!kingatrisk(grid, i, j, 5, 7, pturn)){
					movegrid[5][7] = PMOVE;
					grid[5][7] = PMOVE;
					moves++;
				}
			}
			if(i+1 < 8 && (grid[i+1][j]-17) / 6 != pturn){
				if(!kingatrisk(grid, i, j, i+1, j, pturn)){
					movegrid[i+1][j] = PMOVE;
					moves++;
					if(grid[i+1][j] == EMPTY){
						grid[i+1][j] = PMOVE;
					}
				}
			}
			if(i-1 > -1 && (grid[i-1][j]-17) / 6 != pturn){
				if(!kingatrisk(grid, i, j, i-1, j, pturn)){
					movegrid[i-1][j] = PMOVE;
					moves++;
					if(grid[i-1][j] == EMPTY){
						grid[i-1][j] = PMOVE;
					}
				}
			}
			if(j+1 < 8 && (grid[i][j+1]-17) / 6 != pturn){
				if(!kingatrisk(grid, i, j, i, j+1, pturn)){
					movegrid[i][j+1] = PMOVE;
					moves++;
					if(grid[i][j+1] == EMPTY){
						grid[i][j+1] = PMOVE;
					}
				}
			}
			if(j-1 > -1 && (grid[i][j-1]-17) / 6 != pturn){
				if(!kingatrisk(grid, i, j, i, j-1, pturn)){
					movegrid[i][j-1] = PMOVE;
					moves++;
					if(grid[i][j-1] == EMPTY){
						grid[i][j-1] = PMOVE;
					}
				}
			}
			if(i+1 < 8 && j+1 < 8 && (grid[i+1][j+1]-17) / 6 != pturn){
				if(!kingatrisk(grid, i, j, i+1, j+1, pturn)){
					movegrid[i+1][j+1] = PMOVE;
					moves++;
					if(grid[i+1][j+1] == EMPTY){
						grid[i+1][j+1] = PMOVE;
					}
				}
			}
			if(i+1 < 8 && j-1 > -1 && (grid[i+1][j-1]-17) / 6 != pturn){
				if(!kingatrisk(grid, i, j, i+1, j-1, pturn)){
					movegrid[i+1][j-1] = PMOVE;
					moves++;
					if(grid[i+1][j-1] == EMPTY){
						grid[i+1][j-1] = PMOVE;
					}
				}
			}
			if(i-1 > -1 && j+1 < 8 && (grid[i-1][j+1]-17) / 6 != pturn){
				if(!kingatrisk(grid, i, j, i-1, j+1, pturn)){
					movegrid[i-1][j+1] = PMOVE;
					moves++;
					if(grid[i-1][j+1] == EMPTY){
						grid[i-1][j+1] = PMOVE;
					}
				}
			}
			if(i-1 > -1 && j-1 > -1 && (grid[i-1][j-1]-17) / 6 != pturn){
				if(!kingatrisk(grid, i, j, i-1, j-1, pturn)){
					movegrid[i-1][j-1] = PMOVE;
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
int checkmate(int **grid, int pturn){
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			//Check if they have any moves that don't leave them in check
			if(grid[i][j] != EMPTY && grid[i][j] / 23 == pturn && getmoves(grid, i, j, pturn) > 0){
				//Erase other potential moves
				for(int i = 0; i < 8; i++){
					for(int j = 0; j < 8; j++){
						if(grid[i][j] == PMOVE){
							grid[i][j] = EMPTY;
						}
						movegrid[i][j] = EMPTY;
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
					movegrid[i][j] = EMPTY;
				}
			}
		}
	}
	return pturn+1;
}

int chess(int **grid, SDL_Event eventbutton, int t, int cellsize){
	int ret = CHESS;

	if(t == 1){
		movegrid = (int **) calloc(8, sizeof(int *));
		for(int i = 0; i < 8; i++){
			movegrid[i] = (int *) calloc(8, sizeof(int));
			for(int j = 0; j < 8; j++){
				movegrid[i][j] = EMPTY;
				grid[i][j] = EMPTY;
			}
		}

		chosenx = -1;
		choseny = -1;
		turn = 0;
		p1enpassant = -1;
		p2enpassant = -1;

		//Setup initial board pieces
		for(int i = 0; i < 8; i++){
			grid[i][1] = P1PAWN;
			grid[i][6] = P2PAWN;
		}
		grid[0][0] = P1ROOK;
		grid[1][0] = P1KNIGHT;
		grid[2][0] = P1BISHOP;
		grid[3][0] = P1KING;
		p1kingx = 3;
		p1kingy = 0;
		p1castle1 = 0;
		p1castle2 = 0;
		grid[4][0] = P1QUEEN;
		grid[5][0] = P1BISHOP;
		grid[6][0] = P1KNIGHT;
		grid[7][0] = P1ROOK;
		
		grid[0][7] = P2ROOK;
		grid[1][7] = P2KNIGHT;
		grid[2][7] = P2BISHOP;
		grid[3][7] = P2KING;
		p2kingx = 3;
		p2kingy = 7;
		p2castle1 = 0;
		p2castle2 = 0;
		grid[4][7] = P2QUEEN;
		grid[5][7] = P2BISHOP;
		grid[6][7] = P2KNIGHT;
		grid[7][7] = P2ROOK;
	}

	//Respond to user input
	if(eventbutton.type == SDL_MOUSEBUTTONUP){
		int buttonx = eventbutton.button.x / cellsize;
		int buttony = eventbutton.button.y / cellsize;
		if(eventbutton.button.button == SDL_BUTTON_LEFT && buttonx < 8 && buttony < 8 && buttonx > -1 && buttony > -1){
			//Make potential move
			if(movegrid[buttonx][buttony] == PMOVE){
				//Move piece
				grid[buttonx][buttony] = grid[chosenx][choseny];
				grid[chosenx][choseny] = EMPTY;
				if(turn == 0){
					if(grid[buttonx][buttony] == P1KING){
						p1kingx = buttonx;
						p1kingy = buttony;
						p1castle1 = 1;
						p1castle2 = 1;
						if(buttonx-chosenx == 2){
							grid[7][0] = EMPTY;
							grid[4][0] = P1ROOK;
						} else if(buttonx-chosenx == -2){
							grid[0][0] = EMPTY;
							grid[2][0] = P1ROOK;
						}
					} else if (chosenx == 0 && choseny == 0){
						p1castle1 = 1;
					} else if (chosenx == 7 && choseny == 0){
						p1castle2 = 1;
					} else if (buttonx == 0 && buttony == 7){
						p2castle1 = 1;
					} else if (buttonx == 7 && buttony == 7){
						p2castle2 = 1;
					}
					if(grid[buttonx][buttony] == P1PAWN && buttony-choseny == 2){
						p1enpassant = buttonx;
					} else {
						p1enpassant = -1;
					}
					if(grid[buttonx][buttony] == P1PAWN && buttonx == p2enpassant && buttony == 5){
						grid[buttonx][4] = EMPTY;
					}
					if(buttony == 7 && grid[buttonx][buttony] == P1PAWN){
						grid[buttonx][buttony] = P1QUEEN; //Change this to where it could be any piece
					}
				} else {
					if(grid[buttonx][buttony] == P2KING){
						p2kingx = buttonx;
						p2kingy = buttony;
						p2castle1 = 1;
						p2castle2 = 1;
						if(buttonx-chosenx == 2){
							grid[7][7] = EMPTY;
							grid[4][7] = P2ROOK;
						} else if(buttonx-chosenx == -2){
							grid[0][7] = EMPTY;
							grid[2][7] = P2ROOK;
						}
					} else if (chosenx == 0 && choseny == 7){
						p2castle1 = 1;
					} else if (chosenx == 7 && choseny == 7){
						p2castle2 = 1;
					} else if (buttonx == 0 && buttony == 0){
						p1castle1 = 1;
					} else if (buttonx == 7 && buttony == 0){
						p1castle2 = 1;
					}
					if(grid[buttonx][buttony] == P2PAWN && choseny-buttony == 2){
						p2enpassant = buttonx;
					} else {
						p2enpassant = -1;
					}
					if(grid[buttonx][buttony] == P2PAWN && buttonx == p1enpassant && buttony == 2){
						grid[buttonx][3] = EMPTY;
					}
					if(buttony == 0 && grid[buttonx][buttony] == P2PAWN){
						grid[buttonx][buttony] = P2QUEEN; //Change this to where it could be any piece
					}
				}

				//Erase other potential moves
				for(int i = 0; i < 8; i++){
					for(int j = 0; j < 8; j++){
						if(grid[i][j] == PMOVE){
							grid[i][j] = EMPTY;
						}
						movegrid[i][j] = EMPTY;
					}
				}

				chosenx = -1;
				choseny = -1;

				//Checkmate decides winner
				if(turn == 0 && checkmate(grid, 1)){
					printf("PLAYER 1 WINS!\n");
					ret = ARCADE;
				} else if(turn == 1 && checkmate(grid, 0)){
					printf("PLAYER 2 WINS!\n");
					ret = ARCADE;
				}

				turn = 1-turn; //Swap to next player's turn

			} else if(chosenx != -1){

				//Clear other potential moves
				for(int i = 0; i < 8; i++){
					for(int j = 0; j < 8; j++){
						if(grid[i][j] == PMOVE){
							grid[i][j] = EMPTY;
						}
						movegrid[i][j] = EMPTY;
					}
				}

				chosenx = -1;
				choseny = -1;

				//Look at potential moves
				if(grid[buttonx][buttony] != EMPTY && grid[buttonx][buttony] / 23 == turn){
					chosenx = buttonx;
					choseny = buttony;
					getmoves(grid, chosenx, choseny, turn);
				}
			} else {
				//Look at potential moves
				if(grid[buttonx][buttony] != EMPTY && grid[buttonx][buttony] / 23 == turn){
					chosenx = buttonx;
					choseny = buttony;
					getmoves(grid, chosenx, choseny, turn);
				}
			}
		}
	}

	//Free Dynamic Memory
	if(ret == ARCADE){
		for(int i = 0; i < 8; i++){
			free(movegrid[i]);
		}
		free(movegrid);
	}

	return ret;
}