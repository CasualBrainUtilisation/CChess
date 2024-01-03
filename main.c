#include <stdio.h>
#include "hf/CCBoard.h"
#include "hf/FEN.h"
#include "hf/CCGeneralGameInfo.h"
#include "hf/CCVisualizer.h"


int main()
{
    // printf("hi");
    // GameInfo gameInfo = readFENData("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    // printf("%s \n", gameInfo.currentPosAsFENString);
    // printf("%d", gameInfo.currentTurn);

    Board *chessBoard = InitBoard();

    PrintBoard(chessBoard);

    DeleteChessBoard(&chessBoard);

    return 0;
}








