#include <string.h> // for strtok + strtol ect.
#include <ctype.h> // for lowercase ect.
#include <stdio.h> // for sprintf
#include <errno.h> // for well errno

#include "CChessGame.h"
#include "CCPieces.h"

/* Contains a bunch of code to work with FENs */

// Read the data from any FEN and return game from it, or return NULL to indicate an invalid FEN
ChessGame *GetGameFromFEN(char FEN[])
{   
    // If the FEN is to large (It generally already is at a size of about over 90, but in this case we caree wether it is too large to be copied in the FENToRead array created later) already return with valid beeing 1, to indicate failure // Make sure all strings this func gets are null terminated (for strlen()) !
    if (strlen(FEN) >= 128)
    {
        return NULL;
    }

    // For first, create an empty ChessGame-struct, this will be filled with information from the FEN string
    ChessGame *loadedChessGameFromFEN = InitChessGame();

    // Create a new char[] copied from FEN, so we have acces to its address, and we don't care about it changing with strtok
    char FENToRead[128];
    strcpy(FENToRead, FEN); //FIXME: Unsure about strcpy beeing safe (should be as im checking length above)


    /* Piece Placement*/

    // Get the first part of the FEN string (containing info about the chessPiecePlacement)
    char *pPiecePlacementFENPart = strtok(FENToRead, " ");

    // We'll loop through the whole pPiecePlacementFENPart array in order to place all the pieces accordingly on the new board

    // x and y represent the x and y position we currently are inspecting for piece placement while looping through the pPiecePlacementFENPart
    int x = 0;
    int y = 0;

    // Loop through the whole pPiecePlacementFENPart string
    for (char curChar = *pPiecePlacementFENPart; curChar != '\0'; curChar = *++pPiecePlacementFENPart)
    {
        // In case x is over 8 and y is over 7, the FEN must have been invalid so free opened memory and return NULL
        // Notice though that calling this here will not check effects of the last char, which could be invalid, but that doesn't matter
        if (x >= 9 || y >= 8)
        {
            DeleteChessGame(&loadedChessGameFromFEN);
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
        const char *pieceChars = "pnbrqk";
        // Loop through the whole pieceChars array and check wether any of the chars in there match the curChar
        for (char curPieceChar = *pieceChars, curPieceCharIndex = 0; curPieceChar != '\0'; curPieceChar = *++pieceChars, curPieceCharIndex++)
        {
            // If the pieceChar actually matches the curChar, add according piece to the board
            if (curPieceChar == tolower(curChar))
            {
                // If the piece is supposed to be placed outside the bounds of the chessBoard return NULL and free allocated memory, as the FEN must be invalid
                if (x >= 8 || y >= 8)
                {
                    DeleteChessGame(&loadedChessGameFromFEN);
                    return NULL;
                }

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
    if (tolower(pCurrentTurnFENPart[0]) == 'w') // Check wether the letter 'w' is represented here, in that case, its white's turn
    {
        loadedChessGameFromFEN->currentTurn = White;
    }
    else if (tolower(pCurrentTurnFENPart[0]) == 'b') // Else if the letter 'b' is represented here, it's black's turn
    {
        loadedChessGameFromFEN->currentTurn = Black;
    }
    else // If though neither 'b' nor 'w' are represented here, the FEN is invalid so free allocated mem and return NULL
    {
        DeleteChessGame(&loadedChessGameFromFEN);
        return NULL;
    }


    /* Castling Rights */
    // For getting the castling rights from the FEN, we assume that the loadedChessGameFromFEN->gameCastlingRights for white and black are both set to None on default!!

    // Continue reading the FEN string, next will be information about the castling rights of the players
    char *pCastlingRightsFENPart = strtok(NULL, " ");
    // We'll check each char of the string for implementing castling rights
    for (char curCastlingRightsChar = *pCastlingRightsFENPart; curCastlingRightsChar != '\0'; curCastlingRightsChar = *++pCastlingRightsFENPart)
    {
        // Depending on the current char we'll react differently
        switch(curCastlingRightsChar)
        {
            // In case it's an 'Q', white can castle queenside
            case 'Q' :
                // So add QueenSideRights for white, on fail remove allocated mem and return NULL
                if (AddCastlingRight(QueenSide, &loadedChessGameFromFEN->gameCastlingRights.whiteCastlingRights) == 1)
                {
                    DeleteChessGame(&loadedChessGameFromFEN);
                    return NULL;
                }
                break;
            // In case it's an 'K', white can castle kingside
            case 'K' :
                // So add KingSideRights for white
                if (AddCastlingRight(KingSide, &loadedChessGameFromFEN->gameCastlingRights.whiteCastlingRights) == 1)
                {
                    DeleteChessGame(&loadedChessGameFromFEN);
                    return NULL;
                }
                break;
            // In case it's an 'q', black can castle queenside
            case 'q' :
                // So add QueenSideRights for black
                if (AddCastlingRight(QueenSide, &loadedChessGameFromFEN->gameCastlingRights.blackCastlingRights) == 1)
                {
                    DeleteChessGame(&loadedChessGameFromFEN);
                    return NULL;
                }
                break;
            // In case it's an 'k', black can castle kingside
            case 'k' :
                // So add KingSideRights for black
                if (AddCastlingRight(KingSide, &loadedChessGameFromFEN->gameCastlingRights.blackCastlingRights) == 1)
                {
                    DeleteChessGame(&loadedChessGameFromFEN);
                    return NULL;
                }
                break;
            // A '-' indicates that no castling rights are to be set, so just leave them at the default NONE
            case '-' :
                break;
            // If there is any other character here, the FEN is invalid, so free allocated mem and return NULL //TODO: decide wether to actually do this or wether to just ignore cases like this
            // default :
            //     DeleteChessGame(&loadedChessGameFromFEN);
            //     return NULL;
        }
    }

    /* En Passant */
    // Continue reading the FEN string, next will be the possible En Passant move
    char *pPossibleEnPassantMovesFENPart = strtok(NULL, " ");
    // Get the Pos from the pPossibleEnPassantMovesFENPart field notation, GetPosFromFieldNotation() will return NULL if this notation is invalid, though we'll just take the NULL then //TODO: idk prob make things like this actually not work
    loadedChessGameFromFEN->possibleEnPassantDestinationPos = GetPosFromFieldNotation(pPossibleEnPassantMovesFENPart);
    

    /* PlayedHalfMoves */
    // Continue reading the FEN string, next will be the playedHalfMoves
    char *pPlayedHalfMovesFENPart = strtok(NULL, " ");

    // Set errno to 0, as we'll have to check wether following strtol() returns an error
    errno = 0;
    // We'll use strtol() to get the int represented in the pPlayedHalfMovesFENPart
    loadedChessGameFromFEN->playedHalfMoves = strtol(pPlayedHalfMovesFENPart, NULL, 10);

    // Check wether errno ain't 0 anymore, which would mean that in strtol something went wrong, so return NULL and free allocated mem
    if (errno != 0)
    {
        DeleteChessGame(&loadedChessGameFromFEN);
        return NULL;
    }


    /* NextMoveNumber */
    // Continue reading the FEN string, next will be the nextMoveNumber
    char *pNextMoveNumberFENPart = strtok(NULL, " ");

    // Set errno to 0, as we'll have to check wether following strtol returns an error
    errno = 0;
    // We'll use strtol to get the int represented in the pNextMoveNumberFENPart
    loadedChessGameFromFEN->nextMoveNumber = strtol(pNextMoveNumberFENPart, NULL, 10);

    // Check wether errno ain't 0 anymore, which would mean that in strtol something went wrong, so return NULL and free allocated mem
    if (errno != 0)
    {
        DeleteChessGame(&loadedChessGameFromFEN);
        return NULL;
    }

    return loadedChessGameFromFEN;
}

// Converts the given ChessGame to a FEN string array and sets the given FENToChange to it (wow such gorgeus function!)
void ConvertGameToFEN(char FENToReturn[128], ChessGame *chessGameToConvert)
{
    // Keeps track of the index that is next to be set in the FEN
    int curIndex = 0;

    /* Piece Position */

    // The amount of boardFields that have been checked since the last piece was added to the FEN
    int fieldsSinceLastPiece = 0;
    // Loop through the chessBoard, keeping track of y and x cords, y comes first as ranks are simply seperated by '/' while files take multiple chars
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            // Get the pos we currently are inspecting
            Pos curPos = {x, y}; 
            // Get the piece at the curPos
            Piece *pieceAtCurPos = GetPieceAtPos(curPos, chessGameToConvert->board);
            // Check wether there actually is a piece at the curPos
            if (pieceAtCurPos != NULL)
            {
                // The chars to represent the different peice types, to get corresponding care (enum)PieceType can be used as index
                const char *pieceChars = "pnbrqk";
                // Get the pieceChar corresponding to the pieceAtCurPos' pieceType
                char pieceTypeChar = pieceChars[pieceAtCurPos->pieceType];

                // If we have anyything of meaning for the FEN to store in the fieldsSinceLastPiece add corresponding char to the FEN
                if (fieldsSinceLastPiece != 0)
                {
                    FENToReturn[curIndex] = fieldsSinceLastPiece + '0';
                    curIndex++;
                    // Reset the fieldsSinceLastPiece back to 0, as we have just added the old value
                    fieldsSinceLastPiece = 0;
                }

                // Now append the pieceTypeChar (in caps if pieceAtCurPos is White) to the FENToReturn
                FENToReturn[curIndex] = pieceAtCurPos->pieceColor == White ? toupper(pieceTypeChar) : pieceTypeChar;
                curIndex++;
            }
            // If there ain't a piece at the curPos we'll incre fieldsSinceLastPiece
            else fieldsSinceLastPiece++;
        }

        // If we have anyything of meaning for the FEN to store in the fieldsSinceLastPiece add corresponding char to the FEN
        if (fieldsSinceLastPiece != 0)
        {
            FENToReturn[curIndex] = fieldsSinceLastPiece + '0';
            curIndex++;
            // Reset the fieldsSinceLastPiece back to 0, as we have just added the old value
            fieldsSinceLastPiece = 0;
        }

        // Add a '/' to the FENToReturn indicate the end of a file
        FENToReturn[curIndex] = '/';
        curIndex++;
    }
    // Skip one index here to seperate the Piece Position from the CurrentTurn section of the FEN with a space (spaces are good)
    FENToReturn[curIndex] = ' ';
    curIndex++;


    /* Current Turn */
    // Add a 'w' if the chessGameToConvert->currentTurn is White and a 'b' if it's not (meaning it's black)
    FENToReturn[curIndex] = chessGameToConvert->currentTurn == White ? 'w' : 'b'; 
    curIndex++;

    // Skip one index here to seperate the CurrentTurn section from the CastleRights section of the FEN with a space (spaces are good)
    FENToReturn[curIndex] = ' ';
    curIndex++;


    /* Castling Manager */
    // If the startCastlingCurIndex doesn't change, that means that we haven't added any castling rule to the FEN, so we have to add a '-'
    int startCastlingCurIndex = curIndex;

    // Add corresponding characters foreach castlingRight
    switch (chessGameToConvert->gameCastlingRights.whiteCastlingRights)
    {
        case QueenSide:
            FENToReturn[curIndex] = 'Q';
            curIndex++;
            break;

        case KingSide:
            FENToReturn[curIndex] = 'K';
            curIndex++;
            break;

        case BothSides:
            FENToReturn[curIndex] = 'K';
            curIndex++;
            FENToReturn[curIndex] = 'Q';
            curIndex++;
            break;
    }

    switch (chessGameToConvert->gameCastlingRights.blackCastlingRights)
    {
        case QueenSide:
            FENToReturn[curIndex] = 'q';
            curIndex++;
            break;

        case KingSide:
            FENToReturn[curIndex] = 'k';
            curIndex++;
            break;

        case BothSides:
            FENToReturn[curIndex] = 'k';
            curIndex++;
            FENToReturn[curIndex] = 'q';
            curIndex++;
            break;
    }

    // If the startCastlingCurIndex didn't change, that means that we haven't added any castling rule to the FEN, so we have to add a '-'
    if (curIndex == startCastlingCurIndex)
    {
        FENToReturn[curIndex] = '-';
        curIndex++;
    }

    // Skip one index here to seperate the CastleRights section from the EnPassant section of the FEN with a space (spaces are good)
    FENToReturn[curIndex] = ' ';
    curIndex++;

    
    /* Possible En Passant Move */
    // Check wether there actually is any possibleEnPassantDestinationPos to add here
    if (chessGameToConvert->possibleEnPassantDestinationPos != NULL)
    {
        // In this array wee'll store the fieldNotationForPossibleEnPassantDestinationPos
        // Notice that this array won't have a '\0' in the end (which we don't need)
        char fieldNotationForPossibleEnPassantDestinationPos[2];
        // Save the fieldNotation from the possibleEnPassantDestinationPos into fieldNotationForPossibleEnPassantDestinationPos
        GetFieldNotationFromPos(chessGameToConvert->possibleEnPassantDestinationPos, fieldNotationForPossibleEnPassantDestinationPos);

        // Add the field notation we got to FENToReturn
        FENToReturn[curIndex] = fieldNotationForPossibleEnPassantDestinationPos[0];
        curIndex++;
        FENToReturn[curIndex] = fieldNotationForPossibleEnPassantDestinationPos[1];
        curIndex++;
    }
    // If there is no possibleEnPassantDestinationPos just add a dash
    else
    {
        FENToReturn[curIndex] = '-';
        curIndex++;
    }

    // Skip one index here to seperate the EnPassant section from the HalfMovesSinceCaptureOrPawnMove section of the FEN with a space (spaces are good)
    FENToReturn[curIndex] = ' ';
    curIndex++;


    /* HalfMovesSinceCaptureOrPawnMove */

    // The number (HalfMovesSinceCaptureOrPawnMove) that is to be added to the FENToReturn will be stored in here as string
    char halfMoveNumberToAddAsString[16];
    sprintf(halfMoveNumberToAddAsString, "%d", chessGameToConvert->playedHalfMoves);

    // Add each char of the halfMoveNumberToAddAsString to the FEN
    for (int i = 0; halfMoveNumberToAddAsString[i] != '\0'; i++)
    {
        FENToReturn[curIndex] = halfMoveNumberToAddAsString[i];
        curIndex++;
    }

    // Skip one index here to seperate the HalfMovesSinceCaptureOrPawnMove section from the nextMoveNumber section of the FEN with a space (spaces are good)
    FENToReturn[curIndex] = ' ';
    curIndex++;


    /* NextMoveNumber */

    // The number (HalfMovesSinceCaptureOrPawnMove) that is to be added to the FENToReturn will be stored in here as string
    char nextMoveNumberToAddAsString[16];
    sprintf(nextMoveNumberToAddAsString, "%d", chessGameToConvert->nextMoveNumber);

    // Add each char of the nextMoveNumberToAddAsString to the FEN
    for (int i = 0; nextMoveNumberToAddAsString[i] != '\0'; i++)
    {
        FENToReturn[curIndex] = nextMoveNumberToAddAsString[i];
        curIndex++;
    }
}