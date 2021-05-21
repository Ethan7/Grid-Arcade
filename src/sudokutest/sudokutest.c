/* By Ethan Hughes */
/* Written 5/9/2021 */

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int **undergrid;
int stopper;
int solutions;

//Guess checking
int sudokuhelper(int x, int y, int guess){
	for(int i = 0; i < 9; i++){
		if(undergrid[i][y] == guess){
			return 0;
		}
	}
	for(int i = 0; i < 9; i++){
		if(undergrid[x][i] == guess){
			return 0;
		}
	}
	int x0 = x - (x % 3);
	int y0 = y - (y % 3);
	int x1 = x0+3;
	int y1 = y0+3;
	for(int i = x0; i < x1; i++){
		for(int j = y0; j < y1; j++){
			if(undergrid[i][j] == guess){
				return 0;
			}
		}
	}
	return 1;
}

//Recursive backtracking solution counting algorithm
int sudokushrinker(){
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(undergrid[i][j] == -1){
				for(int k = 0; k < 9; k++){
					if(sudokuhelper(i, j, k)){
						undergrid[i][j] = k;
						//Recursive call until the sudoku is solved
						if(sudokushrinker()){
							solutions++;
						}
						undergrid[i][j] = -1;
					}
				}
				return 0;
			}
		}
	}
	return 1;
}

//Recursive backtracking algorithm
int sudokusolver(){
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(undergrid[i][j] == -1){
				for(int k = 0; k < 9; k++){
					if(sudokuhelper(i, j, k)){
						stopper++;
						if(stopper == 1000){
							return 1;
						}
						undergrid[i][j] = k;
						//Recursive call until the sudoku is solved
						if(sudokusolver()){
							return 1;
						}
						undergrid[i][j] = -1;
					}
				}
				return 0;
			}
		}
	}
	return 1;
}

//Recursive backtracking and solution storing algorithm
int sudokusolver3(){
	int solution = 0;
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(undergrid[i][j] == -1){
				for(int k = 0; k < 9; k++){
					if(sudokuhelper(i, j, k)){
						stopper++;
						if(stopper == 10000){
							return 2;
						}
						undergrid[i][j] = k;
						//Recursive call until the sudoku is solved
						int test = 0;
						if((test = sudokusolver3())){
							if(test == 2){
								return 2;
							}
							solution++;
							if(solution == 2){
								return 1;
							}
						}
						undergrid[i][j] = -1;
					}
				}
				if(solution == 1){
					return 1;
				}
				return 0;
			}
		}
	}
	return 1;
}

//Random placement and checking algorithm
int sudokusolver2(){
	int breakout = 1;
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			int guess;
			for(int k = 0; k < 9; k++){
				stopper++;
				guess = rand() % 9;
				if(sudokuhelper(i, j, guess)){
					undergrid[i][j] = guess;
					break;
				}
			}
			if(undergrid[i][j] == -1){
				breakout = 0;
				break;
			}
		}
		if(breakout == 0){
			break;
		}
	}
	return breakout;
}

//Print grid
void printboard(){
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(undergrid[i][j] == -1){
				printf(" ");
			} else {
				printf("%d", undergrid[i][j]);
			}
		}
		printf("\n");
	}
}

int main(){
	//allocate grid and set random seed
	srand(time(NULL));
	undergrid = (int **) calloc(9, sizeof(int *));
	for(int i = 0; i < 9; i++){
		undergrid[i] = (int *) calloc(9, sizeof(int));
		for(int j = 0; j < 9; j++){
			undergrid[i][j] = -1;
		}
	}

	//Test time for 100 sudoku grids with backtracking
	int fails = 0;
	clock_t t;
    t = clock();
	for(int n = 0; n < 100; n++){
		fails--;
		//Clear the board
		for(int i = 0; i < 9; i++){
			for(int j = 0; j < 9; j++){
				undergrid[i][j] = -1;
			}
		}
		do{
			for(int i = 0; i < 9; i++){
				for(int j = 0; j < 9; j++){
					undergrid[i][j] = -1;
				}
			}
			fails++;
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
			//Add stopper to clear random initinal placement
		} while(sudokusolver() == 0 || stopper == 1000);
		//Shrink sudoku while only 1 solution
		int x, y, value;
		do{
			x = rand() % 9;
			y = rand() % 9;
			value = undergrid[x][y];
			undergrid[x][y] = -1;
			solutions = 0;
			sudokushrinker();
		} while(solutions == 1);
		undergrid[x][y] = value;
		//printboard();
	}
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
	//Output test results
	printf("time: %f\n", time_taken);
	printf("stopper: %d\n", stopper);
	printf("fails = %d\n", fails);


	//Test time for 100 sudoku grids with random placement
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			undergrid[i][j] = -1;
		}
	}
	fails = 0;
	t = clock();
	for(int n = 0; n < 100; n++){
		//Clear grid
		for(int i = 0; i < 9; i++){
			for(int j = 0; j < 9; j++){
				undergrid[i][j] = -1;
			}
		}
		while(sudokusolver2() == 0){
			fails++;
			//Add stopper to clear broken boards
			if(stopper > 100000){
				for(int i = 0; i < 9; i++){
					for(int j = 0; j < 9; j++){
						undergrid[i][j] = -1;
					}
				}
				stopper = 0;
			}
		}

		//Shrink sudoku while only 1 solution
		int x, y, value;
		do{
			x = rand() % 9;
			y = rand() % 9;
			value = undergrid[x][y];
			undergrid[x][y] = -1;
			solutions = 0;
			sudokushrinker();
		} while(solutions == 1);
		undergrid[x][y] = value;
		//printboard();
	}
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
	printf("time: %f\n", time_taken);
	printf("stopper: %d\n", stopper);
	printf("fails = %d\n", fails);

	//Free memory
	for(int i = 0; i < 9; i++){
		free(undergrid[i]);
	}
	free(undergrid);

	return 0;
}