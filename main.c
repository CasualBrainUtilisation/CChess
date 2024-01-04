#include <stdio.h>
#include "hf/CCBoard.h"
#include "hf/CCFEN.h"
#include "hf/CChessGame.h"
#include "hf/CCVisualizer.h"


int main()
{
    // printf("hi");
    // GameInfo gameInfo = readFENData("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    // printf("%s \n", gameInfo.currentPosAsFENString);
    // printf("%d", gameInfo.currentTurn);

    ChessGame *chessGame = GetGameFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    PrintBoard(chessGame->board);

    DeleteChessBoard(&chessGame->board);

    return 0;
}








