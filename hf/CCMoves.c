#include "CCBoard.h"

/* Everything to work with chess moves */

// The different types of chess moves, to be easily defined in an enum, this is later used in the PerformMove() function to easily act different depending on moveType
typedef enum MoveType
{
    Default, // A default chess move
    EnPassant, // Google En Passant
    CastleLong, // A long castle move
    CastleShort, // A short castle move
    Promotion, // A move resulting in pawn promotion 
    //TODO: Unsure wether this type is needed (rn id thinks so, so prob remove this) 
    DoublePawnMove, // When a pawn moves 2 squares off its starting square, this is used to determinate possible en Passant moves in the next move, when performing one
} MoveType;

// Stores all information needed about any chessMove
typedef struct Move
{
    Pos moveTargetPos; // The Position the piece will be moved to, when performing this move
    Pos moveStartPos; // The pos the piece moved is placed before the move is performed
    enum MoveType moveType; // The type of this move
} Move;
