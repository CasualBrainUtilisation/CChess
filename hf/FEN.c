#include <string.h> //for strtok ect
#include <ctype.h> // for lowercase ect.

#include "CCGeneralGameInfo.h"
#include "CCPieces.h"

/* Contains a bunch of code to work with FENs */

//Read the data from any FEN and store it as GameInfo struct
GameInfo readFENData(char FEN[])
{   

    // For first, create an empty GameInfo-struct, this will be filled with information from the FEN string
    GameInfo gameInfo;

    // Create a new char[] copied from FEN, but we have acces to its address, and we don't care about it changing with strtok
    char FENToRead[128];
    strcpy(FENToRead, FEN);

    // Get the first part of the FEN string (containing info about the chessPiecePlacement)
    char *pPiecePlacementFENPart = strtok(FENToRead, " ");
    strcpy(gameInfo.currentPosAsFENString, pPiecePlacementFENPart);
    
    // Continue reading the FEN string, next will be information about the current player turn
    char *pCurrentTurnFENPart = strtok(NULL, " ");
    if (tolower(pCurrentTurnFENPart[0]) == 'w') //Check weter the letter "w" is represented here, in that case, its white's turn
    {
        gameInfo.currentTurn = White;
    }
    else if (tolower(pCurrentTurnFENPart[0]) == 'b') 
    {
        gameInfo.currentTurn = Black;
    }
    
    GameCastlingRights gameCastlingRights = {BothSides, BothSides};
    gameInfo.gameCastlingRights = gameCastlingRights;
    gameInfo.nextMoveNumber = 3;
    gameInfo.playedHalfMoves = 50;
    Pos enPassantPos = {120, 120};
    gameInfo.possibleEnPassantDestinationPos = enPassantPos;

    return gameInfo;
}