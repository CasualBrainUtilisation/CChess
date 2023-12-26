output: main.o CCBoard.o CCGeneralGameInfo.o CCMoves.o CCPieces.o FEN.o
	gcc main.o CCBoard.o CCGeneralGameInfo.o CCMoves.o CCPieces.o FEN.o -o output
	
main.o: main.c
	gcc -c main.c

CCBoard.o: hf/CCBoard.c hf/CCBoard.h:
	gcc -c CCBoard.o

CCGeneralGameInfo.o: hf/CCGeneralGameInfo.c hf/CCGeneralGameInfo.h:
	gcc -c CCGeneralGameInfo.o

CCMoves.o: hf/CCMoves.c hf/CCMoves.h:
	gcc -c CCMoves.o

CCPieces.o: hf/CCPieces.c hf/CCPieces.h:
	gcc -c CCPieces.o

FEN.o: hf/FEN.c hf/FEN.h:
	gcc -c FEN.o
