/* By Ethan Hughs */
/* Written 12/1/2018 */

#ifdef _WIN32
	#define SDL_MAIN_HANDLED
	#include<SDL.h>
#else
	#include<SDL2/SDL.h>
#endif
#include<SDL2/SDL_image.h>
#include<stdio.h>
#include<stdlib.h>
#include"arcade-defs.h"
/*
//Game defines
#define ARCADE 0
#define SNAKE 1
#define PATH 2
#define MAZES 3
#define PONG 4
#define TETRIS 5
#define SPACE 6
#define FROGGER 7
#define CONWAY 8
#define LANGSTON 9
#define FLAPPY 10
#define CONNECT4 11
#define CHECKERS 12
#define MINES 13
#define CHESS 14
#define BATTLE1 15
#define BATTLE2 16
#define SUDOKU 17
#define BREAKOUT 18
#define COLLAPSE 19
#define TRON 20
#define SETTINGS 19
#define SETTINGS2 20
#define SETTINGS3 21
#define SETUP 22

//Color defines
#define BLACK -1
#define WHITE 0
#define RED 1
#define GREEN 2
#define BLUE 3
#define YELLOW 4
#define MAGENTA 5
#define CYAN 6

//Number image defines
#define ZERO 7
#define ONE 8
#define TWO 9
#define THREE 10
#define FOUR 11
#define FIVE 12
#define SIX 13
#define SEVEN 14
#define EIGHT 15
#define NINE 16

//Chess defines
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

//Menu image defines
#define TITLE 38
#define settings_img 59
#define fullscreen_img 30
#define confirm_img 31
#define cancel_img 32
#define width_img 33
#define height_img 34
#define cellsize_img 35
#define arrow_img 36
#define arrow_flip 37

#define TEXTURES 58

//Menu defines
#define ROWS 22 //Main menu two-sided rows
#define SROWS 10 //Settings menu rows
*/
const char *texturenames[TEXTURES] = { "./img/black.png", "./img/white.png", "./img/red.png", "./img/green.png", "./img/blue.png", "./img/yellow.png", "./img/magenta.png", "./img/cyan.png", "./img/zero.png", "./img/one.png", "./img/two.png", "./img/three.png", "./img/four.png", "./img/five.png", "./img/six.png", "./img/seven.png", "./img/eight.png", "./img/nine.png", "./img/p1pawn.png", "./img/p1rook.png", "./img/p1bishop.png", "./img/p1knight.png", "./img/p1queen.png", "./img/p1king.png", "./img/p2pawn.png", "./img/p2rook.png", "./img/p2bishop.png", "./img/p2knight.png", "./img/p2queen.png", "./img/p2king.png", "./img/fullscreen.png", "./img/confirm.png", "./img/cancel.png", "./img/width.png", "./img/height.png", "./img/cellsize.png", "./img/arrow.png", "./img/arrow_flip.png", "./img/title.png", "./img/snake.png", "./img/path.png", "./img/mazes.png", "./img/pong.png", "./img/tetris.png", "./img/space.png", "./img/frogger.png", "./img/conway.png", "./img/langston.png", "./img/flappy.png", "./img/connect4.png", "./img/checkers.png", "./img/mines.png", "./img/chess.png", "./img/battle1.png", "./img/battle2.png", "./img/sudoku.png", "./img/breakout.png", "./img/collapse.png", "./img/snaketron.png", "./img/settings.png"};

void clear(int **grid, int width, int height){
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			grid[i][j] = -1;
		}
	}
}

enum GAMEMODE arcade(SDL_Event event, int width, int height, enum GAMEMODE *setupgame){
	if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT){
		int index = ((event.button.y / (height/((ROWS/2)+2))) * 2) - 2 + (event.button.x / (width/2));
		switch(index){
		case 2:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
			return index-1;
		case 3:
		case 9:
		case 10:
			*setupgame = index-1;
			return SETUP;
		default:
			break;
		}
	}

	return ARCADE;
}

enum GAMEMODE settings(SDL_Event event, int height, int *placemarker, int *wentry, int *hentry, int *centry){
	if(event.type == SDL_KEYDOWN){
		int key = -1;
		switch( event.key.keysym.sym ){
		case SDLK_0:
			key = 0;
			break;
		case SDLK_1:
			key = 1;
			break;
		case SDLK_2:
			key = 2;
			break;
		case SDLK_3:
			key = 3;
			break;
		case SDLK_4:
			key = 4;
			break;
		case SDLK_5:
			key = 5;
			break;
		case SDLK_6:
			key = 6;
			break;
		case SDLK_7:
			key = 7;
			break;
		case SDLK_8:
			key = 8;
			break;
		case SDLK_9:
			key = 9;
			break;
		default:
			break;
		}
		if(key != -1){
			if(*placemarker == -1){
			} else if (*placemarker / 3 == 0){
				*wentry = *wentry * 10 + key; 
			} else if(*placemarker / 3 == 1){
				*hentry = *hentry * 10 + key; 
			} else if(*placemarker / 3 == 2){
				*centry = *centry * 10 + key; 
			}
			if(*placemarker % 3 == 2){
				*placemarker = -1;
			} else {
				*placemarker += 1;
			}
		}
	}

	if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT){
		switch(event.button.y / (height/SROWS)){
		case 2:
			*placemarker = 0;
			*wentry = 0;
			break;
		case 3:
			*placemarker = 3;
			*hentry = 0;
			break;
		case 4:
			*placemarker = 6;
			*centry = 0;
			break;
		case 6:
			return SETTINGS3;
		case 7:
			if(*wentry < 9){
				*wentry = 9;
			}
			if(*hentry < 9){
				*hentry = 9;
			}
			if(*centry == 0){
				*centry = 1;
			}
			return SETTINGS2;
		case 8:
			return ARCADE;
		default:
			break;
		}
	}

	return SETTINGS;
}

int snake(int **grid, SDL_Event event, int t, int width, int height, int wrap);

int path(int **grid, SDL_Event event, int t, int width, int height);

int mazes(int **grid, SDL_Event event, int t, int width, int height);

int pong(int **grid, SDL_Event event, int t, int width, int height);

int tetris(int **grid, SDL_Event event, int t, int width, int height);

int conway(int **grid, int t, int width, int height);

int langston(int **grid, int t, int width, int height);

int space(int **grid, SDL_Event event, int t, int width, int height);

int frogger(int **grid, SDL_Event event, int t, int width, int height);

int connect4(int **grid, SDL_Event event, int t, int width, int height);

int flappy(int **grid, SDL_Event event, int t, int width, int height);

int checkers(int **grid, SDL_Event eventbutton, int t, int cellsize, int width, int height);

int mines(int **grid, SDL_Event eventbutton, int t, int cellsize, int width, int height);

int chess(int **grid, SDL_Event eventbutton, int t, int cellsize);

int battle1(int **grid, SDL_Event eventbutton, int t, int cellsize, int width, int height);

int battle2(int **grid, SDL_Event eventbutton, int t, int cellsize, int width, int height);

int sudoku(int **grid, SDL_Event eventbutton, int t, int cellsize, int width, int height);

int breakout(int **grid, SDL_Event event, int t, int width, int height);

int collapse(int **grid, SDL_Event event, int t, int width, int height);

int tron(int **grid, SDL_Event event, int t, int width, int height);

int setup(int **grid, SDL_Event eventbutton, enum GAMEMODE setupgame, int cellsize, int width, int height);

int main(int argc, char **argv){
	int cellsize = 32; //Grid cell size
	int width = 20; //Grid width
	int height = 15; //Grid height

	//Possible starting cases
	switch(argc){
		case 1:
			//Set Nothing
			break;
		case 2:
			cellsize = atoi(argv[1]);
			if(cellsize < 1){
				fprintf(stderr, "Invalid cellsize needs to be > 0\n");
				return EXIT_FAILURE;
			}
			break;
		case 4:
			cellsize = atoi(argv[1]);
			width = atoi(argv[2]);
			height = atoi(argv[3]);
			//Ensure minimum screen size.
			if(width < 9){
				fprintf(stderr, "Invalid width needs to be > 7\n");
				return EXIT_FAILURE;
			} else if(height < 9){
				fprintf(stderr, "Invalid height needs to be > 7\n");
				return EXIT_FAILURE;
			} else if(cellsize < 1){
				fprintf(stderr, "Invalid cellsize needs to be > 0\n");
				return EXIT_FAILURE;
			}
			break;
		default:
			fprintf(stderr, "usage: ./arcade [tile size] [width] [height]\n");
			return EXIT_FAILURE;
	}
	if(width * cellsize < 64 || height * cellsize < 64){
		fprintf(stderr, "Invalid width, height, or cellsize\n");
		fprintf(stderr, "width * cellsize must be > 63\n");
		fprintf(stderr, "height * cellsize must be > 63\n");
		return EXIT_FAILURE;
	}

	int fullwidth = width * cellsize;
	int fullheight = height * cellsize;

	//SDL2 Stuff

	SDL_SetMainReady();

	SDL_Window *window = NULL;

	SDL_Surface *screenSurface = NULL;

	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ){
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	} else {
		window = SDL_CreateWindow( "Grid Arcade", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		 fullwidth, fullheight, SDL_WINDOW_SHOWN );
		if(window == NULL){
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		} else {
			//Initialize PNG loading 
			int imgFlags = IMG_INIT_PNG;
			if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
				printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
			}
		}
	}

	screenSurface = SDL_GetWindowSurface( window );

	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF) );

	SDL_UpdateWindowSurface( window );

	SDL_Event event;

	SDL_Delay( 1000 );

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = cellsize;
	rect.h = cellsize;

	//Load all Texture Surfaces
	SDL_Surface **texture_img = (SDL_Surface **) calloc(TEXTURES, sizeof(SDL_Surface *));
	for(int i = 0; i < TEXTURES; i++){
		texture_img[i] = IMG_Load(texturenames[i]);
		texture_img[i] = SDL_ConvertSurface( texture_img[i], screenSurface->format, 0);
	}
	
	//End SDL2 Stuff

	//Allocate for grid and lastgrid
	int **grid = (int **) calloc(width, sizeof(int *)); //Used to display grid of tiles
	int **lastgrid = (int **) calloc(width, sizeof(int *)); //Used to keep track of last grid to speed up graphics
	for(int i = 0; i < width; i++){
		grid[i] = (int *) calloc(height, sizeof(int));
		lastgrid[i] = (int *) calloc(height, sizeof(int));
		for(int j = 0; j < height; j++){
			grid[i][j] = -1;
			lastgrid[i][j] = -1;
		}
	}

	int running = 1; //Whether game is running
	int t = 0; //Running time
	int paused = 0; //Whether the game is paused
	int speed = 256; //Speed for the gameplay loop
	enum GAMEMODE game = ARCADE; //Current game within the arcade
	enum GAMEMODE setupgame = CONWAY; //Game which is currently being setup
	int wentry = width; //Width setting
	int hentry = height; //Height setting
	int centry = cellsize; //Cellsize setting
	int placemarker = 0; //Placemarker for which digit of which setting you're changing
	int delay = 0; //Allows certain games and projects to display output one last time
	SDL_Event buttonheld;
	buttonheld.type = SDL_USEREVENT;

	//Gameplay Loop
	while(running){

		t++;

		//Input Step
		SDL_Event leftrightbutton; //stores mouse buttons + return and r keys
		leftrightbutton.type = SDL_USEREVENT;
		SDL_Event direction; //stores direction + return keys
		direction.type = SDL_USEREVENT;

		while(SDL_PollEvent( &event ) != 0 || paused){
			if(buttonheld.type != SDL_USEREVENT){
				buttonheld.button.x = event.button.x;
				buttonheld.button.y = event.button.y;
			}
			switch(event.type){
			case SDL_KEYDOWN:
				if(game == SETTINGS){
					leftrightbutton = event;
				}
				switch( event.key.keysym.sym ){
				case SDLK_ESCAPE:
					paused = 0;
					running = 0;
					break;
				case SDLK_SPACE:
					paused = 1 - paused;
					break;
				case SDLK_RETURN:
					leftrightbutton = event;
					buttonheld = event;
					direction = event;
					break;
				case SDLK_TAB:
					if(speed == 1){
						speed = 256;
					} else {
						speed >>= 1;
					}
					break;
				case SDLK_BACKSPACE:
					game = ARCADE;
					clear(grid, width, height);
					t = 0;
					break;
				case SDLK_UP:
					direction = event;
					break;
				case SDLK_DOWN:
					direction = event;
					break;
				case SDLK_LEFT:
					direction = event;
					break;
				case SDLK_RIGHT:
					direction = event;
					break;
				case SDLK_w:
					direction = event;
					break;
				case SDLK_s:
					direction = event;
					break;
				case SDLK_a:
					direction = event;
					break;
				case SDLK_d:
					direction = event;
					break;
				case SDLK_r:
					leftrightbutton = event;
					break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch ( event.button.button ){
				case SDL_BUTTON_LEFT:
					buttonheld = event;
					break;
				case SDL_BUTTON_RIGHT:
					buttonheld = event;
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch ( event.button.button ){
				case SDL_BUTTON_LEFT:
					leftrightbutton = event;
					buttonheld.type = SDL_USEREVENT;
					break;
				case SDL_BUTTON_RIGHT:
					leftrightbutton = event;
					buttonheld.type = SDL_USEREVENT;
					break;
				}
				break;
			case SDL_QUIT:
				paused = 0;
				running = 0;
				break;
			default:
				break;
			}
		}

		//Game Step
		switch(game){
		case ARCADE:
			if((game = arcade(leftrightbutton, fullwidth, fullheight, &setupgame)) != ARCADE){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00) );
				t = 0;
			}
			break;
		case SNAKE:
			if((game = snake(grid, direction, t, width, height, 0)) != SNAKE){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case PATH:
			if((game = path(grid, event, t, width, height)) != PATH){
				game = PATH;
				delay = 1;
			}
			break;
		case MAZES:
			if((game = mazes(grid, direction, t, width, height)) != MAZES){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case PONG:
			if((game = pong(grid, direction, t, width, height)) != PONG){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case TETRIS:
			if((game = tetris(grid, direction, t, width, height)) != TETRIS){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case CONWAY:
			if((game = conway(grid, t, width, height)) != CONWAY){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case SPACE:
			if((game = space(grid, direction, t, width, height)) != SPACE){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case FROGGER:
			if((game = frogger(grid, direction, t, width, height)) != FROGGER){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case LANGSTON:
			if((game = langston(grid, t, width, height)) != LANGSTON){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case CONNECT4:
			if((game = connect4(grid, direction, t, width, height)) != CONNECT4){
				game = CONNECT4;
				delay = 1;
			}
			break;
		case FLAPPY:
			if((game = flappy(grid, direction, t, width, height)) != FLAPPY){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case CHECKERS:
			if((game = checkers(grid, leftrightbutton, t, cellsize, width, height)) != CHECKERS){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case MINES:
			if((game = mines(grid, leftrightbutton, t, cellsize, width, height)) != MINES){
				game = MINES;
				delay = 1;
			}
			break;
		case CHESS:
			if((game = chess(grid, leftrightbutton, t, cellsize)) != CHESS){
				game = CHESS;
				delay = 1;
			}
			break;
		case BATTLE1:
			if((game = battle1(grid, leftrightbutton, t, cellsize, width, height)) != BATTLE1){
				game = BATTLE1;
				delay = 1;
			}
			break;
		case BATTLE2:
			if((game = battle2(grid, leftrightbutton, t, cellsize, width, height)) != BATTLE2){
				game = BATTLE2;
				delay = 1;
			}
			break;
		case SUDOKU:
			if((game = sudoku(grid, leftrightbutton, t, cellsize, width, height)) != SUDOKU){
				game = SUDOKU;
				delay = 1;
			}
			break;
		case BREAKOUT:
			if((game = breakout(grid, direction, t, width, height)) != BREAKOUT){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case COLLAPSE:
			if((game = collapse(grid, direction, t, width, height)) != COLLAPSE){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case TRON:
			if((game = tron(grid, direction, t, width, height)) != TRON){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case SETUP:
			if((game = setup(grid, buttonheld, setupgame, cellsize, width, height)) != SETUP){
				t = 0;
			}
			break;
		case SETTINGS:
			if((game = settings(leftrightbutton, fullheight, &placemarker, &wentry, &hentry, &centry)) != SETTINGS){
				if(game == SETTINGS2){
					for(int i = 0; i < width; i++){
						free(grid[i]);
						free(lastgrid[i]);
					}
					free(grid);
					free(lastgrid);

					SDL_SetWindowFullscreen(window, 0);
					
					cellsize = centry;
					width = wentry;
					height = hentry;
					fullwidth = width * cellsize;
					fullheight = height * cellsize;
					
					SDL_SetWindowSize(window, fullwidth, fullheight);
					screenSurface = SDL_GetWindowSurface( window );

					grid = (int **) calloc(width, sizeof(int *));
					lastgrid = (int **) calloc(width, sizeof(int *));
					for(int i = 0; i < width; i++){
						grid[i] = (int *) calloc(height, sizeof(int));
						lastgrid[i] = (int *) calloc(height, sizeof(int));
					}

					clear(grid, width, height);
					clear(lastgrid, width, height);
					game = ARCADE;
				} else if(game == SETTINGS3){
					for(int i = 0; i < width; i++){
						free(grid[i]);
						free(lastgrid[i]);
					}
					free(grid);
					free(lastgrid);

					SDL_DisplayMode DM;
					SDL_GetCurrentDisplayMode(0, &DM);
					fullwidth = DM.w;
					fullheight = DM.h;
					
					SDL_SetWindowSize(window, fullwidth, fullheight);
					SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
					screenSurface = SDL_GetWindowSurface( window );

					width = fullwidth / cellsize;
					height = fullheight / cellsize;

					grid = (int **) calloc(width, sizeof(int *));
					lastgrid = (int **) calloc(width, sizeof(int *));
					for(int i = 0; i < width; i++){
						grid[i] = (int *) calloc(height, sizeof(int));
						lastgrid[i] = (int *) calloc(height, sizeof(int));
					}

					clear(grid, width, height);
					clear(lastgrid, width, height);
					game = ARCADE;
				}
				centry = cellsize;
				wentry = width;
				hentry = height;
				t = 0;
			}
			break;
		default:
			printf("gamemode error\n");
			break;
		}

		//Display Step
		if(game == ARCADE || game == SETTINGS){
			SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00) );
		}
		rect.w = cellsize;
		rect.h = cellsize;
		rect.x = 0;
		for(int i = 0; i < width; i++){
			rect.y = 0;
			for(int j = 0; j < height; j++){
				//rect.x = i * cellsize;
				//rect.y = j * cellsize;
				if(grid[i][j] != lastgrid[i][j]){
					//printf("grid value: %d\n", grid[i][j]);
					lastgrid[i][j] = grid[i][j];
					SDL_BlitScaled(texture_img[grid[i][j]+1], NULL, screenSurface, &rect);
				}
				rect.y += cellsize;
			}
			rect.x += cellsize;
		}

		//Delay screen clear
		if(delay){
			SDL_UpdateWindowSurface( window );
			SDL_Delay(5000);
			clear(grid, width, height);
			clear(lastgrid, width, height);
			SDL_PumpEvents();
			SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
			game = ARCADE;
			t = 0;
			delay = 0;
		}

		if(game == ARCADE){ //Display main menu
			SDL_Rect img_rect;
			img_rect.x = fullwidth*0.325;
			img_rect.h = fullheight/((ROWS/2)+2);
			img_rect.y = fullheight/((ROWS/2)+2);
			img_rect.w = fullwidth*0.35;
			for(int i = 0; i < ROWS-2; i++){
				SDL_BlitScaled(texture_img[i+TITLE], NULL, screenSurface, &img_rect);
				img_rect.y = (2+(i/2))*img_rect.h;
				if(i == ROWS-4){
					img_rect.x = fullwidth*0.325;
				} else if(i % 2 == 0){
					img_rect.x = fullwidth*0.15;
				} else {
					img_rect.x = fullwidth*0.5;
				}
				//img_rect.w = fullwidth*0.7;
			}
			SDL_Rect arrow_rect;
			arrow_rect.x = fullwidth*0.1;
			arrow_rect.h = fullheight/((ROWS/2)+2);
			arrow_rect.y = 2*arrow_rect.h;
			arrow_rect.w = fullwidth*0.05;
			if(event.button.y / arrow_rect.h > 1 && event.button.y / arrow_rect.h < (ROWS/2)+1){
				arrow_rect.y = (event.button.y / arrow_rect.h) * arrow_rect.h;
			}
			if(event.button.x < fullwidth * 0.5){
				arrow_rect.x = fullwidth*0.1;
				SDL_BlitScaled(texture_img[arrow_img], NULL, screenSurface, &arrow_rect);
			} else if(event.button.y / arrow_rect.h < ROWS/2){
				arrow_rect.x = fullwidth*0.85;
				SDL_BlitScaled(texture_img[arrow_flip], NULL, screenSurface, &arrow_rect);
			}
		} else if(game == SETTINGS){ //Display settings menu
			SDL_Rect img_rect;
			img_rect.h = fullheight/SROWS;
			img_rect.w = fullwidth*0.7;
			img_rect.x = fullwidth*0.2;
			img_rect.y = fullheight/SROWS;
			SDL_BlitScaled(texture_img[settings_img], NULL, screenSurface, &img_rect);
			img_rect.w = fullwidth*0.4;
			img_rect.x = fullwidth*0.2;
			img_rect.y = 2*(fullheight/SROWS);
			SDL_BlitScaled(texture_img[width_img], NULL, screenSurface, &img_rect);
			img_rect.w = fullwidth*0.1;
			img_rect.x = fullwidth*0.6;
			img_rect.y = 2*(fullheight/SROWS);
			SDL_BlitScaled(texture_img[wentry/100 + 8], NULL, screenSurface, &img_rect);
			img_rect.x = fullwidth*0.7;
			img_rect.y = 2*(fullheight/SROWS);
			SDL_BlitScaled(texture_img[wentry/10 % 10 + 8], NULL, screenSurface, &img_rect);
			img_rect.x = fullwidth*0.8;
			img_rect.y = 2*(fullheight/SROWS);
			SDL_BlitScaled(texture_img[wentry % 10 + 8], NULL, screenSurface, &img_rect);
			img_rect.w = fullwidth*0.4;
			img_rect.x = fullwidth*0.2;
			img_rect.y = 3*(fullheight/SROWS);
			SDL_BlitScaled(texture_img[height_img], NULL, screenSurface, &img_rect);
			img_rect.w = fullwidth*0.1;
			img_rect.x = fullwidth*0.6;
			img_rect.y = 3*(fullheight/SROWS);
			SDL_BlitScaled(texture_img[hentry/100 + 8], NULL, screenSurface, &img_rect);
			img_rect.x = fullwidth*0.7;
			img_rect.y = 3*(fullheight/SROWS);
			SDL_BlitScaled(texture_img[hentry/10 % 10 + 8], NULL, screenSurface, &img_rect);
			img_rect.x = fullwidth*0.8;
			img_rect.y = 3*(fullheight/SROWS);
			SDL_BlitScaled(texture_img[hentry % 10 + 8], NULL, screenSurface, &img_rect);
			img_rect.w = fullwidth*0.4;
			img_rect.x = fullwidth*0.2;
			img_rect.y = 4*(fullheight/SROWS);
			SDL_BlitScaled(texture_img[cellsize_img], NULL, screenSurface, &img_rect);
			img_rect.w = fullwidth*0.1;
			img_rect.x = fullwidth*0.6;
			img_rect.y = 4*(fullheight/SROWS);
			SDL_BlitScaled(texture_img[centry/100 + 8], NULL, screenSurface, &img_rect);
			img_rect.x = fullwidth*0.7;
			img_rect.y = 4*(fullheight/SROWS);
			SDL_BlitScaled(texture_img[centry/10 % 10 + 8], NULL, screenSurface, &img_rect);
			img_rect.x = fullwidth*0.8;
			img_rect.y = 4*(fullheight/SROWS);
			SDL_BlitScaled(texture_img[centry % 10 + 8], NULL, screenSurface, &img_rect);
			img_rect.x = fullwidth*0.2;
			img_rect.w = fullwidth*0.7;
			img_rect.y = 6*(fullheight/SROWS);
			SDL_BlitScaled(texture_img[fullscreen_img], NULL, screenSurface, &img_rect);
			img_rect.y = 7*(fullheight/SROWS);
			SDL_BlitScaled(texture_img[confirm_img], NULL, screenSurface, &img_rect);
			img_rect.y = 8*(fullheight/SROWS);
			SDL_BlitScaled(texture_img[cancel_img], NULL, screenSurface, &img_rect);
			SDL_Rect arrow_rect;
			arrow_rect.x = fullwidth*0.1;
			arrow_rect.y = 2*(fullheight/SROWS);
			arrow_rect.h = fullheight/SROWS;
			arrow_rect.w = fullwidth*0.1;
			if(event.button.y / (fullheight/SROWS) > 1 && event.button.y / (fullheight/SROWS) < (SROWS-1) && event.button.y / (fullheight/SROWS) != 5){
				arrow_rect.y = (event.button.y / (fullheight/SROWS)) * (fullheight/SROWS);
			}
			SDL_BlitScaled(texture_img[arrow_img], NULL, screenSurface, &arrow_rect);
		}

		SDL_UpdateWindowSurface( window );

		//Delay Step
		if(game != ARCADE && game != SETTINGS && game != SETUP && game != CONNECT4 && game != CHECKERS && game != MINES && game != CHESS && game != BATTLE1 && game != BATTLE2 && game != SUDOKU && speed != 1){
			SDL_Delay( speed );
		}
	}

	//Free Dynamic memory
	for(int i = 0; i < width; i++){
		free(grid[i]);
		free(lastgrid[i]);
	}
	free(grid);
	free(lastgrid);

	for(int i = 0; i < TEXTURES; i++){
		SDL_FreeSurface(texture_img[i]);
	}
	free(texture_img);
	SDL_FreeSurface(screenSurface);

	SDL_DestroyWindow(window);

	SDL_Quit();

	return EXIT_SUCCESS;
}
