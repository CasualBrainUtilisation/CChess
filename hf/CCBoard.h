#ifndef CCBoard
#define CCBoard

/* Contains the ChessBoard and Pos struct (which represents a pos on the chessboard) and also all the functions that impact or require the ChessBoard struct */

/* Boards Info */
// Pos represents a position on the chessBoard
// This can be imagined like a cordinate system on the board
typedef struct Vector2
{
    int X; // The X represents the files of the ChessBoard (The numbers 1 - 8 (although we use the scope of 0-7))
    int Y; // The Y represents the ranks of the ChessBoard (The letters a - h (with 0-7))
} Pos;

#endif