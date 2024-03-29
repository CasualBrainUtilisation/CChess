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
Board *InitBoard()
{
    // For first create the board on the heap
    Board *board = malloc(sizeof(Board));
    // Must be NULL on default, which is needed for creating the list when adding the first piece, this makes sure it isn't filled with garbage
    board->headPieceData = NULL; 

    // Also make sure all the elements of the hashTable are NULL on default (as that is what represents that there is no piece here)
    for (int i = 0; i < 64; i++)
    {
        board->piecesHashTable[i] = NULL;
    }

    return board;
}

/* Hashtable functions */
// Simple function that'll return the hash of inputed pos, those should though go from 0-7 (else we'll have collisions (which are not handled by the piecesHashTable))
// ALL poses from 0|0 to 7|7 have a unique hash
int getHash(Pos pos)
{
    return pos.X * 8 + pos.Y;
}

// Returns the piece at given pos on the given board using its piecesHashTable
// If there is no piece at the given pos, returned piece will be NULL
Piece *GetPieceAtPos(Pos pos, Board *board)
{
    return board->piecesHashTable[getHash(pos)];
}

// Updates the key in the hashTable of given board for given piece
// This should be called after changing the position of a piece
void UpdatePiecePosOnBoard(Piece *pieceToUpdate, Pos oldPos, Board *board)
{
    printf("Setting piece to NULL at %d|%d \n", oldPos.X, oldPos.Y);
    // Remove piece from old key value
    board->piecesHashTable[getHash(oldPos)] = NULL;

    // Check wether we're overwriting a piece, if so prompt that
    if (board->piecesHashTable[getHash(pieceToUpdate->pos)] != NULL) printf("Overwriting old piece at (%d|%d) on the piecesHashTable, you are moving a piece on a square that already contains one \n", pieceToUpdate->pos.X, pieceToUpdate->pos.Y);

    // Add piece to the new key value
    board->piecesHashTable[getHash(pieceToUpdate->pos)] = pieceToUpdate;
}

/* Linked List functions */
// add a given piece to the PieceDataList
void addPieceToPieceData(Piece *pieceToAdd, Board *board)
{
    // Create the newPieceData
    PieceData *newPieceData = malloc(sizeof(PieceData));
    // Fill the pieceToAddDate given to this function
    newPieceData->data = pieceToAdd;
    // Set the Next to null, as this element will be the last one on the linked list
    newPieceData->Next = NULL;

    if (board->headPieceData == NULL) // If the linked PieceData list is currently empty (which it is if the head is null), make this newPieceData be the first element 
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

// Adds a piece with given properties to the given board, added piece will be returned as pointer
Piece *AddPiece(PieceType pieceType, PieceColor pieceColor, Pos pos, Board *board)
{
    // Create the new piece
    Piece *newPiece = malloc(sizeof(Piece));
    // Fill in given data
    newPiece->pieceColor = pieceColor;
    newPiece->pieceType = pieceType;
    newPiece->pos = pos;

    // Add the newPiece to the PieceData list, so its referenced on the board
    addPieceToPieceData(newPiece, board);

    // Also add the new Piece to the piecesHashTable
    // We'll quickly check wether collision occurs here, which then would make no sense
    if (board->piecesHashTable[getHash(pos)] != NULL) printf("Overwriting old piece at (%d|%d) on the piecesHashTable, there should only ever be one piece at a specific pos \n", pos.X, pos.Y);
    board->piecesHashTable[getHash(pos)] = newPiece;

    // Now return the added piece
    return newPiece;
}

// Removes given piece from the pieceData linked list of given board, returns 0 on success, 1 on fail
int removePieceFromPieceData(Piece *pieceToRemove, Board *board)
{
    // To do this we'll loop through the pieceData linked list, til reaching given pieceToRemove
    // curElementOfPieceDataList keeps track of the element currently inspected in the loop, at first this will be set to the head (1st ele) of the linked list
    PieceData *curElementOfPieceDataList = board->headPieceData;
    // The previousElementOfPieceDataList keeps track of the element previously inspected in the loop on last call
    PieceData *previousElementOfPieceDataList = curElementOfPieceDataList;

    while(curElementOfPieceDataList != NULL)
    {
        // Check wether we have reached the PieceToRemove, if so remove the piece from the list
        if (curElementOfPieceDataList->data == pieceToRemove)
        {   
            // If the pieceToRemove is the head of pieceData, we need to change the head to the 2nd ele in the linked list, leading to a different procedure then normal
            if (curElementOfPieceDataList == board->headPieceData)
            {
                board->headPieceData = curElementOfPieceDataList->Next;
            }
            else
            {
                // Set the Next of the previous element to the next of the current, so the current is skipped in the linked lists and already removed from the list (not from mem)
                previousElementOfPieceDataList->Next = curElementOfPieceDataList->Next;
            }

            // After this curElementOfPieceDataList simply needs to be freed to also remove it from memory and the pointer will be set to NULL (this is unecessary but good practice)
            free(curElementOfPieceDataList);
            curElementOfPieceDataList = NULL;

            return 0; //We can end this function now, the rest is not needed to run anymore, as the pieceToRemove has already been removed, return 0 to indicate sucess
        }

        // Move on to the next element foreach pointer
        previousElementOfPieceDataList = curElementOfPieceDataList;
        curElementOfPieceDataList = curElementOfPieceDataList->Next;
    }

    // When this is called, we went through the whole loop without finding the pieceToRemove, so print a lil notice and return 1; 
    printf("Piece To Remove not found in linked list board->headPieceData");
    return 1;
}

// Removes given Piece from given board (hashtable and pieceData linked list), returns 0 on success, 1 on fail
int RemovePiece(Piece *pieceToRemove, Board *board)
{
    // First remove the piece from the pieceData linked list
    int sucess = removePieceFromPieceData(pieceToRemove, board);
    // As we won't check wether it makes sense to remove the piece from the hashTable, this will depend on success of the removage from the linked list
    if (sucess == 0)
    {
        // Remove the piece from the piecesHashTable (aka set its hash index (value) to NULL)
        board->piecesHashTable[getHash(pieceToRemove->pos)] = NULL;
    }

    // Also free the pieceToRemove now, as its been removed from the hashTable and the linked pieceData list (and set it to NULL)
    free(pieceToRemove);
    pieceToRemove = NULL;

    return sucess;
}

// This function will free all the parts of the chessBoard freeing is required for and make sure everything allocated is properly deleted
void DeleteChessBoard(Board **board)
{
    // For this purpose we'll loop through the whole piece Data linked list of the board and free every single part of it, aswell as stored pieces
    // curElementOfPieceDataList keeps track of the cur element of the linked list referenced in the loop, obviously it starts at the beginning of the linked list, so we set it to the head of pieceData
    PieceData *curElementOfPieceDataList = (*board)->headPieceData;

    while(curElementOfPieceDataList != NULL)
    {
        // Store the element we want to delete in a seperate pointer
        PieceData *pieceDataToDelete = curElementOfPieceDataList;
        // Change the curElement to the next as long as we still have access to the current
        curElementOfPieceDataList = curElementOfPieceDataList->Next;

        // Start by freeing the data part (which is the stored piece)
        free(pieceDataToDelete->data);
        pieceDataToDelete->data = NULL;

        // Now free the pieceData itself
        free(pieceDataToDelete);
        pieceDataToDelete = NULL;
    }

    // Also set all these pointers to be null now
    (*board)->headPieceData = NULL;
    (*board)->lastPieceDataStored = NULL;

    // At last free the board itself
    free(*board);
    *board = NULL;
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
        printf("*********************************   Element %d   ***********************************\n", i);

        printf("PieceColor (White : 0, Black : 1) :                                           %d \n", curElementOfPieceDataList->data->pieceColor);
        printf("PieceType (Pawn : 0, Horsey : 1, Bishop : 2, Rook : 3, Queen : 4, King : 5) : %d \n", curElementOfPieceDataList->data->pieceType);
        printf("Pos :                                                                        (%d|%d) \n", curElementOfPieceDataList->data->pos.X, curElementOfPieceDataList->data->pos.Y);

        printf("\n");

        curElementOfPieceDataList = curElementOfPieceDataList->Next;

        i++;
    }
}

// Prints all information about given piece
void D_PrintPiece(Piece *piece, char name[])
{   
    printf("*********************************   Element %s   ***********************************\n", name);

    printf("PieceColor (White : 0, Black : 1) :                                           %d \n", piece->pieceColor);
    printf("PieceType (Pawn : 0, Horsey : 1, Bishop : 2, Rook : 3, Queen : 4, King : 5) : %d \n", piece->pieceType);
    printf("Pos :                                                                        (%d|%d) \n", piece->pos.X, piece->pos.Y);
    
    printf("\n");
}