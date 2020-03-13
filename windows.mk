# Written 2/23/2020 #
CC=i686-w64-mingw32-gcc

CFLAGS=-I../SDL2/SDL2/i686-w64-mingw32/include -I../SDL2/SDL2/i686-w64-mingw32/include/SDL2 -I../SDL2/SDL2_image/i686-w64-mingw32/include -L../SDL2/SDL2/i686-w64-mingw32/lib -L../SDL2/SDL2_image/i686-w64-mingw32/lib -Wall -lm -lSDL2 -lSDL2_image -lSDL2main -g

all: arcade.o mazes.o snake.o path.o pong.o tetris.o conway.o space.o frogger.o langston.o connect4.o flappy.o checkers.o mines.o chess.o setup.o
	$(CC) arcade.o mazes.o snake.o path.o pong.o tetris.o conway.o space.o frogger.o langston.o connect4.o flappy.o checkers.o mines.o chess.o setup.o $(CFLAGS) -o arcade.exe

#May just want to compile in one step in the future, as all my projects are small in size
arcade.o: src/arcade.c
	$(CC) -c src/arcade.c $(CFLAGS)
mazes.o: src/mazes.c
	$(CC) -c src/mazes.c $(CFLAGS)
snake.o: src/snake.c
	$(CC) -c src/snake.c $(CFLAGS)
path.o: src/path.c
	$(CC) -c src/path.c $(CFLAGS)
pong.o: src/pong.c
	$(CC) -c src/pong.c $(CFLAGS)
tetris.o: src/tetris.c
	$(CC) -c src/tetris.c $(CFLAGS)
conway.o: src/conway.c
	$(CC) -c src/conway.c $(CFLAGS)
space.o: src/space.c
	$(CC) -c src/space.c $(CFLAGS)
frogger.o: src/frogger.c
	$(CC) -c src/frogger.c $(CFLAGS)
langston.o: src/langston.c
	$(CC) -c src/langston.c $(CFLAGS)
connect4.o: src/connect4.c
	$(CC) -c src/connect4.c $(CFLAGS)
flappy.o: src/flappy.c
	$(CC) -c src/flappy.c $(CFLAGS)
checkers.o: src/checkers.c
	$(CC) -c src/checkers.c $(CFLAGS)
mines.o: src/mines.c
	$(CC) -c src/mines.c $(CFLAGS)
chess.o: src/chess.c
	$(CC) -c src/chess.c $(CFLAGS)
setup.o: src/setup.c
	$(CC) -c src/setup.c $(CFLAGS)

clean:
	rm -f *.o