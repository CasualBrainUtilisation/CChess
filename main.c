#include <stdio.h>

void main()
{
    
}


// Pos represents a position on the chessBoard, while MoveDir represents the the direction of a chessPieceMove
typedef struct Vector2
{
    int X;
    int Y;
} Pos, MoveDir;

/* Chess Pieces */
// The chessPieceTypes easily defined in an enum
enum PieceType
{
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
};

// A single ChessPiece
typedef struct Piece
{
    Pos pos;
    enum PieceType pieceType;
} Piece;


/* Moves */
// The different types of chess moves, to be easily defined in an enum
enum MoveType
{
    Default, // The default chess move
    PawnDiagonal, // When a pawn consumes a piece by moving diagonally
    EnPassant, // Google en passant
    Castle, // A castle move
};

// The struct for a move itself
typedef struct Move
{
    MoveDir moveDir; // the direction (and length) of a chess move
    enum MoveType moveType; // the moveType performed
} Move;

