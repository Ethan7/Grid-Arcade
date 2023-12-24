/* By Ethan Hughes */
/* Written 12/2/2018 */

#include<SDL2/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"arcade-defs.h"
/*
#define ARCADE 0
#define MAZES 3

#define EMPTY -1
#define WALL 0
#define PLAYER 1
#define START 2
#define END 3
*/
struct wall{
	int x, y; //Position for this wall
	int h, v; //Directional offset for this wall
} *wall_list;

int px = 1, py = 1, temp_width = 3, temp_height = 3, wall_count = 2, fast_maze = 0;
int mazes(int **grid, SDL_Event event, int t, int width, int height){
	int ret = MAZES;

	if(t == 1){
		px = 1;
		py = 1;
		temp_width = width;
		temp_height = height;
		if(width % 2 == 0){
			temp_width--;
		}
		if(height % 2 == 0){
			temp_height--;
		}

		int i = 0, j = 0;
		for(i = 0; i < temp_width; i++){
			for(j = 0; j < temp_height; j++){
				grid[i][j] = WALL;
			}
		}
		grid[1][1] = EMPTY;

		//Generate initial two walls for wall list
		wall_list = (struct wall *) calloc(temp_width*temp_height/4, sizeof(struct wall));
		wall_list[0].x = 1;
		wall_list[0].y = 2;
		wall_list[0].h = 0;
		wall_list[0].v = 1;
		wall_list[1].x = 2;
		wall_list[1].y = 1;
		wall_list[1].h = 1;
		wall_list[1].v = 0;
		wall_count = 2;

		srand(time(NULL));
	}

	//Generate maze with Prim's algorithm
	do{
		if(wall_count > 0){
			//Pick random wall from wall list
			int random = rand() % (wall_count);
			if(random > 0){
				random--;
			}

			//If current wall can be placed
			if(grid[wall_list[random].x+wall_list[random].h][wall_list[random].y+wall_list[random].v] == WALL){
				//Place current wall
				grid[wall_list[random].x][wall_list[random].y] = EMPTY;
				grid[wall_list[random].x+wall_list[random].h][wall_list[random].y+wall_list[random].v] = EMPTY;

				//Add new potential walls to wall list
				if(wall_list[random].x > 2 && grid[wall_list[random].x+wall_list[random].h-2]
					[wall_list[random].y+wall_list[random].v] == WALL){
					wall_list[wall_count].x = wall_list[random].x+wall_list[random].h-1;
					wall_list[wall_count].y = wall_list[random].y+wall_list[random].v;
					wall_list[wall_count].h = -1;
					wall_list[wall_count].v = 0;
					wall_count++;
				}
				if(wall_list[random].x < temp_width-3 && grid[wall_list[random].x+wall_list[random].h+2]
					[wall_list[random].y+wall_list[random].v] == WALL){
					wall_list[wall_count].x = wall_list[random].x+wall_list[random].h+1;
					wall_list[wall_count].y = wall_list[random].y+wall_list[random].v;
					wall_list[wall_count].h = 1;
					wall_list[wall_count].v = 0;
					wall_count++;
				}
				if(wall_list[random].y > 2 && grid[wall_list[random].x+wall_list[random].h]
					[wall_list[random].y+wall_list[random].v-2] == WALL){
					wall_list[wall_count].x = wall_list[random].x+wall_list[random].h;
					wall_list[wall_count].y = wall_list[random].y+wall_list[random].v-1;
					wall_list[wall_count].h = 0;
					wall_list[wall_count].v = -1;
					wall_count++;
				}
				if(wall_list[random].y < temp_height-3 && grid[wall_list[random].x+wall_list[random].h]
					[wall_list[random].y+wall_list[random].v+2] == WALL){
					wall_list[wall_count].x = wall_list[random].x+wall_list[random].h;
					wall_list[wall_count].y = wall_list[random].y+wall_list[random].v+1;
					wall_list[wall_count].h = 0;
					wall_list[wall_count].v = 1;
					wall_count++;
				}
			}

			//Clear current wall and shrink wall list
			for(int i = random; i < wall_count; i++){
				wall_list[i] = wall_list[i+1];
			}
			wall_count--;
		}
	} while (fast_maze && wall_count > 0);

	//Allow player to traverse the maze
	if(event.type == SDL_KEYDOWN){
		switch( event.key.keysym.sym ){
		case SDLK_UP:
			if(grid[px][py-1] != WALL){
				grid[px][py] = EMPTY;
				py -= 1;
			}
			break;
		case SDLK_DOWN:
			if(grid[px][py+1] != WALL){
				grid[px][py] = EMPTY;
				py += 1;
			}
			break;
		case SDLK_LEFT:
			if(grid[px-1][py] != WALL){
				grid[px][py] = EMPTY;
				px -= 1;
			}
			break;
		case SDLK_RIGHT:
			if(grid[px+1][py] != WALL){
				grid[px][py] = EMPTY;
				px += 1;
			}
			break;
		case SDLK_RETURN:
			fast_maze = 1;
		}
	}

	//Display start, player, and end point
	grid[1][1] = START;
	if(wall_count == 0){
		fast_maze = 0;
		grid[temp_width-2][temp_height-2] = END;
	}
	grid[px][py] = PLAYER;

	//Check if player has completed maze
	if(px == temp_width-2 && py == temp_height-2){
		printf("YOU WIN!\n");
		printf("Time to beat: %d\n", t);
		ret = ARCADE;
	}

	//Memory Cleanup
	if(ret == ARCADE){
		free(wall_list);
	}

	return ret;
}
