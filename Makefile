output: main.o CCBoard.o CCGeneralGameInfo.o CCVisualizer.o FEN.o
	gcc main.o CCBoard.o CCGeneralGameInfo.o CCVisualizer.o FEN.o -o output
	
main.o: main.c
	gcc -c main.c

CCBoard.o: hf/CCBoard.c hf/CCBoard.h
	gcc -c hf/CCBoard.c

CCGeneralGameInfo.o: hf/CCGeneralGameInfo.c hf/CCGeneralGameInfo.h
	gcc -c hf/CCGeneralGameInfo.c

CCVisualizer.o: hf/CCVisualizer.c hf/CCVisualizer.h
	gcc -c hf/CCVisualizer.c

FEN.o: hf/FEN.c hf/FEN.h
	gcc -c hf/FEN.c -

debug: main.o CCBoard.o CCGeneralGameInfo.o CCVisualizer.o FEN.o
	gcc main.o CCBoard.o CCGeneralGameInfo.o CCVisualizer.o FEN.o -o debug -g -ggdb

clean:
	rm *.o output