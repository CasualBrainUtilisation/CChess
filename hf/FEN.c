#include <string.h> //for NULL

#include "CCGeneralGameInfo.h"
#include "CCPieces.h"

/* Contains a bunch of code to work with FENs */

//Read the data from any FEN and store it as GameInfo struct
GameInfo readFENData(char FEN[])
{   
    // For first, create an empty GameInfo-struct, this will be filled later
    GameInfo gameInfo;

    char *pFenPart = strtok(FEN, " ");

    //This array will contain all 6 parts of the FEN divided by spaces
    char fenParts[6][128];

    for (int i = 0; i < 6; i++)
    {
        strcpy(fenParts[i], pFenPart);
        
        pFenPart = strtok(NULL, " ");

        if (pFenPart == NULL) break;
    }
    

    strcpy(gameInfo.currentPosAsFENString, fenParts[0]);
    gameInfo.currentTurn = White;
    GameCastlingRights gameCastlingRights = {BothSides, BothSides};
    gameInfo.gameCastlingRights = gameCastlingRights;
    gameInfo.nextMoveNumber = 3;
    gameInfo.playedHalfMoves = 50;
    Pos enPassantPos = {120, 120};
    gameInfo.possibleEnPassantDestinationPos = enPassantPos;

    return gameInfo;
}