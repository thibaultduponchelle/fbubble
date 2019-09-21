all: frosted-bubble

frosted-bubble: main.o bubble.o state.o board.o ui.o
	gcc -g -o frosted-bubble *.o -lSDL -lSDL_image -lSDL_ttf -lSDL_gfx

main.o: main.c
	gcc -g -c main.c

bubble.o: bubble.c bubble.h
	gcc -g -c bubble.c

state.o: state.c state.h
	gcc -g -c state.c

board.o: board.c board.h
	gcc -g -c board.c

ui.o: ui.c ui.h
	gcc -g -c ui.c

clean: 
	rm -f *.o frosted-bubble

.PHONY: all clean
