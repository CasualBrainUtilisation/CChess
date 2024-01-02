#include <stdlib.h>
#include "CCPieces.h"


/* Contains the ChessBoard and Pos struct (which represents a pos on the chessboard) and also all the functions that impact or require the ChessBoard struct */

/* Hashtable functions */
// Simple function that'll return the hash of inputed pos
// ALL poses have a unique hash
int getHash(Pos pos)
{
    return pos.X * 8 - pos.Y;
}

// Returns the piece at given pos on the given board using its piecesHashTable
// If there is no piece at the given pos, returned piece will be NULL
Piece *GetPieceAtPos(Pos pos, Board *board)
{
    return board->piecesHashTable[getHash(pos)];
}

// Linked list functions
void addPieceToPieceData(Piece *pieceToAdd, Board *board)
{
    PieceData newPieceData = {&pieceToAdd, NULL};
    board->lastPieceDataStored->Next = &newPieceData;
}