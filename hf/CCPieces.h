#ifndef CCPieces
#define CCPieces

/* Contains enums for the Piece struct, that is also defined in here, aswell as the pos struct */

// Pos represents a position on the chessBoard
// This can be imagined like a cordinate system on the board from 0-7 on x and y
typedef struct Vector2
{
    int X; // The X represents the files of the ChessBoard (The numbers 0-7)
    int Y; // The Y represents the ranks of the ChessBoard (The letters a - h (with 0-7))
} Pos;

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

// Struct with all info needed to define and use a chessPiece
typedef struct Piece
{
    Pos pos; // The position of the piece on the chessBoard
    PieceType pieceType; // The type of the piece
    PieceColor pieceColor; // The color of the piece
} Piece;

#endif
