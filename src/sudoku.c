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
int sudokugrid[9][9];
int stopper;

//Guess checker
int sudokuhelper(int x, int y, int guess){
	for(int i = 0; i < 9; i++){
		if(sudokugrid[i][y] == guess){
			return 0;
		}
	}
	for(int i = 0; i < 9; i++){
		if(sudokugrid[x][i] == guess){
			return 0;
		}
	}
	int x0 = x - (x % 3);
	int y0 = y - (y % 3);
	int x1 = x0+3;
	int y1 = y0+3;
	for(int i = x0; i < x1; i++){
		for(int j = y0; j < y1; j++){
			if(sudokugrid[i][j] == guess){
				return 0;
			}
		}
	}
	return 1;
}

//Recursive backtracking while counting solutions to generate sudoku grid
int sudokusolver(){
	int solutions = 0;
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(sudokugrid[i][j] == EMPTY){
				for(int k = 0; k < 9; k++){
					if(sudokuhelper(i, j, k)){
						stopper++;
						if(stopper == 10000){
							return 2;
						}
						sudokugrid[i][j] = k;
						//Recursive call until the sudoku is solved
						int test = 0;
						if((test = sudokusolver())){
							if(test == 2){
								return 2;
							}
							solutions++;
							if(solutions == 2){
								return 1;
							}
						}
						sudokugrid[i][j] = EMPTY;
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
	do{
		//Clear the grid
		for(int i = 0; i < 9; i++){
			for(int j = 0; j < 9; j++){
				sudokugrid[i][j] = EMPTY;
			}
		}
		//Add 17 initial random number placements
		int x, y, guess;
		for(int i = 0; i < 17; i++){
			do{
				x = rand() % 9;
				y = rand() % 9;
				guess = rand() % 9;
			} while(sudokugrid[x][y] != EMPTY || sudokuhelper(x, y, guess) == 0);
			sudokugrid[x][y] = guess;
		}
		stopper = 0;
	} while(sudokusolver() == 0 || stopper == 10000);
	int hints = 0;
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(sudokugrid[i][j] > EMPTY){
				grid[i][j] = sudokugrid[i][j]+ONE;
				hints++;
			} else {
				grid[i][j] = CLEAR;
			}
		}
	}
	printf("number of hints: %d\n", hints);
}

int sudoku(int **grid, SDL_Event eventbutton, int t, int cellsize, int width, int height){	
	int ret = SUDOKU;

	if(t == 1){
		//set random seed
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
			} else if(grid[buttonx][buttony] > CLEAR && grid[buttonx][buttony] < NINE && sudokugrid[buttonx][buttony] == EMPTY){
				grid[buttonx][buttony]++;
			} else if(grid[buttonx][buttony] == NINE && sudokugrid[buttonx][buttony] == EMPTY){
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
					if(grid[i][j] == CLEAR || sudokuhelper(i, j, grid[i][j]-ONE) == 0){
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