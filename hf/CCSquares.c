#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* Contains the pos struct, representing a square on board, aswell as functions to convert these into strings and to get a pos of a string */

// Pos represents a position on the chessBoard
// This can be imagined like a cordinate system on the board from 0-7 on x and y, notice though that (0|0) is placed on the top left of the board, rather then the bottom left
typedef struct Vector2
{
    int X; // The X represents the files of the ChessBoard (The letters a - h (with 0-7))
    int Y; // The Y represents the ranks of the ChessBoard (The numbers 1-8 (with 0-7))
} Pos;

// Function that returns a Pos pointer, from given FieldNotation (e.g.: e4, h1 ...), returns NULL if the FieldNotation is invalid
// Given strings should be null terminated
Pos *GetPosFromFieldNotation(char *fieldNotation)
{
    // If the fieldNotation is smaller then 2, it wouldn't have enough chars to actually contain a valid fieldNotation so return NULL
    // Notice that the string should have a '\0' for this to work, else we can not determinate the length with strlen()
    if (strlen(fieldNotation) < 2)
    {
        return NULL;
    }

    // The values that the returned pos pointers x and y will be set to later on
    int X = 0;
    int Y = 0;

    /* Getting X */
    // Here we get pos.x the letters in the square string represent, this will also work for capital letters, cuz of tolower() (which aint valid notation normally)
    X = tolower(fieldNotation[0]) - 'a';
    // Check wether the X though actually gave a valid pos.x in the range of 0-7, if not return NULL
    if (X < 0 || 7 < X) return NULL;

    /* Getting Y */
    // Get the digit represented by the first char of the fieldNotation and set the Y to it
    Y = fieldNotation[1] - '0';
    // Check wether the Y actually is a valid digit char for the fieldNotation, if not return NULL
    if (Y < 1 || 8 < Y) return NULL;
    // As opposed to normal chessNotation for us the top left actually is pos 0|0 and we have a reange of 0-7, so we kindof mirror the Y, while also moving the range from 1-8 to 0-7
    Y = 8 - Y;

    /* Creating and returning posToReturn*/
    // Malloc the posToReturn
    Pos *posToReturn = malloc(sizeof(Pos));

    // Set its posToReturn's x and y to those calculated before
    posToReturn->X = X;
    posToReturn->Y = Y;

    // Return the fun posToReturn
    return posToReturn;
}

// Function that sets the given fieldNotationToSet for given pos
// Given Pos will not be checked for validity, make sure it's valid
void GetFieldNotationFromPos(Pos *pos, char *fieldNotationToSet)
{
    // We're effectifly reversing what's done in GetPosFromFieldNotation() without safety checks
    // Add 'a' to the y pos, resulting in a range from a-h
    fieldNotationToSet[0] = pos->X + 'a';
    // Mirror the pos.y and change the range from 0-7 to 1-8, then convert the number to an ascii char
    fieldNotationToSet[1] = 8 - pos->Y + '0';
}

// Checks wether given position is on the valid range of a chessBoard (meaning both x and y would have to be in a range from 0-7)
int IsPosOnBoard(Pos pos)
{
    return pos.X >= 0 && pos.X <= 7 && pos.Y >= 0 && pos.Y <= 7;
}