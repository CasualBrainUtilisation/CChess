#include "CCPieces.h"

/* Contains the ChessBoard and Pos struct (which represents a pos on the chessboard) and also all the functions that impact or require the ChessBoard struct */

// Pos represents a position on the chessBoard
// This can be imagined like a cordinate system on the board
typedef struct Vector2
{
    int X; // The X represents the files of the ChessBoard (The numbers 1 - 8 (although we use the scope of 0-7))
    int Y; // The Y represents the ranks of the ChessBoard (The letters a - h (with 0-7))
} Pos;

// Struct containing info about a chessBoard aka thee pieces placed on top
typedef struct Board
{
    Piece *pAllPieces; // A simple pointer used as array that contains all the chessPieces present on Board
    Piece *piecesHashTable[64]; // A hashTable that uses the pos of a piece as key an stores the pointer to the piece as value
} Board;