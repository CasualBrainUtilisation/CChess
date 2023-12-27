#include <string.h> //for NULL

#include "CCGeneralGameInfo.h"
#include "CCPieces.h"

/* Contains a bunch of code to work with FENs */

//Read the data from any FEN and store it as GameInfo struct
GameInfo readFENData(char FEN[])
{   
    // For first, create an empty GameInfo-struct, this will be filled with information from the FEN string
    GameInfo gameInfo;

    // Get the first part of the FEN string (containing info about the chessPiecePlacement)
    char *pPiecePlacementFENPart = strtok(FEN, " ");
    strcpy(gameInfo.currentPosAsFENString, pPiecePlacementFENPart);
    
    gameInfo.currentTurn = White;
    GameCastlingRights gameCastlingRights = {BothSides, BothSides};
    gameInfo.gameCastlingRights = gameCastlingRights;
    gameInfo.nextMoveNumber = 3;
    gameInfo.playedHalfMoves = 50;
    Pos enPassantPos = {120, 120};
    gameInfo.possibleEnPassantDestinationPos = enPassantPos;

    return gameInfo;
}