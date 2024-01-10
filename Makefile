output: main.o CCBoard.o CCFEN.o CChessGame.o CCMoves.o CCSquares.o CCVisualizer.o
	gcc main.o CCBoard.o CCFEN.o CChessGame.o CCMoves.o CCSquares.o CCVisualizer.o -o output
	
	
main.o: main.c
	gcc -c main.c

CCBoard.o: hf/CCBoard.c hf/CCBoard.h
	gcc -c hf/CCBoard.c

CCFEN.o: hf/CCFEN.c hf/CCFEN.h
	gcc -c hf/CCFEN.c

CChessGame.o: hf/CChessGame.c hf/CChessGame.h
	gcc -c hf/CChessGame.c

CCMoves.o: hf/CCMoves.c hf/CCMoves.h
	gcc -c hf/CCMoves.c

CCSquares.o: hf/CCSquares.c hf/CCSquares.h
	gcc -c hf/CCSquares.c

CCVisualizer.o: hf/CCVisualizer.c hf/CCVisualizer.h
	gcc -c hf/CCVisualizer.c


debug:  main.o CCBoard.o CCFEN.o CChessGame.o CCMoves.o CCSquares.o CCVisualizer.o
	gcc main.o CCBoard.o CCFEN.o CChessGame.o CCMoves.o CCSquares.o CCVisualizer.o -o debug -g -ggdb

clean:
	rm *.o output