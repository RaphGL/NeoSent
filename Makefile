CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g $(shell sdl2-config --cflags)
LDFLAGS = -lm $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf
# debug flags
DBFLAGS := -g -fsanitize=address -fsanitize=undefined
SRC = $(wildcard src/*.c) 
OBJ = $(patsubst src/%.c, %.o, $(SRC))

.PHONY: all debug clean

all: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -O2 $^

debug: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(DBFLAGS) $^

clean:
	rm -f *.o a.out

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $?
