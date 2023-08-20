/* By Ethan Hughes */
/* Written 5/6/2021 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"arcade-defs.h"
/*
#define ARCADE 0
#define SUDOKU 17

#define EMPTY -1
#define CLEAR 0

//Number defines
#define ONE 8
#define TWO 9
#define THREE 10
#define FOUR 11
#define FIVE 12
#define SIX 13
#define SEVEN 14
#define EIGHT 15
#define NINE 16
*/
int sudokufinal[9][9];
int stopper;
int testing = 1;
int testing2 = 0;

//Guess checker
int sudokuhelper(int x, int y, int guess, int sudoku[9][9]){
	for(int i = 0; i < 9; i++){
		if(sudoku[i][y] == guess && x != i){
			return 0;
		}
	}
	for(int i = 0; i < 9; i++){
		if(sudoku[x][i] == guess && y != i){
			return 0;
		}
	}
	int x0 = x - (x % 3);
	int y0 = y - (y % 3);
	int x1 = x0+3;
	int y1 = y0+3;
	for(int i = x0; i < x1; i++){
		for(int j = y0; j < y1; j++){
			if(sudoku[i][j] == guess && (x != i || y != j)){
				return 0;
			}
		}
	}
	return 1;
}

//Two functions call each other
int sudokuwave(int x, int y, int guess, int sudokucollapse[9][9][9], int sudokucount[9][9], int sudokugrid[9][9], int *count);

//Wave function collapse helper function
int sudokufunction(int x, int y, int guess, int sudokucollapse[9][9][9], int sudokucount[9][9], int sudokugrid[9][9], int *count){
	if(sudokucollapse[x][y][guess] == 0){
		sudokucollapse[x][y][guess] = 1;
		sudokucount[x][y]++;
		if(sudokucount[x][y] == 8){
			for(int k = 0; k < 9; k++){
				if(sudokucollapse[x][y][k] == 0){
					if(sudokuhelper(x, y, k, sudokugrid) == 0){
						return 1;
					}
					sudokugrid[x][y] = k;
					sudokuwave(x, y, k, sudokucollapse, sudokucount, sudokugrid, count);
					(*count)++;
				}
			}
		}
	}
	return 0;
}

//Sudoku wave function collapse along adjacent squares
int sudokuwave(int x, int y, int guess, int sudokucollapse[9][9][9], int sudokucount[9][9], int sudokugrid[9][9], int *count){
	sudokucount[x][y] = 9;
	int ret = 0;
	for(int i = 0; i < 9; i++){
		sudokucollapse[x][y][i] = 1;
	}
	for(int i = 0; i < 9; i++){
		if(x != i){
			ret += sudokufunction(i, y, guess, sudokucollapse, sudokucount, sudokugrid, count);
		}
	}
	for(int i = 0; i < 9; i++){
		if(y != i){
			ret += sudokufunction(x, i, guess, sudokucollapse, sudokucount, sudokugrid, count);
		}
	}
	int x0 = x - (x % 3);
	int y0 = y - (y % 3);
	int x1 = x0+3;
	int y1 = y0+3;
	for(int i = x0; i < x1; i++){
		for(int j = y0; j < y1; j++){
			if(x != i || y != j){	
				ret += sudokufunction(i, j, guess, sudokucollapse, sudokucount, sudokugrid, count);
			}
		}
	}
	return ret;
}

//Recursive backtracking while counting solutions to generate sudoku grid
int sudokusolver(int sudokucollapse[9][9][9], int sudokucount[9][9], int sudokugrid[9][9]){
	int solutions = 0;
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(sudokugrid[i][j] == EMPTY){
				for(int k = 0; k < 9; k++){
					if(sudokucollapse[i][j][k] == 0){
						stopper++;
						if(stopper == 10000){
							printf("%d\n", stopper);
							return 2;
						}
						//Store different versions of sudoku grid for each recusion in case the random wave collapse reduces to a uncontinuable state
						//There should be a maximum of 81-17=64 recursion levels which means only 64 of these arrays should be stored at one time
						int sudokucollapse_copy[9][9][9];
						int sudokucount_copy[9][9];
						int sudokugrid_copy[9][9];
						memcpy(sudokucollapse_copy, sudokucollapse, sizeof(sudokucollapse_copy));
						memcpy(sudokucount_copy,  sudokucount, sizeof(sudokucount_copy));
						memcpy(sudokugrid_copy, sudokugrid, sizeof(sudokugrid_copy));
						int count;
						sudokugrid_copy[i][j] = k;
						if(sudokuhelper(i, j, k, sudokugrid_copy) == 0 || 
						sudokuwave(i, j, k, sudokucollapse_copy, sudokucount_copy, sudokugrid_copy, &count)){
							continue;
						}
						//Recursive call until the sudoku is solved
						int test = 0;
						if((test = sudokusolver(sudokucollapse_copy, sudokucount_copy, sudokugrid_copy))){
							if(test == 2){
								return 2;
							}
							solutions++;
							if(solutions == 2){
								if(testing){
									for(int o = 0; o < 9; o++){
										for(int p = 0; p < 9; p++){
											printf("%d", sudokugrid[o][p]);
										}
										printf("\n");
									}
									testing = 0;
								}
								memcpy(sudokugrid, sudokugrid_copy, sizeof(sudokugrid_copy));
								return 1;
							}
						}
					} else {
						continue;
					}
				}
				if(solutions == 1){
					return 1;
				}
				return 0;
			}
		}
	}
	return 1;
}

//Generate new sudokugrid and display it on the main grid
void sudokugenerate(int **grid){
	int sudokugrid[9][9];
	int sudokucount[9][9];
	int sudokucollapse[9][9][9];
	do{
		//Clear the grid
		for(int i = 0; i < 9; i++){
			for(int j = 0; j < 9; j++){
				sudokugrid[i][j] = EMPTY;
				sudokucount[i][j] = 0;
				for(int k = 0; k < 9; k++){
					sudokucollapse[i][j][k] = 0;
				}
			}
		}
		//Add 17 initial random number placements
		int x, y, guess;
		for(int i = 0; i < 17; i++){
			do{
				x = rand() % 9;
				y = rand() % 9;
				guess = rand() % 9;
			} while(sudokucollapse[x][y][guess] == 1 || sudokuhelper(x, y, guess, sudokugrid) == 0);
			sudokugrid[x][y] = guess;
			if(sudokuwave(x, y, guess, sudokucollapse, sudokucount, sudokugrid, &i)){
				stopper = 10000;
			}
		}
		stopper = 0;
	} while(stopper == 10000 || sudokusolver(sudokucollapse, sudokucount, sudokugrid) == 0 || stopper == 10000);
	int hints = 0;
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			sudokufinal[i][j] = EMPTY;
			if(sudokugrid[i][j] > EMPTY){
				grid[i][j] = sudokugrid[i][j]+ONE;
				sudokufinal[i][j] = sudokugrid[i][j];
				hints++;
			} else {
				grid[i][j] = CLEAR;
			}
			printf("%d", sudokufinal[i][j]);
		}
		printf("\n");
	}
	printf("number of hints: %d\n", hints);
}

int sudoku(int **grid, SDL_Event eventbutton, int t, int cellsize, int width, int height){	
	int ret = SUDOKU;

	if(t == 1){
		//Allocate grid and set random seed
		srand(time(NULL));
		
		//Generate Sudoku
		sudokugenerate(grid);
	}

	//Respond to user input
	if(eventbutton.type == SDL_MOUSEBUTTONUP){
		int buttonx = eventbutton.button.x / cellsize;
		int buttony = eventbutton.button.y / cellsize;
		if(eventbutton.button.button == SDL_BUTTON_LEFT && buttonx < 9 && buttony < 9 && buttonx > -1 && buttony > -1){
			//Add to current grid number
			if(grid[buttonx][buttony] == CLEAR){
				grid[buttonx][buttony] = ONE;
			} else if(grid[buttonx][buttony] > CLEAR && grid[buttonx][buttony] < NINE && sudokufinal[buttonx][buttony] == EMPTY){
				grid[buttonx][buttony]++;
			} else if(grid[buttonx][buttony] == NINE && sudokufinal[buttonx][buttony] == EMPTY){
				grid[buttonx][buttony] = CLEAR;
			}
		}
	} else if(eventbutton.type == SDL_KEYDOWN){
		int solved = 1;
		switch( eventbutton.key.keysym.sym ){
		case SDLK_r:
			//Generate new Sudoku
			sudokugenerate(grid);
			break;
		case SDLK_RETURN:
			//Check if sudoku has been solved
			solved = 1;
			for(int i = 0; i < 9; i++){
				for(int j = 0; j < 9; j++){
					if(grid[i][j] == CLEAR || sudokuhelper(i, j, grid[i][j]-ONE, sudokufinal) == 0){
						solved = 0;
					}
				}
			}
			if(solved){
				printf("Sudoku Solved!\n");
			} else {
				printf("Sudoku unsolved.\n");
			}
			break;
		}
	}

	return ret;
}