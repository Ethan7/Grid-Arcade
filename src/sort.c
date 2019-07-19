/* By Ethan Hughes */
/* Written 7/5/2019 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>

#define ARCADE 0
#define SORT 14

#define EMPTY -1
#define FILLED 0

//Recursive merging function, returns start of sorted list
int *merge(int len, int arr[], int a, int b){
	if(len == 1){ //Return single value
		return arr;
	} else if (len == 2){ //Check whether to swap when given two values
		if(arr[a] > arr[b]){
			int temp = arr[a];
			arr[a] = arr[b];
			arr[b] = temp;
			return arr;
		} else {
			return arr;
		}
	} else { //Run recursive merge when given more than two values
		int c = (a+b)/2;
		int d = c+1;

		merge(len-len/2, arr, a, c);

		merge(len/2, arr, d, b);

		//Sort returned values and return sorted sub-list
		int temp[len];
		int c1 = a;
		int c2 = d;
		for(int i = 0; i < len; i++){
			if(c1 < d && c2 <= b){
				if(arr[c1] <= arr[c2]){
					temp[i] = arr[c1];
					c1++;
				} else {
					temp[i] = arr[c2];
					c2++;
				}
			} else if(c1 < d){
				temp[i] = arr[c1];
				c1++;
			} else {
				temp[i] = arr[c2];
				c2++;
			}
		}
		for(int i = 0; i < len; i++){
			arr[a+i] = temp[i];
		}
		//free(temp);
		return arr;
	}
}

//Base merge sorting function, implements recursive function, returns start of sorted list
int *mergesort(int len, int values[]){
	if(len > 1){
		merge(len, values, 0, len-1);
	}
	return values;
}

int *gridarray;

int sort(int **grid, int game, int t, int width, int height){

	//Respond to user input
	if(t == 1){
		gridarray = (int *) calloc(width, sizeof(int));
		for(int i = 0; i < width; i++){
			gridarray[i] = 0;
			for(int j = 0; j < height; j++){
				if(grid[i][j] == FILLED){
					gridarray[i]++;
				}
			}
		}
	}

	gridarray = mergesort(width, gridarray);
	for(int i = 0; i < width; i++){
		
	}

	return SORT;
}