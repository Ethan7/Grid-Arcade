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

//Color defines
#define WHITE 0
#define RED 1
#define GREEN 2
#define BLUE 3
#define YELLOW 4
#define MAGENTA 5
#define CYAN 6

void clear(int **grid, int width, int height){
	for(int i = 0; i < width; i++){
		for(int j = 0; j < height; j++){
			grid[i][j] = -1;
		}
	}
}

int arcade(SDL_Event event, int game, int width, int height){
	if(event.type == SDL_MOUSEBUTTONUP){
		if(event.button.y / (height/9) == 2){
			return SNAKE;
		} else if(event.button.y / (height/9) == 3){
			return PATH;
		} else if(event.button.y / (height/9) == 4){
			return MAZES;
		} else if(event.button.y / (height/9) == 5){
			return PONG;
		} else if(event.button.y / (height/9) == 6){
			return TETRIS;
		} else if(event.button.y / (height/9) == 7){
			return SPACE;
		}
	}

	return ARCADE;
}

int snake(int **grid, SDL_Event event, int game, int t, int width, int height, int wrap);

int path(int **grid, SDL_Event event, int game, int t, int width, int height);

int mazes(int **grid, SDL_Event event, int game, int t, int width, int height);

int pong(int **grid, SDL_Event event, int game, int t, int width, int height);

int tetris(int **grid, SDL_Event event, int game, int t, int width, int height);

int space(int **grid, SDL_Event event, int game, int t, int width, int height);

int main(int argc, char **argv){
	int size = 5; //Grid cell size
	int width = 20; //Grid width
	int height = 20; //Grid height

	//Possible starting cases
	switch(argc){
		case 1:
			//Set Nothing
			break;
		case 2:
			size = atoi(argv[1]);
			break;
		case 4:
			size = atoi(argv[1]);
			width = atoi(argv[2]);
			height = atoi(argv[3]);
			//Ensure minimum screen size.
			if(width < 3){
				fprintf(stderr, "Invalid width\n");
				return EXIT_FAILURE;
			} else if(height < 3){
				fprintf(stderr, "Invalid height\n");
				return EXIT_FAILURE;
			}
			break;
		default:
			fprintf(stderr, "usage: ./arcade [log2(cellsize)] [width] [height]\n");
			return EXIT_FAILURE;
	}

	int cellsize = 1 << size;
	int fullwidth = width << size;
	int fullheight = height << size;

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

	//WHITE Surface
	SDL_Surface *white = SDL_CreateRGBSurface(0, cellsize, cellsize, 32, 0, 0, 0, 0);
	SDL_FillRect(white, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

	//RED Surface
	SDL_Surface *red = SDL_CreateRGBSurface(0, cellsize, cellsize, 32, 0, 0, 0, 0);
	SDL_FillRect(red, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0x00, 0x00));

	//GREEN Surface
	SDL_Surface *green = SDL_CreateRGBSurface(0, cellsize, cellsize, 32, 0, 0, 0, 0);
	SDL_FillRect(green, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0xFF, 0x00));

	//BLUE Surface
	SDL_Surface *blue = SDL_CreateRGBSurface(0, cellsize, cellsize, 32, 0, 0, 0, 0);
	SDL_FillRect(blue, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0xFF));

	//YELLOW Surface
	SDL_Surface *yellow = SDL_CreateRGBSurface(0, cellsize, cellsize, 32, 0, 0, 0, 0);
	SDL_FillRect(yellow, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0x00));

	//MAGENTA Surface
	SDL_Surface *magenta = SDL_CreateRGBSurface(0, cellsize, cellsize, 32, 0, 0, 0, 0);
	SDL_FillRect(magenta, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0x00, 0xFF));

	//CYAN Surface
	SDL_Surface *cyan = SDL_CreateRGBSurface(0, cellsize, cellsize, 32, 0, 0, 0, 0);
	SDL_FillRect(cyan, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0xFF, 0xFF));

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

	//Space Image
	SDL_Surface *space_img = IMG_Load("./img/soon.png");
	space_img = SDL_ConvertSurface( space_img, screenSurface->format, 0);

	//Arrow Image
	SDL_Surface *arrow_img = IMG_Load("./img/arrow.png");
	arrow_img = SDL_ConvertSurface( arrow_img, screenSurface->format, 0);

	//End SDL2 Stuff

	int **grid = (int **) calloc(width, sizeof(int *));
	for(int i = 0; i < width; i++){
		grid[i] = (int *) calloc(height, sizeof(int));
		for(int j = 0; j < height; j++){
			grid[i][j] = -1;
		}
	}

	int running = 1; //Whether game is running
	int t = 0; //Running time
	int paused = 0; //Whether the game is paused
	int speed = 256; //Speed for the gameplay loop
	int game = ARCADE; //Current game within the arcade

	//Gameplay Loop
	while(running){

		t++;

		//Input Step
		SDL_Event leftbutton;
		leftbutton.type = SDL_USEREVENT;
		SDL_Event direction;
		direction.type = SDL_USEREVENT;
		while(SDL_PollEvent( &event ) != 0 || paused){
			switch(event.type){
			case SDL_KEYDOWN:
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
			case SDL_MOUSEBUTTONUP:
				switch ( event.button.button ){
				case SDL_BUTTON_LEFT:
					leftbutton = event;
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
			if((game = arcade(leftbutton, game, fullwidth, fullheight)) != ARCADE){
				clear(grid, width, height);
				t = 0;
			}
			break;
		case SNAKE:
			if((game = snake(grid, direction, game, t, width, height, 0)) != SNAKE){
				clear(grid, width, height);
				t = 0;
			}
			break;
		case PATH:
			if((game = path(grid, event, game, t, width, height)) != PATH){
				clear(grid, width, height);
				t = 0;
			}
			break;
		case MAZES:
			if((game = mazes(grid, direction, game, t, width, height)) != MAZES){
				clear(grid, width, height);
				t = 0;
			}
			break;
		case PONG:
			if((game = pong(grid, direction, game, t, width, height)) != PONG){
				clear(grid, width, height);
				t = 0;
			}
			break;
		case TETRIS:
			if((game = tetris(grid, direction, game, t, width, height)) != TETRIS){
				clear(grid, width, height);
				t = 0;
			}
			break;
		case SPACE:
			if((game = space(grid, direction, game, t, width, height)) != SPACE){
				clear(grid, width, height);
				t = 0;
			}
			break;
		}

		//Display Step
		SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00) );
		for(int i = 0; i < width; i++){
			for(int j = 0; j < height; j++){
				rect.x = i << size;
				rect.y = j << size;
				switch(grid[i][j]){
				case WHITE:
					SDL_BlitSurface(white, NULL, screenSurface, &rect);
					break;
				case RED:
					SDL_BlitSurface(red, NULL, screenSurface, &rect);
					break;
				case GREEN:
					SDL_BlitSurface(green, NULL, screenSurface, &rect);
					break;
				case BLUE:
					SDL_BlitSurface(blue, NULL, screenSurface, &rect);
					break;
				case YELLOW:
					SDL_BlitSurface(yellow, NULL, screenSurface, &rect);
					break;
				case MAGENTA:
					SDL_BlitSurface(magenta, NULL, screenSurface, &rect);
					break;
				case CYAN:
					SDL_BlitSurface(cyan, NULL, screenSurface, &rect);
					break;
				default:
					break;
				}
			}
		}

		if(game == ARCADE){
			SDL_Rect img_rect;
			img_rect.x = fullwidth*0.2;
			img_rect.h = fullheight/9;
			img_rect.y = fullheight/9;
			img_rect.w = fullwidth*0.7;
			SDL_BlitScaled(title_img, NULL, screenSurface, &img_rect);
			img_rect.y = 2*(fullheight/9);
			img_rect.w = fullwidth*0.7;
			SDL_BlitScaled(snake_img, NULL, screenSurface, &img_rect);
			img_rect.y = 3*(fullheight/9);
			img_rect.w = fullwidth*0.7;
			SDL_BlitScaled(path_img, NULL, screenSurface, &img_rect);
			img_rect.y = 4*(fullheight/9);
			img_rect.w = fullwidth*0.7;
			SDL_BlitScaled(mazes_img, NULL, screenSurface, &img_rect);
			img_rect.y = 5*(fullheight/9);
			img_rect.w = fullwidth*0.7;
			SDL_BlitScaled(pong_img, NULL, screenSurface, &img_rect);
			img_rect.y = 6*(fullheight/9);
			img_rect.w = fullwidth*0.7;
			SDL_BlitScaled(tetris_img, NULL, screenSurface, &img_rect);
			img_rect.y = 7*(fullheight/9);
			img_rect.w = fullwidth*0.7;
			SDL_BlitScaled(space_img, NULL, screenSurface, &img_rect);
			SDL_Rect arrow_rect;
			arrow_rect.x = fullwidth*0.1;
			arrow_rect.y = 2*(fullheight/9);
			arrow_rect.h = fullheight/9;
			arrow_rect.w = fullwidth*0.1;
			if(event.button.y / (fullheight/9) > 1 && event.button.y / (fullheight/9) < 8){
				arrow_rect.y = (event.button.y / (fullheight/9)) * (fullheight/9);
			}
			SDL_BlitScaled(arrow_img, NULL, screenSurface, &arrow_rect);
		}

		SDL_UpdateWindowSurface( window );

		//Delay Step
		if(game != ARCADE){
			SDL_Delay( speed );
		}
	}

	//Free Dynamic memory
	for(int i = 0; i < width; i++){
		free(grid[i]);
	}
	free(grid);

	//End Program
	SDL_FreeSurface(white);
	SDL_FreeSurface(red);
	SDL_FreeSurface(green);
	SDL_FreeSurface(blue);
	SDL_FreeSurface(yellow);
	SDL_FreeSurface(magenta);
	SDL_FreeSurface(cyan);
	SDL_FreeSurface(screenSurface);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}
