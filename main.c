#include <stdio.h>
#include "hf/CCFEN.h"
#include "hf/CChessGame.h"
#include "hf/CCVisualizer.h"
#include "hf/CCMoves.h"


int main()
{
    ChessGame *chessGame = GetGameFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq a6 0 1");
    if(chessGame == NULL) printf("now i have to cry \n");

    PrintBoard(chessGame->board);
    printf("\n");
    
    Pos pos = {0, 6};
    Piece *piece = GetPieceAtPos(pos, chessGame->board);

    MoveDataLinkedList *moves = GetAllMovesForPiece(piece, chessGame);

    if (moves == NULL) printf("There ain't moves lol");

    PerformMove(&moves->Head->Next->data, chessGame);

    PrintBoard(chessGame->board);

    char FEN[128];
    ConvertGameToFEN(FEN, chessGame);

    printf("%s", FEN);

    

    DeleteChessBoard(&chessGame->board);

    return 0;
}