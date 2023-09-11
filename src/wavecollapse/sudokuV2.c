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
int sudokufinal[9][9];
int stopper;

//Guess checker
int sudokuhelper(int x, int y, int guess){
	for(int i = 0; i < 9; i++){
		if(sudokugrid[i][y] == guess && x != i){
			return 1;
		}
	}
	for(int i = 0; i < 9; i++){
		if(sudokugrid[x][i] == guess && y != i){
			return 1;
		}
	}
	int x0 = x - (x % 3);
	int y0 = y - (y % 3);
	int x1 = x0+3;
	int y1 = y0+3;
	for(int i = x0; i < x1; i++){
		for(int j = y0; j < y1; j++){
			if(sudokugrid[i][j] == guess && (x != i || y != j)){
				return 1;
			}
		}
	}
	return 0;
}

//Solution checker
int solutionhelper(){
	int ret = 0;
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			ret += sudokuhelper(i, j, sudokugrid[i][j]);
		}
	}
	return ret;
}

//Two functions call each other
void sudokuwave(int x, int y, int guess, int sudokucollapse[9][9][9], int sudokucount[9][9], int *count);

//Wave function collapse helper function
void sudokufunction(int x, int y, int guess, int sudokucollapse[9][9][9], int sudokucount[9][9], int *count){
	if(sudokucollapse[x][y][guess] == 0){
		sudokucollapse[x][y][guess] = 1;
		sudokucount[x][y]++;
		if(sudokucount[x][y] == 8){
			for(int k = 0; k < 9; k++){
				if(sudokucollapse[x][y][k] == 0){
					sudokugrid[x][y] = k;
					sudokuwave(x, y, k, sudokucollapse, sudokucount, count);
					(*count)++;
				}
			}
		}
	}
}

//Sudoku wave function collapse along adjacent squares
void sudokuwave(int x, int y, int guess, int sudokucollapse[9][9][9], int sudokucount[9][9], int *count){
	sudokucount[x][y] = 9;
	for(int i = 0; i < 9; i++){
		sudokucollapse[x][y][i] = 1;
	}
	for(int i = 0; i < 9; i++){
		if(x != i){
			sudokufunction(i, y, guess, sudokucollapse, sudokucount, count);
		}
	}
	for(int i = 0; i < 9; i++){
		if(y != i){
			sudokufunction(x, i, guess, sudokucollapse, sudokucount, count);
		}
	}
	int x0 = x - (x % 3);
	int y0 = y - (y % 3);
	int x1 = x0+3;
	int y1 = y0+3;
	for(int i = x0; i < x1; i++){
		for(int j = y0; j < y1; j++){
			if(x != i || y != j){	
				sudokufunction(i, j, guess, sudokucollapse, sudokucount, count);
			}
		}
	}
}

void uniquecollapse(int sudokucollapse[9][9][9], int sudokucount[9][9], int *count){
	int guessrow;
	int guesscol;
	int guessbox;
	int rowx = -1;
	int rowy = -1;
	int colx = -1;
	int coly = -1;
	int boxx = -1;
	int boxy = -1;
	for(int k = 0; k < 9; k++){
		for(int i = 0; i < 9; i++){
			guessrow = 0;
			guesscol = 0;
			guessbox = 0;
			for(int j = 0; j < 9; j++){
				if(sudokucollapse[i][j][k] == 0){
					rowx = i;
					rowy = j;
					guessrow++;
				}
				if(sudokucollapse[j][i][k] == 0){
					colx = j;
					coly = i;
					guesscol++;
				}
				if(sudokucollapse[i*3+j%3][j/3+i-i%3][k] == 0){
					boxx = i*3+j%3;
					boxy = j/3+i-i%3;
					guessbox++;
				}
			}
			if(guessrow == 1){
				sudokugrid[rowx][rowy] = k;
				sudokuwave(rowx, rowy, k, sudokucollapse, sudokucount, count);
				(*count)++;
			}
			if(guesscol == 1){
				sudokugrid[colx][coly] = k;
				sudokuwave(colx, coly, k, sudokucollapse, sudokucount, count);
				(*count)++;
			}
			if(guessbox == 1){
				sudokugrid[boxx][boxy] = k;
				sudokuwave(boxx, boxy, k, sudokucollapse, sudokucount, count);
				(*count)++;
			}
		}
	}
}

//Generate new sudokugrid and display it on the main grid
void sudokugenerate(int **grid){
	int sudokucount[9][9];
	int sudokucollapse[9][9][9];
	int randpool[81];
	int stopper;
	int hintcount;
	do{
		stopper = 0;
		//Clear the grid
		for(int i = 0; i < 9; i++){
			for(int j = 0; j < 9; j++){
				sudokugrid[i][j] = EMPTY;
				sudokufinal[i][j] = EMPTY;
				sudokucount[i][j] = 0;
				for(int k = 0; k < 9; k++){
					sudokucollapse[i][j][k] = 0;
				}
			}
		}
		//Generate random pool for easier random coords
		for(int i = 0; i < 82; i++){
			randpool[i] = i;
		}
		int x, y, guess, pool_val;
		int pool_len = 81;
		//Perfectly collapse all possibilities into 1 finished sudoku grid
		for(int i = 0; i < 81; i++){
			do{
				stopper++;
				if(stopper == 10000){
					break;
				}
				pool_val = rand() % pool_len;
				x = randpool[pool_val] % 9;
				y = randpool[pool_val] / 9;
				//x = rand() % 9;
				//y = rand() % 9;
				guess = rand() % 9;
			} while(sudokucollapse[x][y][guess] == 1 || sudokuhelper(x, y, guess));
			if(stopper == 10000){
				break;
			}
			for(int i = pool_val; i < pool_len; i++){
				randpool[i] = randpool[i+1];
			}
			pool_len--;
			sudokugrid[x][y] = guess;
			memcpy(sudokufinal, sudokugrid, sizeof(sudokufinal));
			hintcount = i;
			sudokuwave(x, y, guess, sudokucollapse, sudokucount, &i);
			uniquecollapse(sudokucollapse, sudokucount, &i);
		}
	} while(stopper == 10000 || solutionhelper() || hintcount > 40);
	//Get the number of hints and display the generated sudoku
	int hints = 0;
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(sudokufinal[i][j] > EMPTY){
				grid[i][j] = sudokufinal[i][j]+ONE;
				hints++;
			} else {
				grid[i][j] = CLEAR;
			}
			printf("%d", sudokugrid[i][j]);
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