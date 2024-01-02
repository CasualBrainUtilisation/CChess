#include <stdlib.h>
#include <stdio.h>
#include "CCPieces.h"


/* Contains the ChessBoard and all the functions that impact or require the ChessBoard struct */

// Struct for the linked list defined on board
typedef struct PieceData
{
    Piece *data; // The data of stored piece
    struct PieceData *Next; // The next element in the linked list
} PieceData;

// Struct containing info about a chessBoard aka thee pieces placed on top
typedef struct Board
{
    Piece *piecesHashTable[64]; // A hashTable that uses the pos of a piece as key an stores the pointer to the piece as value, notice it actually stores the address of the pieces, which are first defined in the pAllPieces pointerarray
    // This hashtable comes in handy, cuz it improves performance when checking for a piece at a boardPos //TODO: either give use or remove
    PieceData *headPieceData; // The head of the linked list storing all the data of the pieces on the board
    PieceData *lastPieceDataStored; // The last element in the headPieceData list making it easier to add pieces to the list
} Board;

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
// add a piecee to the PieceDataList
void addPieceToPieceData(Piece *pieceToAdd, Board *board)
{
    // Create the newPieceData
    PieceData *newPieceData = malloc(sizeof(PieceData));
    // Fill the pieceToAddDate given to this function
    newPieceData->data = pieceToAdd;
    // Set the Next to null, as this element will be the last one on the linked list
    newPieceData->Next = NULL;

    if (board->headPieceData == NULL) // If The linked PieceData list is currently empty (which it is if the head is null), make this newPieceData be the first element 
    {
        board->headPieceData = newPieceData;
        board->lastPieceDataStored = board->headPieceData;
    }
    else // In case there already are elements in the pieceDataList
    {
        // Make the previous last element point to this new one
        board->lastPieceDataStored->Next = newPieceData;
        // Store the new element in the lastPieceDataStored pointer
        board->lastPieceDataStored = newPieceData;
    }
    
}

// Adds a piece with given properties to the given board
void AddPiece(PieceType pieceType, PieceColor pieceColor, Pos pos, Board *board)
{
    // Create the new piece
    Piece *newPiece = malloc(sizeof(Piece));
    // Fill in given data
    newPiece->pieceColor = pieceColor;
    newPiece->pieceType = pieceType;
    newPiece->pos = pos;
    // Add the newPiece to the PieceData list, so its referenced on the board
    addPieceToPieceData(newPiece, board);
}

/* Debug */
// Following functions only exist and are only used, to check wether this code actually works as expected, aka to debug

// Simple function to loop through the whole linked list of pieceData on given board and simply print out each of contained elements
void D_PrintPieceDataList(Board *board)
{
    // curElementOfPieceDataList keeps track of the cur element of the linked list referenced in the loop, obviously it starts at the beginning of the linked list, so we set it to the head of pieceData
    PieceData *curElementOfPieceDataList = board->headPieceData;

    int i = 0;
    while(curElementOfPieceDataList != NULL)
    {
        printf("** Element %d ** \n", i);

        printf("PieceColor (White : 0, Black : 1) : %d \n", curElementOfPieceDataList->data->pieceColor);
        printf("PieceType (Pawn : 0, Horsey : 1, Bishop : 2, Rook : 3, Queen : 4, King : 5) : %d \n", curElementOfPieceDataList->data->pieceType);
        printf("Pos : (%d|%d) \n", curElementOfPieceDataList->data->pos.X, curElementOfPieceDataList->data->pos.Y);

        printf("\n");

        curElementOfPieceDataList = curElementOfPieceDataList->Next;

        i++;
    }
}