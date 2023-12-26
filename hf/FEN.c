#include <string.h> //for NULL

#include "CCGeneralGameInfo.h"

/* Contains a bunch of code to work with FENs */

GameInfo readFENData(char FEN[])
{   
    // For first, create an empty GameInfo-struct, this will be filled later
    GameInfo gameInfo;

    char *pFenPart = strtok(FEN, " ");

    //This array will contain all 6 parts of the FEN divided by spaces
    char fenParts[6];

    for (int i = 0; i < 6; i++)
    {
        fenParts[i] = pFenPart;
        
        pFenPart = strtok(NULL, " ");

        if (pFenPart == NULL) break;
    }
    

    strcopy(gameInfo.currentPosAsFENString, fenParts[0]);
    strcopy(gameInfo.currentTurn, fenParts[1]);
    GameCastlingRights gameCastlingRights = {CastlingRights.BothSides, CastlingRights.BothSides};
    gameInfo.gameCastlingRights = gameCastlingRights;
    gameInfo.nextMoveNumber = 3;
    gameInfo.playedHalfMoves = 50;
    gameInfo.possibleEnPassantDestinationPos = {120, 120};

    return gameInfo;
}