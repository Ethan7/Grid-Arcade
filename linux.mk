# Written 2/23/2020 #
CC=gcc

CFLAGS=-Wall -lm -lSDL2 -lSDL2_image -lSDL2main -O3

MKDIR= mkdir -p

OUT_DIR= buildlin

objects := $(patsubst src/%.c,$(OUT_DIR)/%.o,$(wildcard src/*.c))

#link
all: $(OUT_DIR) $(objects)
	$(CC) $(objects) $(CFLAGS) -o arcade

#compile
$(OUT_DIR)/%.o: src/%.c
	$(CC) -c $< $(CFLAGS) -o $@

#make output folder
$(OUT_DIR):
	$(MKDIR) $(OUT_DIR)

clean:
	rm -f *.o
