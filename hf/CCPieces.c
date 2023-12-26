#include "CCBoard.h"

/* everything to work with chessPieces */

// The chessPieceTypes easily defined in an enum
typedef enum PieceType
{
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
} PieceType;

// The color of a chessPiece
typedef enum PieceColor
{
    White,
    Black
} PieceColor;

// A single ChessPiece
typedef struct Piece
{
    Pos pos; // The position of the piece on the chessBoard
    PieceType pieceType; // The type of the piece
    PieceColor pieceColor; // The color of the piece
} Piece;
