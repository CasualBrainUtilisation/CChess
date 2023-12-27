#include <stdio.h>
#include "hf/CCBoard.h"
#include "hf/FEN.h"
#include "hf/CCGeneralGameInfo.h"


int main()
{
    printf("hi");
    GameInfo gameInfo = readFENData("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    printf(gameInfo.currentPosAsFENString);

    return 0;
}








