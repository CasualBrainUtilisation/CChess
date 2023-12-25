#include <stdio.h>

void main()
{
    
}

/* Board Info */
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

// The color of a chessPiece
enum PieceColor
{
    White,
    Black
};

// A single ChessPiece
typedef struct Piece
{
    Pos pos; // The position of the piece on the chessBoard
    enum PieceType pieceType; // The type of the piece
    enum PieceColor pieceColor; // The color of the piece
} Piece;


/* Moves */
// The different types of chess moves, to be easily defined in an enum
enum MoveType
{
    Default, // The default chess move
    PawnDiagonal, // When a pawn consumes a piece by moving diagonally, needed for en passant //TODO: actually give en passant use case or remove
    DoublePawnMove, // When a pawn makes a double move from its start pos, needed for en passant //TODO: actually give en passant use case or remove
    EnPassant, // Google en passant
    Castle, // A castle move
};

// The struct for a move itself
typedef struct Move
{
    MoveDir moveDir; // the direction (and length) of a chess move
    enum MoveType moveType; // the moveType performed
} Move;


/* General Info */
/* Castling */
// The current state of the castling rights
enum CastlingRights
{
    BothSides,
    QueenSide,
    KingSide,
    None
};

// Keeps track of the castling rights for both colors in a game
typedef struct GameCastlingRights 
{
    enum CastlingRights whiteCastlingRights;
    enum CastlingRights blackCastlingRights;
} GameCastlingRights;


/* Game Info */
// Geral Info about a chess game
typedef struct GameInfo
{
    char currentPosAsFENString[128]; // The current boardPosition as FEN
    enum PieceColor currentTurn; // Color that currently is on turn
    GameCastlingRights gameCastlingRights; // The current castling rights for the ongoing game
    Pos possibleEnPassantDestination; // The field that a pawn had to move to, to perform an en passant move in the current state of game
    short playedHalfMoves; // Amount of moves since last capture or pawn moves (Used for draw rules)
    short nextMoveNumber; // The number of the next move to be performed
} GameInfo;
