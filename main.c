#include <stdio.h>
#include "hf/CCFEN.h"
#include "hf/CChessGame.h"
#include "hf/CCVisualizer.h"
#include "hf/CCMoves.h"


int main()
{
    ChessGame *chessGame = GetGameFromFEN("r3krrr/pppppppp/8/8/8/8/PPPPPPPP/RRRRKBNR w KQkq - 0 1");
    if(chessGame == NULL) printf("now i have to cry \n");

    PrintBoard(chessGame->board);
    printf("\n");

    Pos pos = {4, 7};
    Piece *piece = GetPieceAtPos(pos, chessGame->board);

    MoveDataLinkedList *moves = GetAllMovesForPiece(piece, chessGame);

    if (moves == NULL) printf("There ain't moves lol");

    PerformMove(&moves->Head->Next->data, chessGame);

    PrintBoard(chessGame->board);

    

    DeleteChessBoard(&chessGame->board);

    return 0;
}