/* By Ethan Hughs */
/* Written 12/1/2018 */

#define SDL_MAIN_HANDLED
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<stdio.h>
#include<stdlib.h>

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
#define SETUP 13
#define MINES 14
#define SETTINGS 15
#define SETTINGS2 16
#define SETTINGS3 17

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
#define ONE 7
#define TWO 8
#define THREE 9
#define FOUR 10
#define FIVE 11
#define SIX 12
#define SEVEN 13
#define EIGHT 14

//Menu defines
#define ROWS 17
#define SROWS 10

void clear(int **grid, int width, int height){
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			grid[i][j] = -1;
		}
	}
}

int arcade(SDL_Event event, int game, int width, int height, int *setupgame){
	if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT){
		switch(event.button.y / (height/ROWS)){
		case 2:
			return SNAKE;
		case 3:
			*setupgame = PATH;
			return SETUP;
		case 4:
			return MAZES;
		case 5:
			return PONG;
		case 6:
			return TETRIS;
		case 7:
			*setupgame = CONWAY;
			return SETUP;
		case 8:
			return SPACE;
		case 9:
			return FROGGER;
		case 10:
			*setupgame = LANGSTON;
			return SETUP;
		case 11:
			return CONNECT4;
		case 12:
			return FLAPPY;
		case 13:
			return CHECKERS;
		case 14:
			return MINES;
		case 15:
			return SETTINGS;
		default:
			break;
		}
	}

	return ARCADE;
}

int settings(SDL_Event event, int game, int height, int *placemarker, int *wentry, int *hentry, int *centry){
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
			if(*wentry < 8){
				*wentry = 8;
			}
			if(*hentry < 8){
				*hentry = 8;
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

int snake(int **grid, SDL_Event event, int game, int t, int width, int height, int wrap);

int path(int **grid, SDL_Event event, int game, int t, int width, int height);

int mazes(int **grid, SDL_Event event, int game, int t, int width, int height);

int pong(int **grid, SDL_Event event, int game, int t, int width, int height);

int tetris(int **grid, SDL_Event event, int game, int t, int width, int height);

int conway(int **grid, int game, int t, int width, int height);

int langston(int **grid, int game, int t, int width, int height);

int space(int **grid, SDL_Event event, int game, int t, int width, int height);

int frogger(int **grid, SDL_Event event, int game, int t, int width, int height);

int connect4(int **grid, SDL_Event event, int game, int t, int width, int height);

int flappy(int **grid, SDL_Event event, int game, int t, int width, int height);

int checkers(int **grid, SDL_Event eventbutton, int game, int t, int cellsize, int width, int height);

int mines(int **grid, SDL_Event eventbutton, int game, int t, int cellsize, int width, int height);

int setup(int **grid, SDL_Event eventbutton, int setupgame, int cellsize);

int main(int argc, char **argv){
	int cellsize = 32; //Grid cell size
	int width = 20; //Grid width
	int height = 20; //Grid height

	//Possible starting cases
	switch(argc){
		case 1:
			//Set Nothing
			break;
		case 2:
			cellsize = atoi(argv[1]);
			break;
		case 4:
			cellsize = atoi(argv[1]);
			width = atoi(argv[2]);
			height = atoi(argv[3]);
			//Ensure minimum screen size.
			if(width < 8){
				fprintf(stderr, "Invalid width needs to be > 7\n");
				return EXIT_FAILURE;
			} else if(height < 8){
				fprintf(stderr, "Invalid height needs to be > 7\n");
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

	//BLACK Surface
	SDL_Surface *black = SDL_CreateRGBSurface(0, 8, 8, 32, 0, 0, 0, 0);
	SDL_FillRect(black, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));

	//WHITE Surface
	SDL_Surface *white = SDL_CreateRGBSurface(0, 8, 8, 32, 0, 0, 0, 0);
	SDL_FillRect(white, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

	//RED Surface
	SDL_Surface *red = SDL_CreateRGBSurface(0, 8, 8, 32, 0, 0, 0, 0);
	SDL_FillRect(red, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0x00, 0x00));

	//GREEN Surface
	SDL_Surface *green = SDL_CreateRGBSurface(0, 8, 8, 32, 0, 0, 0, 0);
	SDL_FillRect(green, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0xFF, 0x00));

	//BLUE Surface
	SDL_Surface *blue = SDL_CreateRGBSurface(0, 8, 8, 32, 0, 0, 0, 0);
	SDL_FillRect(blue, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0xFF));

	//YELLOW Surface
	SDL_Surface *yellow = SDL_CreateRGBSurface(0, 8, 8, 32, 0, 0, 0, 0);
	SDL_FillRect(yellow, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0x00));

	//MAGENTA Surface
	SDL_Surface *magenta = SDL_CreateRGBSurface(0, 8, 8, 32, 0, 0, 0, 0);
	SDL_FillRect(magenta, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0x00, 0xFF));

	//CYAN Surface
	SDL_Surface *cyan = SDL_CreateRGBSurface(0, 8, 8, 32, 0, 0, 0, 0);
	SDL_FillRect(cyan, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0xFF, 0xFF));

	//Number Surface Array
	SDL_Surface **numbers_img = (SDL_Surface **) calloc(10, sizeof(SDL_Surface *));
	
	//Zero Surface
	numbers_img[0] = IMG_Load("./img/zero.png");
	numbers_img[0] = SDL_ConvertSurface( numbers_img[0], screenSurface->format, 0);

	//One Surface
	numbers_img[1] = IMG_Load("./img/one.png");
	numbers_img[1] = SDL_ConvertSurface( numbers_img[1], screenSurface->format, 0);

	//Two Surface
	numbers_img[2] = IMG_Load("./img/two.png");
	numbers_img[2] = SDL_ConvertSurface( numbers_img[2], screenSurface->format, 0);

	//Three Surface
	numbers_img[3] = IMG_Load("./img/three.png");
	numbers_img[3] = SDL_ConvertSurface( numbers_img[3], screenSurface->format, 0);

	//Four Surface
	numbers_img[4] = IMG_Load("./img/four.png");
	numbers_img[4] = SDL_ConvertSurface( numbers_img[4], screenSurface->format, 0);

	//Five Surface
	numbers_img[5] = IMG_Load("./img/five.png");
	numbers_img[5] = SDL_ConvertSurface( numbers_img[5], screenSurface->format, 0);

	//Six Surface
	numbers_img[6] = IMG_Load("./img/six.png");
	numbers_img[6] = SDL_ConvertSurface( numbers_img[6], screenSurface->format, 0);

	//Seven Surface
	numbers_img[7] = IMG_Load("./img/seven.png");
	numbers_img[7] = SDL_ConvertSurface( numbers_img[7], screenSurface->format, 0);

	//Eight Surface
	numbers_img[8] = IMG_Load("./img/eight.png");
	numbers_img[8] = SDL_ConvertSurface( numbers_img[8], screenSurface->format, 0);

	//Nine Surface
	numbers_img[9] = IMG_Load("./img/nine.png");
	numbers_img[9] = SDL_ConvertSurface( numbers_img[9], screenSurface->format, 0);

	//Title Image
	SDL_Surface *title_img = IMG_Load("./img/title.png");
	title_img = SDL_ConvertSurface( title_img, screenSurface->format, 0);

	//Mazes Image
	SDL_Surface *mazes_img = IMG_Load("./img/mazes.png");
	mazes_img = SDL_ConvertSurface( mazes_img, screenSurface->format, 0);

	//Path Image
	SDL_Surface *path_img = IMG_Load("./img/path.png");
	path_img = SDL_ConvertSurface( path_img, screenSurface->format, 0);

	//Pong Image
	SDL_Surface *pong_img = IMG_Load("./img/pong.png");
	pong_img = SDL_ConvertSurface( pong_img, screenSurface->format, 0);

	//Snake Image
	SDL_Surface *snake_img = IMG_Load("./img/snake.png");
	snake_img = SDL_ConvertSurface( snake_img, screenSurface->format, 0);

	//Tetris Image
	SDL_Surface *tetris_img = IMG_Load("./img/tetris.png");
	tetris_img = SDL_ConvertSurface( tetris_img, screenSurface->format, 0);

	//Conway Image
	SDL_Surface *conway_img = IMG_Load("./img/conway.png");
	conway_img = SDL_ConvertSurface( conway_img, screenSurface->format, 0);

	//Space Image
	SDL_Surface *space_img = IMG_Load("./img/space.png");
	space_img = SDL_ConvertSurface( space_img, screenSurface->format, 0);

	//Frogger Image
	SDL_Surface *frogger_img = IMG_Load("./img/frogger.png");
	frogger_img = SDL_ConvertSurface( frogger_img, screenSurface->format, 0);

	//Langston Image
	SDL_Surface *langston_img = IMG_Load("./img/langston.png");
	langston_img = SDL_ConvertSurface( langston_img, screenSurface->format, 0);

	//Connect4 Image
	SDL_Surface *connect4_img = IMG_Load("./img/connect4.png");
	connect4_img = SDL_ConvertSurface( connect4_img, screenSurface->format, 0);

	//Flappy Image
	SDL_Surface *flappy_img = IMG_Load("./img/flappy.png");
	flappy_img = SDL_ConvertSurface( flappy_img, screenSurface->format, 0);

	//Checkers Image
	SDL_Surface *checkers_img = IMG_Load("./img/checkers.png");
	checkers_img = SDL_ConvertSurface( checkers_img, screenSurface->format, 0);

	//Mines Image
	SDL_Surface *mines_img = IMG_Load("./img/mines.png");
	mines_img = SDL_ConvertSurface( mines_img, screenSurface->format, 0);

	//Settings Image
	SDL_Surface *settings_img = IMG_Load("./img/settings.png");
	settings_img = SDL_ConvertSurface( settings_img, screenSurface->format, 0);

	//Fullscreen Image
	SDL_Surface *fullscreen_img = IMG_Load("./img/fullscreen.png");
	fullscreen_img = SDL_ConvertSurface( fullscreen_img, screenSurface->format, 0);

	//Confirm Image
	SDL_Surface *confirm_img = IMG_Load("./img/confirm.png");
	confirm_img = SDL_ConvertSurface( confirm_img, screenSurface->format, 0);

	//Cancel Image
	SDL_Surface *cancel_img = IMG_Load("./img/cancel.png");
	cancel_img = SDL_ConvertSurface( cancel_img, screenSurface->format, 0);

	//Width Image
	SDL_Surface *width_img = IMG_Load("./img/width.png");
	width_img = SDL_ConvertSurface( width_img, screenSurface->format, 0);

	//Height Image
	SDL_Surface *height_img = IMG_Load("./img/height.png");
	height_img = SDL_ConvertSurface( height_img, screenSurface->format, 0);

	//Cellsize Image
	SDL_Surface *cellsize_img = IMG_Load("./img/cellsize.png");
	cellsize_img = SDL_ConvertSurface( cellsize_img, screenSurface->format, 0);

	//Arrow Image
	SDL_Surface *arrow_img = IMG_Load("./img/arrow.png");
	arrow_img = SDL_ConvertSurface( arrow_img, screenSurface->format, 0);
	
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
	int game = ARCADE; //Current game within the arcade
	int setupgame = CONWAY; //Game which is currently being setup
	int wentry = width; //Width setting
	int hentry = height; //Height setting
	int centry = cellsize; //Cellsize setting
	int placemarker = 0; //Placemarker for which digit of which setting you're changing
	SDL_Event buttonheld;
	buttonheld.type = SDL_USEREVENT;

	//Gameplay Loop
	while(running){

		t++;

		//Input Step
		SDL_Event leftrightbutton;
		leftrightbutton.type = SDL_USEREVENT;
		SDL_Event direction;
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
					if(paused){
						paused = 0;
					} else {
						paused = 1;
					}
					break;
				case SDLK_RETURN:
					buttonheld = event;
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
			if((game = arcade(leftrightbutton, game, fullwidth, fullheight, &setupgame)) != ARCADE){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00) );
				t = 0;
			}
			break;
		case SNAKE:
			if((game = snake(grid, direction, game, t, width, height, 0)) != SNAKE){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case PATH:
			if((game = path(grid, event, game, t, width, height)) != PATH){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case MAZES:
			if((game = mazes(grid, direction, game, t, width, height)) != MAZES){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case PONG:
			if((game = pong(grid, direction, game, t, width, height)) != PONG){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case TETRIS:
			if((game = tetris(grid, direction, game, t, width, height)) != TETRIS){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case CONWAY:
			if((game = conway(grid, game, t, width, height)) != CONWAY){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case SPACE:
			if((game = space(grid, direction, game, t, width, height)) != SPACE){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case FROGGER:
			if((game = frogger(grid, direction, game, t, width, height)) != FROGGER){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case LANGSTON:
			if((game = langston(grid, game, t, width, height)) != LANGSTON){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case CONNECT4:
			if((game = connect4(grid, direction, game, t, width, height)) != CONNECT4){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case FLAPPY:
			if((game = flappy(grid, direction, game, t, width, height)) != FLAPPY){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case CHECKERS:
			if((game = checkers(grid, leftrightbutton, game, t, cellsize, width, height)) != CHECKERS){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case MINES:
			if((game = mines(grid, leftrightbutton, game, t, cellsize, width, height)) != MINES){
				clear(grid, width, height);
				clear(lastgrid, width, height);
				t = 0;
			}
			break;
		case SETUP:
			if((game = setup(grid, buttonheld, setupgame, cellsize)) != SETUP){
				t = 0;
			}
			break;
		case SETTINGS:
			if((game = settings(leftrightbutton, game, fullheight, &placemarker, &wentry, &hentry, &centry)) != SETTINGS){
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
					lastgrid[i][j] = grid[i][j];
					switch(grid[i][j]){
					case BLACK:
						SDL_BlitScaled(black, NULL, screenSurface, &rect);
						break;
					case WHITE:
						SDL_BlitScaled(white, NULL, screenSurface, &rect);
						break;
					case RED:
						SDL_BlitScaled(red, NULL, screenSurface, &rect);
						break;
					case GREEN:
						SDL_BlitScaled(green, NULL, screenSurface, &rect);
						break;
					case BLUE:
						SDL_BlitScaled(blue, NULL, screenSurface, &rect);
						break;
					case YELLOW:
						SDL_BlitScaled(yellow, NULL, screenSurface, &rect);
						break;
					case MAGENTA:
						SDL_BlitScaled(magenta, NULL, screenSurface, &rect);
						break;
					case CYAN:
						SDL_BlitScaled(cyan, NULL, screenSurface, &rect);
						break;
					default:
						//Blit numbers for Minesweeper
						if(grid[i][j] != -1){
							SDL_BlitScaled(numbers_img[grid[i][j]-6], NULL, screenSurface, &rect);
						}
						break;
					}
				}
				rect.y += cellsize;
			}
			rect.x += cellsize;
		}

		if(game == ARCADE){ //Display main menu
			SDL_Rect img_rect;
			img_rect.x = fullwidth*0.2;
			img_rect.h = fullheight/ROWS;
			img_rect.y = fullheight/ROWS;
			img_rect.w = fullwidth*0.7;
			SDL_BlitScaled(title_img, NULL, screenSurface, &img_rect);
			img_rect.y = 2*(fullheight/ROWS);
			img_rect.w = fullwidth*0.7;
			SDL_BlitScaled(snake_img, NULL, screenSurface, &img_rect);
			img_rect.y = 3*(fullheight/ROWS);
			img_rect.w = fullwidth*0.7;
			SDL_BlitScaled(path_img, NULL, screenSurface, &img_rect);
			img_rect.y = 4*(fullheight/ROWS);
			img_rect.w = fullwidth*0.7;
			SDL_BlitScaled(mazes_img, NULL, screenSurface, &img_rect);
			img_rect.y = 5*(fullheight/ROWS);
			img_rect.w = fullwidth*0.7;
			SDL_BlitScaled(pong_img, NULL, screenSurface, &img_rect);
			img_rect.y = 6*(fullheight/ROWS);
			img_rect.w = fullwidth*0.7;
			SDL_BlitScaled(tetris_img, NULL, screenSurface, &img_rect);
			img_rect.y = 7*(fullheight/ROWS);
			img_rect.w = fullwidth*0.7;
			SDL_BlitScaled(conway_img, NULL, screenSurface, &img_rect);
			img_rect.y = 8*(fullheight/ROWS);
			img_rect.w = fullwidth*0.7;
			SDL_BlitScaled(space_img, NULL, screenSurface, &img_rect);
			img_rect.y = 9*(fullheight/ROWS);
			img_rect.w = fullwidth*0.7;
			SDL_BlitScaled(frogger_img, NULL, screenSurface, &img_rect);
			img_rect.y = 10*(fullheight/ROWS);
			img_rect.w = fullwidth*0.7;
			SDL_BlitScaled(langston_img, NULL, screenSurface, &img_rect);
			img_rect.y = 11*(fullheight/ROWS);
			img_rect.w = fullwidth*0.7;
			SDL_BlitScaled(connect4_img, NULL, screenSurface, &img_rect);
			img_rect.y = 12*(fullheight/ROWS);
			img_rect.w = fullwidth*0.7;
			SDL_BlitScaled(flappy_img, NULL, screenSurface, &img_rect);
			img_rect.y = 13*(fullheight/ROWS);
			img_rect.w = fullwidth*0.7;
			SDL_BlitScaled(checkers_img, NULL, screenSurface, &img_rect);
			img_rect.y = 14*(fullheight/ROWS);
			img_rect.w = fullwidth*0.7;
			SDL_BlitScaled(mines_img, NULL, screenSurface, &img_rect);
			img_rect.y = 15*(fullheight/ROWS);
			img_rect.w = fullwidth*0.7;
			SDL_BlitScaled(settings_img, NULL, screenSurface, &img_rect);
			SDL_Rect arrow_rect;
			arrow_rect.x = fullwidth*0.1;
			arrow_rect.y = 2*(fullheight/ROWS);
			arrow_rect.h = fullheight/ROWS;
			arrow_rect.w = fullwidth*0.1;
			if(event.button.y / (fullheight/ROWS) > 1 && event.button.y / (fullheight/ROWS) < (ROWS-1)){
				arrow_rect.y = (event.button.y / (fullheight/ROWS)) * (fullheight/ROWS);
			}
			SDL_BlitScaled(arrow_img, NULL, screenSurface, &arrow_rect);
		} else if(game == SETTINGS){ //Display settings menu
			SDL_Rect img_rect;
			img_rect.h = fullheight/SROWS;
			img_rect.w = fullwidth*0.7;
			img_rect.x = fullwidth*0.2;
			img_rect.y = fullheight/SROWS;
			SDL_BlitScaled(settings_img, NULL, screenSurface, &img_rect);
			img_rect.w = fullwidth*0.4;
			img_rect.x = fullwidth*0.2;
			img_rect.y = 2*(fullheight/SROWS);
			SDL_BlitScaled(width_img, NULL, screenSurface, &img_rect);
			img_rect.w = fullwidth*0.1;
			img_rect.x = fullwidth*0.6;
			img_rect.y = 2*(fullheight/SROWS);
			SDL_BlitScaled(numbers_img[wentry/100], NULL, screenSurface, &img_rect);
			img_rect.x = fullwidth*0.7;
			img_rect.y = 2*(fullheight/SROWS);
			SDL_BlitScaled(numbers_img[wentry/10 % 10], NULL, screenSurface, &img_rect);
			img_rect.x = fullwidth*0.8;
			img_rect.y = 2*(fullheight/SROWS);
			SDL_BlitScaled(numbers_img[wentry % 10], NULL, screenSurface, &img_rect);
			img_rect.w = fullwidth*0.4;
			img_rect.x = fullwidth*0.2;
			img_rect.y = 3*(fullheight/SROWS);
			SDL_BlitScaled(height_img, NULL, screenSurface, &img_rect);
			img_rect.w = fullwidth*0.1;
			img_rect.x = fullwidth*0.6;
			img_rect.y = 3*(fullheight/SROWS);
			SDL_BlitScaled(numbers_img[hentry/100], NULL, screenSurface, &img_rect);
			img_rect.x = fullwidth*0.7;
			img_rect.y = 3*(fullheight/SROWS);
			SDL_BlitScaled(numbers_img[hentry/10 % 10], NULL, screenSurface, &img_rect);
			img_rect.x = fullwidth*0.8;
			img_rect.y = 3*(fullheight/SROWS);
			SDL_BlitScaled(numbers_img[hentry % 10], NULL, screenSurface, &img_rect);
			img_rect.w = fullwidth*0.4;
			img_rect.x = fullwidth*0.2;
			img_rect.y = 4*(fullheight/SROWS);
			SDL_BlitScaled(cellsize_img, NULL, screenSurface, &img_rect);
			img_rect.w = fullwidth*0.1;
			img_rect.x = fullwidth*0.6;
			img_rect.y = 4*(fullheight/SROWS);
			SDL_BlitScaled(numbers_img[centry/100], NULL, screenSurface, &img_rect);
			img_rect.x = fullwidth*0.7;
			img_rect.y = 4*(fullheight/SROWS);
			SDL_BlitScaled(numbers_img[centry/10 % 10], NULL, screenSurface, &img_rect);
			img_rect.x = fullwidth*0.8;
			img_rect.y = 4*(fullheight/SROWS);
			SDL_BlitScaled(numbers_img[centry % 10], NULL, screenSurface, &img_rect);
			img_rect.x = fullwidth*0.2;
			img_rect.w = fullwidth*0.7;
			img_rect.y = 6*(fullheight/SROWS);
			SDL_BlitScaled(fullscreen_img, NULL, screenSurface, &img_rect);
			img_rect.y = 7*(fullheight/SROWS);
			SDL_BlitScaled(confirm_img, NULL, screenSurface, &img_rect);
			img_rect.y = 8*(fullheight/SROWS);
			SDL_BlitScaled(cancel_img, NULL, screenSurface, &img_rect);
			SDL_Rect arrow_rect;
			arrow_rect.x = fullwidth*0.1;
			arrow_rect.y = 2*(fullheight/SROWS);
			arrow_rect.h = fullheight/SROWS;
			arrow_rect.w = fullwidth*0.1;
			if(event.button.y / (fullheight/SROWS) > 1 && event.button.y / (fullheight/SROWS) < (SROWS-1) && event.button.y / (fullheight/SROWS) != 5){
				arrow_rect.y = (event.button.y / (fullheight/SROWS)) * (fullheight/SROWS);
			}
			SDL_BlitScaled(arrow_img, NULL, screenSurface, &arrow_rect);
		}

		SDL_UpdateWindowSurface( window );

		//Delay Step
		if(game != ARCADE && game != SETTINGS && game != SETUP && game != CONNECT4 && game != CHECKERS && speed != 1){
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
	SDL_FreeSurface(black);
	SDL_FreeSurface(white);
	SDL_FreeSurface(red);
	SDL_FreeSurface(green);
	SDL_FreeSurface(blue);
	SDL_FreeSurface(yellow);
	SDL_FreeSurface(magenta);
	SDL_FreeSurface(cyan);
	for(int i = 0; i < 10; i++){
		SDL_FreeSurface(numbers_img[i]);
	}
	free(numbers_img);
	SDL_FreeSurface(title_img);
	SDL_FreeSurface(path_img);
	SDL_FreeSurface(tetris_img);
	SDL_FreeSurface(mazes_img);
	SDL_FreeSurface(pong_img);
	SDL_FreeSurface(snake_img);
	SDL_FreeSurface(conway_img);
	SDL_FreeSurface(space_img);
	SDL_FreeSurface(frogger_img);
	SDL_FreeSurface(langston_img);
	SDL_FreeSurface(connect4_img);
	SDL_FreeSurface(flappy_img);
	SDL_FreeSurface(checkers_img);
	SDL_FreeSurface(mines_img);
	SDL_FreeSurface(settings_img);
	SDL_FreeSurface(fullscreen_img);
	SDL_FreeSurface(confirm_img);
	SDL_FreeSurface(cancel_img);
	SDL_FreeSurface(width_img);
	SDL_FreeSurface(height_img);
	SDL_FreeSurface(cellsize_img);
	SDL_FreeSurface(arrow_img);
	SDL_FreeSurface(screenSurface);

	SDL_DestroyWindow(window);

	SDL_Quit();

	return EXIT_SUCCESS;
}