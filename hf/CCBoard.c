#include <stdlib.h>
#include <stdio.h>
#include "CCPieces.h"
#include "CCMoves.h"


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
void updatePiecePosInHashTable(Piece *pieceToUpdate, Pos oldPos, Board *board)
{
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

/* Moves */
// Following functions will all be related to moves

// Performs given move on given board
void PerformMove(Move *moveToPerform, Board *board)
{
    // First remove the piece to capture
    if (moveToPerform->PieceToCapture != NULL)
    {
        RemovePiece(moveToPerform->PieceToCapture, board);
    }

    // Store the oldPos of the piece, so we can change its pos in the piecesHashTable later on
    Pos oldPos = moveToPerform->PieceToMove->pos;
    // Move pieceToMove to target pos, 
    moveToPerform->PieceToMove->pos = moveToPerform->moveTargetPos;
    // Now update its pos in the piecesHashTable
    updatePiecePosInHashTable(moveToPerform->PieceToMove, oldPos, board);
}


/* Getting Moves */

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

// Function that should be used to intialize a MoveDataLinkedList (makes sure pointers are NULL)
MoveDataLinkedList *initMoveDataLinkedList()
{
    MoveDataLinkedList *newList = malloc(sizeof(MoveDataLinkedList));

    newList->Head = NULL;
    newList->LastElement = NULL;

    newList->listSize = 0;

    return newList;
}

// add a given move to the moveDataList
void addMoveToMoveDataLinkedList(Move moveToAdd, MoveDataLinkedList *moveDataLinkedList)
{
    // Create the newMoveData
    MoveData *newMoveData = malloc(sizeof(MoveData));
    // Fill the moveToAdd given to this function
    newMoveData->data = moveToAdd;
    // Set the Next to null, as this element will be the last one on the linked list
    newMoveData->Next = NULL;

    if (moveDataLinkedList->Head == NULL) // If the linked MoveData list is currently empty (which it is if the head is null), make this newMoveData be the first element 
    {
        moveDataLinkedList->Head = newMoveData;
        moveDataLinkedList->LastElement = moveDataLinkedList->Head;
    }
    else // In case there already are elements in the moveDataList
    {
        // Make the previous last element point to this new one
        moveDataLinkedList->LastElement->Next = newMoveData;
        // Store the new element in the lastPieceDataStored pointer
        moveDataLinkedList->LastElement = newMoveData;
    }

    // Also increase the listSize variable, as we just increased the size
    moveDataLinkedList->listSize++;
}

// Merges to moveDataLinkedList in given order
// The list should then be referenced by the headList
void mergeMoveDataLinkedList(MoveDataLinkedList *headList, MoveDataLinkedList *finalList)
{
    // In case the headList is empty, make it point to the finalList Head
    if (headList->Head == NULL)
    {
        headList->Head = finalList->Head;
        headList->LastElement = finalList->LastElement;
    }
    // In any other cases simply make the lastElement in the headList point to the first (head) in the finalList
    else
    {
        headList->LastElement->Next = finalList->Head;
    }
    
    // Also add the listSizes together
    headList->listSize += finalList->listSize;
}

// Deletes given moveDataLinkedList data alongside freeing everything in there that we mallocated
// Must be called to delete any MoveDataLinkedList (aslong as you actually wanna avoid memory leaks that is)
void DeleteMoveDataLinkedList(MoveDataLinkedList **moveDataLinkedListToDelete)
{
    // Simply loop through the moveDataLinked list and free every element (while storing the next element in nextMoveDataToDelete)
    for (MoveData *moveDataToDelete = (*moveDataLinkedListToDelete)->Head, *nextMoveDataToDelete = NULL; moveDataToDelete != NULL; moveDataToDelete = nextMoveDataToDelete)
    {
        // We set the nextMoveDataToDelete after the for params, as else we would also set it to moveDataToDelete->Next when moveDataToDelete is actually NULL (we need to check moveDataToDelete for beeing NULL first)
        nextMoveDataToDelete = moveDataToDelete->Next;

        free(moveDataToDelete);
        moveDataToDelete = NULL;
    }

    // Also free the moveDataLinkedListToDelete itself and make it point to NULL
    free(*moveDataLinkedListToDelete);
    *moveDataLinkedListToDelete = NULL;
}

// Returns all the moves avaible for given piece on given board on line with given MoveDir
MoveDataLinkedList *getLineMoves(MoveDir moveDir, Piece *pieceToGetMovesFor, Board *board)
{
    MoveDataLinkedList *moveList = initMoveDataLinkedList();

    // Loop through given line starting from the piece pos
    for (   int
            x = pieceToGetMovesFor->pos.X + moveDir.X, // Make sure we start not at the start x but already go one step in the moveDir (else we'll check for a move from the starting to the starting square)
            y = pieceToGetMovesFor->pos.Y + moveDir.Y; // Make sure we start not at the start y but already go one step in the moveDir (else we'll check for a move from the starting to the starting square)
            x <= 7 && x >= 0 && y <= 7 && y >= 0;  // Check wether x and y are on the board to run this for loop
            x += moveDir.X, y += moveDir.Y) // Go further in the moveDir
    {
        // Get the pos at the current index in the loop
        Pos curPos = {x, y};
        // Get the piece at the current Pos
        Piece *pieceAtCurPos = GetPieceAtPos(curPos, board);
        // Create the move from the pieceToGetMovesFor to the curPos
        Move curMove = {pieceToGetMovesFor->pos, curPos, pieceToGetMovesFor, NULL};
        // Check wether there actually is a piece at the currentPosition
        if (pieceAtCurPos != NULL)
        {
            // Check wether the pieceAtCurPos has a different color then the pieceToGetMovesFor, if so, add this move and set the pieceToCapture to the pieceAtCurPos, as it can be captured
            if (pieceAtCurPos->pieceColor != pieceToGetMovesFor->pieceColor)
            {
                curMove.PieceToCapture = pieceAtCurPos;
                addMoveToMoveDataLinkedList(curMove, moveList);
            }
            // As there is a piece here, we can't go further into this direction, so break;
            break;
        }

        // Add the curMove to the moveList
        addMoveToMoveDataLinkedList(curMove, moveList);
    }

    return moveList;
}

// Checks moves with given offsets from pos of given piece on given board for validity and only returns those moves that are valid
MoveDataLinkedList *checkMoveOffsetsForValidity(Pos possibleOffsetPos[], int possibleOffsetPosCount, Piece *pieceToCheckMovesFor, Board *board)
{
    // All the valid moves will be added to this list (and then returned)
    MoveDataLinkedList *moveList = initMoveDataLinkedList();

    for (int i = 0; i < possibleOffsetPosCount; i++ )
    {
        // Get the pos at the current index in the loop
        Pos curPos = {pieceToCheckMovesFor->pos.X + possibleOffsetPos[i].X, pieceToCheckMovesFor->pos.Y + possibleOffsetPos[i].Y};
        
        // If this move would actually lead of board continue with the next one
        if (curPos.X < 0 || curPos.X > 7 || curPos.Y < 0 || curPos.Y > 7) continue;


        // Create the move from the pieceToCheckMovesFor to the curPos
        Move curMove = {pieceToCheckMovesFor->pos, curPos, pieceToCheckMovesFor, NULL};
        
        // Get the piece at the current Pos
        Piece *pieceAtCurPos = GetPieceAtPos(curPos, board);
        // Check wether there actually is a piece at the currentPosition
        if (pieceAtCurPos != NULL)
        {
            // Check wether the pieceAtCurPos has a different color then the pieceToCheckMovesFor, if so, add this move and set the pieceToCapture to the pieceAtCurPos, as it can be captured
            if (pieceAtCurPos->pieceColor != pieceToCheckMovesFor->pieceColor)
            {
                curMove.PieceToCapture = pieceAtCurPos;
                addMoveToMoveDataLinkedList(curMove, moveList);
            }
            else // If the piece has the same color as the pieceToCheckMovesFor this move is invalid, so continue with the next one
            {
                continue;
            }
        }

        // Add the curMove to the moveList
        addMoveToMoveDataLinkedList(curMove, moveList);
    }

    // Return the moves we got througout this function
    return moveList;
}

// Gives back all the possible moves for given piece
MoveDataLinkedList *GetAllMovesForPiece(Piece *pieceToGetMovesFor, Board *board)
{
    // List to keep track of all the moves we get throughout this function
    MoveDataLinkedList *moveList = initMoveDataLinkedList();

    // Obviously depending on type, move generation will work completly different for different pieces
    switch (pieceToGetMovesFor->pieceType)
    {
        case Pawn:
            break;

        case Knight:

            // The Knight can offset into some fun L-directions, here we get these possible offsets into a simple list
            MoveDir KnightMoveOffsets[] = {{1,2}, {-1,2}, {2,1}, {-2,1}, {2,-1}, {-2,-1}, {-1,-2}, {1,-2}};

            // Now we check this offsets for representing valid moves, as checkMoveOffsetsForValidity() returns these valid moves, we simpoly merge them to the moveList
            mergeMoveDataLinkedList(moveList, checkMoveOffsetsForValidity(KnightMoveOffsets, sizeof(KnightMoveOffsets)/(sizeof(KnightMoveOffsets[0])), pieceToGetMovesFor, board));

            break;

        case Bishop:

            // The bishop can move in diagonal directions, so this is just a list of that, every diagonal direction, we'll loop through to get the moves for each dir
            MoveDir diagonalMoveDirs[] = {{1,1}, {-1,1}, {-1,-1}, {1,-1}};

            // Loop through all the moveDirs and get according lineMoves
            for (int i = 0; i < 4; i++)
            {
                // Add the line moves of the current moveDir to the moveList
                mergeMoveDataLinkedList(moveList, getLineMoves(diagonalMoveDirs[i], pieceToGetMovesFor, board));
            }


            break;

        case Rook:

            // The Rook can move in straight directions, so this is just a list of that, every straight direction, we'll loop through to get the moves for each dir
            MoveDir straightMoveDir[] = {{0,1}, {1,0}, {-1,0}, {0,-1}};

            // Loop through all the moveDirs and get according lineMoves
            for (int i = 0; i < 4; i++)
            {
                // Add the line moves of the current moveDir to the moveList
                mergeMoveDataLinkedList(moveList, getLineMoves(straightMoveDir[i], pieceToGetMovesFor, board));
            }

            break;

        case Queen:

            // The Queen can move into every direction, so this is just a list of that, every direction, we'll loop through to get the moves for each dir
            MoveDir QueenMoveDirs[] = {{1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1}, {0,-1}, {1,-1}};

            // Loop through all the moveDirs and get according lineMoves
            for (int i = 0; i < 8; i++)
            {
                // Add the line moves of the current moveDir to the moveList
                mergeMoveDataLinkedList(moveList, getLineMoves(QueenMoveDirs[i], pieceToGetMovesFor, board));
            }

            break;
        
        case King: //TODO: castling

            // The King can offset into every direction, here we get these possible offsets into a simple list
            MoveDir KingMoveOffsets[] = {{1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1}, {0,-1}, {1,-1}};

            // Now we check this offsets for representing valid moves, as checkMoveOffsetsForValidity() returns these valid moves, we simpoly merge them to the moveList
            mergeMoveDataLinkedList(moveList, checkMoveOffsetsForValidity(KingMoveOffsets, sizeof(KingMoveOffsets)/(sizeof(KingMoveOffsets[0])), pieceToGetMovesFor, board));

            break;
    }

    // If we didn't find any moves we'll return this information (NULL)
    if (moveList->listSize == 0) return NULL;

    // Else just return the moves we found
    return moveList;
}
