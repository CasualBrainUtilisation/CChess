#include <string.h> // for strtok ect
#include <ctype.h> // for lowercase ect.
#include <stdlib.h> // malloc

#include "CChessGame.h"
#include "CCPieces.h"

/* Contains a bunch of code to work with FENs */

// Read the data from any FEN and return game from it, or return NULL to indicate an invalid FEN
ChessGame *GetGameFromFEN(char FEN[])
{   
    // If the FEN is to large (It generally already is at a size of about over 90, but in this case we caree wether it is too large to be copied in the FENToRead array created later) already return with valid beeing 1, to indicate failure //TODO: Make sure all strings this func gets are null terminated !
    if (strlen(FEN) >= 128) //FIXME: not sure wether this is actually safe
    {
        return NULL;
    }

    // For first, create an empty ChessGame-struct, this will be filled with information from the FEN string
    ChessGame *loadedChessGameFromFEN = malloc(sizeof(ChessGame));

    // Create a new char[] copied from FEN, so we have acces to its address, and we don't care about it changing with strtok
    char FENToRead[128];
    strcpy(FENToRead, FEN); //FIXME: Unsure about strcpy beeing safe

    // Get the first part of the FEN string (containing info about the chessPiecePlacement)
    char *pPiecePlacementFENPart = strtok(FENToRead, " ");
    
    // Now create a board and fill it using the pPiecePlacementFENPart extracted from the FEN[]
    loadedChessGameFromFEN->board = InitBoard();

    // We'll loop through the whole pPiecePlacementFENPart array in order to place all the pieces accordingly on the new board

    // x and y represent the x and y position we currently are inspecting for piece placement while looping through the pPiecePlacementFENPart
    int x = 0;
    int y = 0;

    // Loop through the whole pPiecePlacementFENPart string
    for (char curChar = *pPiecePlacementFENPart; curChar != '\0'; curChar = *++pPiecePlacementFENPart)
    {
        // In case x is over 8 and y is over 7, the FEN must have been invalid so free opened memory and return NULL
        if (x >= 9 || y >= 8)
        {
            free(loadedChessGameFromFEN);
            return NULL;
        }

        // Get the number the curChar represents (as long as it as a number that is)
        int numberOfCurChar = curChar - '0';
        // Check wether the curChar actually represents a valid number
        if (numberOfCurChar >= 0 && numberOfCurChar <= 7)
        {
            // Incre x by the number of char now
            x += numberOfCurChar;

            // Continue with the next char, as for this one we've already taken action
            continue; 
        }

        // Check wether the curChar represents a slash, which indicates starting on the beginning of a new file of the chessBoard
        if (curChar == '/')
        {
            // Set x back to 0, as we'll begin at the beginning of the new file
            x = 0;
            // Incree y which is keeping track of the file
            y++;

            // Continue with the next char, as for this one we've already taken action
            continue;
        }

        // The characters used to represent piece types, the index of them in this array is equal to their value in the pieceType enum
        char *pieceChars = "pnbrqk";
        // Loop through the whole pieceChars array and check wether any of the chars in there match the curChar
        for (char curPieceChar = *pieceChars, curPieceCharIndex = 0; curPieceChar != '\0'; curPieceChar = *++pieceChars, curPieceCharIndex++)
        {
            // If the pieceChar actually matches the curChar, add according piece to the board
            if (curPieceChar == tolower(curChar))
            {
                // Add the piece to the board
                Pos pieceToAddPos = {x, y};
                AddPiece(curPieceCharIndex, islower(curChar), pieceToAddPos, loadedChessGameFromFEN->board);

                // After adding a piece x also needs to be incremented, as the next square  after this added piece is approeached now
                x++;

                // Break out of this while loop, as we've now found the matching pieceChar, which in fact was the whole purpose of this loop
                break;
            }
        }

    }
    
    // Continue reading the FEN string, next will be information about the current player turn
    char *pCurrentTurnFENPart = strtok(NULL, " ");
    if (tolower(pCurrentTurnFENPart[0]) == 'w') //Check weter the letter "w" is represented here, in that case, its white's turn
    {
        loadedChessGameFromFEN->currentTurn = White;
    }
    else if (tolower(pCurrentTurnFENPart[0]) == 'b') 
    {
        loadedChessGameFromFEN->currentTurn = Black;
    }
    
    GameCastlingRights gameCastlingRights = {BothSides, BothSides};
    loadedChessGameFromFEN->gameCastlingRights = gameCastlingRights;
    loadedChessGameFromFEN->nextMoveNumber = 3;
    loadedChessGameFromFEN->playedHalfMoves = 50;
    Pos enPassantPos = {120, 120};
    loadedChessGameFromFEN->possibleEnPassantDestinationPos = &enPassantPos;

    return loadedChessGameFromFEN;
}