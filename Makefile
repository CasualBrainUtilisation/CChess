output: main.o CCBoard.o CChessGame.o CCVisualizer.o CCFEN.o
	gcc main.o CCBoard.o CChessGame.o CCVisualizer.o CCFEN.o -o output
	
main.o: main.c
	gcc -c main.c

CCBoard.o: hf/CCBoard.c hf/CCBoard.h
	gcc -c hf/CCBoard.c

CChessGame.o: hf/CChessGame.c hf/CChessGame.h
	gcc -c hf/CChessGame.c

CCVisualizer.o: hf/CCVisualizer.c hf/CCVisualizer.h
	gcc -c hf/CCVisualizer.c

CCFEN.o: hf/CCFEN.c hf/CCFEN.h
	gcc -c hf/CCFEN.c

debug: main.o CCBoard.o CChessGame.o CCVisualizer.o CCFEN.o
	gcc main.o CCBoard.o CChessGame.o CCVisualizer.o CCFEN.o -o debug -g -ggdb

clean:
	rm *.o output