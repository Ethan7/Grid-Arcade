/* By Ethan Hughes */
/* Written 5/6/2021 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

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

int **undergrid;
int ret;
int stopper;

//Guess checker
int sudokuhelper(int x, int y, int guess){
	for(int i = 0; i < 9; i++){
		if(undergrid[i][y] == guess && x != i){
			return 0;
		}
	}
	for(int i = 0; i < 9; i++){
		if(undergrid[x][i] == guess && y != i){
			return 0;
		}
	}
	int x0 = x - (x % 3);
	int y0 = y - (y % 3);
	int x1 = x0+3;
	int y1 = y0+3;
	for(int i = x0; i < x1; i++){
		for(int j = y0; j < y1; j++){
			if(undergrid[i][j] == guess && (x != i || y != j)){
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
			if(undergrid[i][j] == EMPTY){
				for(int k = 0; k < 9; k++){
					if(sudokuhelper(i, j, k)){
						stopper++;
						if(stopper == 10000){
							return 2;
						}
						undergrid[i][j] = k;
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
						undergrid[i][j] = EMPTY;
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

//Generate new undergrid and display it on the main grid
void sudokugenerate(int **grid){
	do{
		//Clear the grid
		for(int i = 0; i < 9; i++){
			for(int j = 0; j < 9; j++){
				undergrid[i][j] = EMPTY;
			}
		}
		//Add 17 initial random number placements
		int x, y, guess;
		for(int i = 0; i < 17; i++){
			do{
				x = rand() % 9;
				y = rand() % 9;
				guess = rand() % 9;
			} while(sudokuhelper(x, y, guess) == 0);
			undergrid[x][y] = guess;
		}
		stopper = 0;
	} while(sudokusolver() == 0 || stopper == 10000);
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(undergrid[i][j] > EMPTY){
				grid[i][j] = undergrid[i][j]+ONE;
			} else {
				grid[i][j] = CLEAR;
			}
		}
	}
}

int sudoku(int **grid, SDL_Event eventbutton, int game, int t, int cellsize, int width, int height){

	if(t == 1){
		//Allocate grid and set random seed
		srand(time(NULL));
		ret = SUDOKU;
		undergrid = (int **) calloc(9, sizeof(int *));
		for(int i = 0; i < 9; i++){
			undergrid[i] = (int *) calloc(9, sizeof(int));
		}
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
			} else if(grid[buttonx][buttony] > CLEAR && grid[buttonx][buttony] < NINE && undergrid[buttonx][buttony] == EMPTY){
				grid[buttonx][buttony]++;
			} else if(grid[buttonx][buttony] == NINE && undergrid[buttonx][buttony] == EMPTY){
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

	//Free Dynamic Memory
	if(ret == ARCADE){
		for(int i = 0; i < width; i++){
			free(undergrid[i]);
		}
		free(undergrid);
	}

	return ret;
}