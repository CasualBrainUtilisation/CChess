#include <stdio.h>
#include "hf/CCBoard.h"
#include "hf/CCFEN.h"
#include "hf/CChessGame.h"
#include "hf/CCVisualizer.h"


int main()
{
    ChessGame *chessGame = GetGameFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    if(chessGame == NULL) printf("now i have to cry \n");

    PrintBoard(chessGame->board);

    Pos pos = {7, 6};
    Piece *piece = GetPieceAtPos(pos, chessGame->board);

    Move *moves = GetAllMovesForPiece(piece, chessGame->board);
    PerformMove(moves[0], chessGame->board);

    PrintBoard(chessGame->board);

    

    DeleteChessBoard(&chessGame->board);

    return 0;
}








