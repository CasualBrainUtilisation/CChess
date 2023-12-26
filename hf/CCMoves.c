#include "CCBoard.h"

/* Everything to work with chess moves */

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
    Pos moveDestinationPos; // the destinated field of the move
    enum MoveType moveType; // the moveType performed
} Move;
