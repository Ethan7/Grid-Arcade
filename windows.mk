# Written 2/23/2020 #
CC=i686-w64-mingw32-gcc

CFLAGS=-I../SDL2/SDL2/i686-w64-mingw32/include -I../SDL2/SDL2/i686-w64-mingw32/include/SDL2 -I../SDL2/SDL2_image/i686-w64-mingw32/include -L../SDL2/SDL2/i686-w64-mingw32/lib -L../SDL2/SDL2_image/i686-w64-mingw32/lib -Wall -lm -lSDL2 -lSDL2_image -lSDL2main -g

MKDIR= mkdir -p

OUT_DIR= buildwin

objects := $(patsubst src/%.c,$(OUT_DIR)/%.o,$(wildcard src/*.c))

#link
all: $(OUT_DIR) $(objects)
	$(CC) $(objects) $(CFLAGS) -o arcade.exe

#compile
$(OUT_DIR)/%.o: src/%.c
	$(CC) -c $< $(CFLAGS) -o $@

#make output folder
$(OUT_DIR):
	$(MKDIR) $(OUT_DIR)

clean:
	rm -f *.o