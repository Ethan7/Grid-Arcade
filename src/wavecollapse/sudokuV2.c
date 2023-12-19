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
int sudoku_grid[9][9];
int sudoku_final[9][9];
int stopper;

//Guess checker
int sudoku_helper(int x, int y, int guess){
	for(int i = 0; i < 9; i++){
		if(sudoku_grid[i][y] == guess && x != i){
			return 1;
		}
	}
	for(int i = 0; i < 9; i++){
		if(sudoku_grid[x][i] == guess && y != i){
			return 1;
		}
	}
	int x0 = x - (x % 3);
	int y0 = y - (y % 3);
	int x1 = x0+3;
	int y1 = y0+3;
	for(int i = x0; i < x1; i++){
		for(int j = y0; j < y1; j++){
			if(sudoku_grid[i][j] == guess && (x != i || y != j)){
				return 1;
			}
		}
	}
	return 0;
}

//Solution checker
int solution_helper(){
	int ret = 0;
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			ret += sudoku_helper(i, j, sudoku_grid[i][j]);
		}
	}
	return ret;
}

//Two functions call each other
void sudoku_wave(int x, int y, int guess, int sudoku_collapse[9][9][9], int sudoku_count[9][9], int *count);

//Wave function collapse helper function
void sudoku_function(int x, int y, int guess, int sudoku_collapse[9][9][9], int sudoku_count[9][9], int *count){
	if(sudoku_collapse[x][y][guess] == 0){
		sudoku_collapse[x][y][guess] = 1;
		sudoku_count[x][y]++;
		if(sudoku_count[x][y] == 8){
			for(int k = 0; k < 9; k++){
				if(sudoku_collapse[x][y][k] == 0){
					sudoku_grid[x][y] = k;
					sudoku_wave(x, y, k, sudoku_collapse, sudoku_count, count);
					(*count)++;
				}
			}
		}
	}
}

//Sudoku wave function collapse along adjacent squares
void sudoku_wave(int x, int y, int guess, int sudoku_collapse[9][9][9], int sudoku_count[9][9], int *count){
	sudoku_count[x][y] = 9;
	for(int i = 0; i < 9; i++){
		sudoku_collapse[x][y][i] = 1;
	}
	for(int i = 0; i < 9; i++){
		if(x != i){
			sudoku_function(i, y, guess, sudoku_collapse, sudoku_count, count);
		}
	}
	for(int i = 0; i < 9; i++){
		if(y != i){
			sudoku_function(x, i, guess, sudoku_collapse, sudoku_count, count);
		}
	}
	int x0 = x - (x % 3);
	int y0 = y - (y % 3);
	int x1 = x0+3;
	int y1 = y0+3;
	for(int i = x0; i < x1; i++){
		for(int j = y0; j < y1; j++){
			if(x != i || y != j){	
				sudoku_function(i, j, guess, sudoku_collapse, sudoku_count, count);
			}
		}
	}
}

void unique_collapse(int sudoku_collapse[9][9][9], int sudoku_count[9][9], int *count){
	int guess_row;
	int guess_col;
	int guess_box;
	int row_x = -1;
	int row_y = -1;
	int col_x = -1;
	int col_y = -1;
	int box_x = -1;
	int box_y = -1;
	for(int k = 0; k < 9; k++){
		for(int i = 0; i < 9; i++){
			guess_row = 0;
			guess_col = 0;
			guess_box = 0;
			for(int j = 0; j < 9; j++){
				if(sudoku_collapse[i][j][k] == 0){
					row_x = i;
					row_y = j;
					guess_row++;
				}
				if(sudoku_collapse[j][i][k] == 0){
					col_x = j;
					col_y = i;
					guess_col++;
				}
				if(sudoku_collapse[i*3+j%3][j/3+i-i%3][k] == 0){
					box_x = i*3+j%3;
					box_y = j/3+i-i%3;
					guess_box++;
				}
			}
			if(guess_row == 1){
				sudoku_grid[row_x][row_y] = k;
				sudoku_wave(row_x, row_y, k, sudoku_collapse, sudoku_count, count);
				(*count)++;
			}
			if(guess_col == 1){
				sudoku_grid[col_x][col_y] = k;
				sudoku_wave(col_x, col_y, k, sudoku_collapse, sudoku_count, count);
				(*count)++;
			}
			if(guess_box == 1){
				sudoku_grid[box_x][box_y] = k;
				sudoku_wave(box_x, box_y, k, sudoku_collapse, sudoku_count, count);
				(*count)++;
			}
		}
	}
}

//Generate new sudoku_grid and display it on the main grid
void sudoku_generate(int **grid){
	int sudoku_count[9][9];
	int sudoku_collapse[9][9][9];
	int rand_pool[81];
	int stopper;
	int hint_count;
	do{
		stopper = 0;
		//Clear the grid
		for(int i = 0; i < 9; i++){
			for(int j = 0; j < 9; j++){
				sudoku_grid[i][j] = EMPTY;
				sudoku_final[i][j] = EMPTY;
				sudoku_count[i][j] = 0;
				for(int k = 0; k < 9; k++){
					sudoku_collapse[i][j][k] = 0;
				}
			}
		}
		//Generate random pool for easier random coords
		for(int i = 0; i < 82; i++){
			rand_pool[i] = i;
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
				x = rand_pool[pool_val] % 9;
				y = rand_pool[pool_val] / 9;
				//x = rand() % 9;
				//y = rand() % 9;
				guess = rand() % 9;
			} while(sudoku_collapse[x][y][guess] == 1 || sudoku_helper(x, y, guess));
			if(stopper == 10000){
				break;
			}
			for(int i = pool_val; i < pool_len; i++){
				rand_pool[i] = rand_pool[i+1];
			}
			pool_len--;
			sudoku_grid[x][y] = guess;
			memcpy(sudoku_final, sudoku_grid, sizeof(sudoku_final));
			hint_count = i;
			sudoku_wave(x, y, guess, sudoku_collapse, sudoku_count, &i);
			unique_collapse(sudoku_collapse, sudoku_count, &i);
		}
	} while(stopper == 10000 || solution_helper() || hint_count > 40);
	//Get the number of hints and display the generated sudoku
	int hints = 0;
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(sudoku_final[i][j] > EMPTY){
				grid[i][j] = sudoku_final[i][j]+ONE;
				hints++;
			} else {
				grid[i][j] = CLEAR;
			}
			printf("%d", sudoku_grid[i][j]);
		}
		printf("\n");
	}
	printf("number of hints: %d\n", hints);
}

int sudoku(int **grid, SDL_Event event, int t, int cellsize, int width, int height){	
	int ret = SUDOKU;

	if(t == 1){
		//Allocate grid and set random seed
		srand(time(NULL));
		
		//Generate Sudoku
		sudoku_generate(grid);
	}

	//Respond to user input
	if(event.type == SDL_MOUSEBUTTONUP){
		int button_x = event.button.x / cellsize;
		int button_y = event.button.y / cellsize;
		if(event.button.button == SDL_BUTTON_LEFT && button_x < 9 && button_y < 9 && button_x > -1 && button_y > -1){
			//Add to current grid number
			if(grid[button_x][button_y] == CLEAR){
				grid[button_x][button_y] = ONE;
			} else if(grid[button_x][button_y] > CLEAR && grid[button_x][button_y] < NINE && sudoku_final[button_x][button_y] == EMPTY){
				grid[button_x][button_y]++;
			} else if(grid[button_x][button_y] == NINE && sudoku_final[button_x][button_y] == EMPTY){
				grid[button_x][button_y] = CLEAR;
			}
		}
	} else if(event.type == SDL_KEYDOWN){
		int solved = 1;
		switch( event.key.keysym.sym ){
		case SDLK_r:
			//Generate new Sudoku
			sudoku_generate(grid);
			break;
		case SDLK_RETURN:
			//Check if sudoku has been solved
			solved = 1;
			for(int i = 0; i < 9; i++){
				for(int j = 0; j < 9; j++){
					if(grid[i][j] == CLEAR || sudoku_helper(i, j, grid[i][j]-ONE) == 0){
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