#ifndef CCMoves
#define CCMoves

#include "CCPieces.h" // Piece and Pos are required
#include "CChessGame.h"

/* Everything to work with chess moves */

// The different types of chess moves, to be easily defined in an enum, this is later used in the PerformMove() function to easily act different depending on moveType
typedef enum MoveType
{
    Default, // A default chess move
    EnPassant, // Google En Passant //TODO: this prob is actually unecessary (not sure rn though we'll C)
    CastleLong, // A long castle move
    CastleShort, // A short castle move
    DoublePawnMove, // When a pawn moves 2 squares off its starting square, this is used to determinate possible en Passant moves in the next move, when performing one
} MoveType;

// Stores all information needed about any chessMove
typedef struct Move
{
    Pos moveStartPos; // The pos the pieceToMove is placed before the move is performed
    Pos moveTargetPos; // The Position the piece will be moved to, when performing this move

    Piece *PieceToMove; // The Piece that will be moved when this move is performed
    Piece *PieceToCapture; // The Piece that will be captured when this move is performed, in case there actually is no piece that will be captured by this move, this Pointer will be NULL

    enum MoveType moveType; // The type of this move
} Move;

// This struct is used to represent the direction of a move
typedef struct Vector2 MoveDir;

/* Move Functions */
// Following functions will all be related to moves

/* Move Data Linked List */
// For storing moves, we'll need a linked list (as we are gonna add to our list constantly in the getMoves() functions)
// The elements in the linked list
typedef struct MoveData
{
    Move data; // Data stored in an element
    struct MoveData *Next; // The next element in the linked list
} MoveData;

//Kepps track of created move data linked list
typedef struct MoveDataLinkedList
{
    MoveData *Head; // The head of the MoveData linked list (aka first element)
    MoveData *LastElement; // The last element of the moveData linked list, which is kept track of to easily add to the list

    size_t listSize; // Keeps track of the size of the list
} MoveDataLinkedList;

// Deletes given moveDataLinkedList data alongside freeing everything in there that we mallocated
// Must be called to delete any MoveDataLinkedList (aslong as you actually wanna avoid memory leaks that is)
void DeleteMoveDataLinkedList(MoveDataLinkedList **moveDataLinkedListToDelete);

// Performs given move on given board
void PerformMove(Move moveToPerform, Board *board);

// Gives back all the possible moves for given piece on given board
MoveDataLinkedList *GetAllMovesForPiece(Piece *pieceToGetMovesFor, ChessGame *chessGame);

#endif