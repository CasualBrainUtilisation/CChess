#ifndef CCPieces
#define CCPieces

#include "CCBoard.h"

/* Contains enums for the Piece struct, that is also defined in here */

// The chessPieceTypes easily defined in an enum
typedef enum PieceType
{
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
    NONE, // This type is used to signalise that there is no piece present at given spot, e.g.: the hashTable of the Board struct will have pieces with PieceType NONE if there are no pieces at given pos key
} PieceType;

// The color of a chessPiece
typedef enum PieceColor
{
    White,
    Black
} PieceColor;

// Struct with all info needed to define and use a chessPiece
typedef struct Piece
{
    Pos pos; // The position of the piece on the chessBoard
    PieceType pieceType; // The type of the piece
    PieceColor pieceColor; // The color of the piece
} Piece;


#endif
