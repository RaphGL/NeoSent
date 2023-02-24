CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g 
LDFLAGS = -lm -lSDL2
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
