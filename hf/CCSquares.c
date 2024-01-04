#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* Contains the pos struct, representing a square on board, aswell as functions to convert these into strings and to get a pos of a string */

// Pos represents a position on the chessBoard
// This can be imagined like a cordinate system on the board from 0-7 on x and y, notice though that (0|0) is placed on the top left of the board, rather then the bottom left
typedef struct Vector2
{
    int X; // The X represents the files of the ChessBoard (The numbers 0-7)
    int Y; // The Y represents the ranks of the ChessBoard (The letters a - h (with 0-7))
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
    // Get the digit represented by the first char of the fieldNotation and set the X to it
    X = fieldNotation[1] - '0';
    // Check wether the X actually is a valid digit char for the fieldNotation, if not return NULL
    if (X < 1 || 8 < X) return NULL;
    // As opposed to normal chessNotation for us the top left actually is pos 0|0 and we have a reange of 0-7, so we kindof mirror the X, while also moving the range from 1-8 to 0-7
    X = 8 - X;

    /* Getting Y */
    // Here we get pos.y the letters in the square string represent, this will also work for capital letters, cuz of tolower() (which aint valid notation normally)
    Y = tolower(fieldNotation[0]) - 'a';
    // Check wether the Y though actually gave a valid pos.y in the range of 0-7, if not return NULL
    if (Y < 0 || 7 < Y) return NULL;

    /* Creating and returning posToReturn*/
    // Malloc the posToReturn
    Pos *posToReturn = malloc(sizeof(Pos));

    // Set its posToReturn's x and y to those calculated before
    posToReturn->X = X;
    posToReturn->Y = Y;

    // Return the fun posToReturn
    return posToReturn;
}