#ifndef CCBoard
#define CCBoard

#include <stdlib.h>
#include "CCPieces.h"

/* Contains the ChessBoard and all the functions that impact or require the ChessBoard struct */

// Struct for the linked list defined on board
typedef struct PieceData
{
    Piece *data; // The data of stored piece
    struct PieceData *Next; // The next element in the linked list
} PieceData;

// Struct containing info about a chessBoard aka the pieces placed on top 
// Should only be initialized with the InitBoard() func
typedef struct Board
{
    Piece *piecesHashTable[64]; // A hashTable that uses the pos of a piece as key an stores the pointer to the piece as value, notice it actually stores the address of the pieces, which are first defined in the pAllPieces pointerarray
    // This hashtable comes in handy, cuz it improves performance when checking for a piece at a boardPos
    PieceData *headPieceData; // The head of the linked list storing all the data of the pieces on the board //It's best to set this to null after initializing the board, to avoid filling it with garbage, which would then make the AddPiece() not work
    PieceData *lastPieceDataStored; // The last element in the headPieceData list making it easier to add pieces to the list
} Board;

// Function that should always be called to initialize a board, as it sets up certain needed things for the chessBoard
Board *InitBoard();

// Returns the piece at given pos on the given board using its piecesHashTable
// If there is no piece at the given pos, returned piece will be NULL
Piece *GetPieceAtPos(Pos pos, Board *board);

// Updates the key in the hashTable of given board for given piece
// This should be called after changing the position of a piece
void UpdatePiecePosOnBoard(Piece *pieceToUpdate, Pos oldPos, Board *board);

/* PieceData linked list functions */
// Adds a piece with given properties to the given board, added piece will be returned as pointer
Piece *AddPiece(PieceType pieceType, PieceColor pieceColor, Pos pos, Board *board);

// Removes given Piece from given board (hashtable and pieceData linked list), returns 0 on success, 1 on fail
int RemovePiece(Piece *pieceToRemove, Board *board);

// This function will free all the parts of the chessBoard freeing is required for and make sure everything allocated is properly deleted
void DeleteChessBoard(Board **board);

/* Debug */
// Following functions only exist and are only used, to check wether this code actually works as expected, aka to debug

// Simple function to loop through the whole linked list of pieceData on given board and simply print out each of the contained elements
void D_PrintPieceDataList(Board *board);

// Prints all information about given piece
void D_PrintPiece(Piece *piece, char name[]);

#endif