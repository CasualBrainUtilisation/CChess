#include <stdio.h>
#include "hf/CCBoard.h"
#include "hf/CCFEN.h"
#include "hf/CChessGame.h"
#include "hf/CCVisualizer.h"


int main()
{
    ChessGame *chessGame = GetGameFromFEN("rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq - 0 1");
    if(chessGame == NULL) printf("now i have to cry \n");

    PrintBoard(chessGame->board);

    Pos pos = {3, 7};
    Piece *piece = GetPieceAtPos(pos, chessGame->board);

    Move *moves = GetAllMovesForPiece(piece, chessGame->board);

    if (moves == NULL) printf("There ain't moves lol");

    PerformMove(moves[0], chessGame->board);

    PrintBoard(chessGame->board);

    

    DeleteChessBoard(&chessGame->board);

    return 0;
}








