#include <stdio.h>
#include "hf/CCBoard.h"
#include "hf/CCFEN.h"
#include "hf/CChessGame.h"
#include "hf/CCVisualizer.h"


int main()
{
    ChessGame *chessGame = GetGameFromFEN("rnbqkbnr/1p1p1ppp/8/pPp1p3/4P3/8/P1PP1PPP/RNBQKBNR w KQkq a6 0 4");
    if(chessGame == NULL) printf("now i have to cry \n");

    PrintBoard(chessGame->board);

    printf("%d \n", chessGame->currentTurn);

    printf("%d \n", chessGame->gameCastlingRights.whiteCastlingRights);
    printf("%d \n", chessGame->gameCastlingRights.blackCastlingRights);

    if (chessGame->possibleEnPassantDestinationPos != NULL) printf("(%d|%d) \n", chessGame->possibleEnPassantDestinationPos->X, chessGame->possibleEnPassantDestinationPos->Y);

    printf("%d \n", chessGame->playedHalfMoves);
    printf("%d \n", chessGame->nextMoveNumber);

    char FEN[128] = {0};
    ConvertGameToFEN(FEN, chessGame);
    printf("%s", FEN);

    DeleteChessBoard(&chessGame->board);

    return 0;
}








