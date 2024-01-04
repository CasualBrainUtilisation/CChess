#ifndef CCSquares
#define CCSquares

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
Pos *GetPosFromFieldNotation(char *fieldNotation);

#endif