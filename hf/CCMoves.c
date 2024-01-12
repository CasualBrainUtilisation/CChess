#include <stdlib.h>

#include "CCPieces.h" // Piece and Pos are required
#include "CChessGame.h" // Information bout castling and enPassant required

/* Everything to work with chess moves */

// The different types of chess moves, to be easily defined in an enum, this is later used in the PerformMove() function to easily act different depending on moveType
typedef enum MoveType
{
    Default, // A default chess move
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

// Performs given move on given board
void PerformMove(Move *moveToPerform, ChessGame *chessGame)
{
    // First remove the piece to capture
    if (moveToPerform->PieceToCapture != NULL)
    {
        RemovePiece(moveToPerform->PieceToCapture, chessGame->board);
    }

    // Store the oldPos of the piece, so we can change its pos in the piecesHashTable later on
    Pos oldPos = moveToPerform->PieceToMove->pos;
    // Move pieceToMove to target pos, 
    moveToPerform->PieceToMove->pos = moveToPerform->moveTargetPos;
    // Now update its pos in the piecesHashTable
    UpdatePiecePosOnBoard(moveToPerform->PieceToMove, oldPos, chessGame->board);

    // Also set the NewEnPassantDestinationPos to NULL, it will be changed later, in case we are performing a pawnDoubleMove (aka we have to set it to smth)
    SetNewEnPassantDestinationPos(NULL, chessGame);

    // Depending on the move type, we might have to take some additional steps other then just performing given move info
    switch (moveToPerform->moveType) //TODO: Make Castling not work even if rights are present when it just ain't possible (prob remove this in gameLoading though)
    {
        // In case it's a castleLong move, we'll have to also move the rook on the left side 3 squares to the right and we need to remove se CastlingRights
        case CastleLong:
        
        // Get the rook on the left corner
        Piece *castleLongRookToMove = GetPieceAtPos((Pos){0, moveToPerform->PieceToMove->pos.Y}, chessGame->board);

        // Change its position
        castleLongRookToMove->pos = (Pos){3, moveToPerform->PieceToMove->pos.Y};

        // Now update its position on board
        UpdatePiecePosOnBoard(castleLongRookToMove, (Pos){0, moveToPerform->PieceToMove->pos.Y}, chessGame->board);

        // Finally remove the CastleLong castling rights from the color of the piece we just moved (aka set them to NONE)
        if (moveToPerform->PieceToMove->pieceColor == White) chessGame->gameCastlingRights.whiteCastlingRights = None;
        else chessGame->gameCastlingRights.blackCastlingRights = None;

        break;

        // In case it's a castleShort move, we'll have to also move the rook on the right side 2 squares to the left and we need to remove se CastlingRights
        case CastleShort:
        
        // Get the rook on the right corner
        Piece *rookToMove = GetPieceAtPos((Pos){7, moveToPerform->PieceToMove->pos.Y}, chessGame->board);

        // Change its position
        rookToMove->pos = (Pos){5, moveToPerform->PieceToMove->pos.Y};

        // Now update its position on board
        UpdatePiecePosOnBoard(rookToMove, (Pos){7, moveToPerform->PieceToMove->pos.Y}, chessGame->board);

        // Finally remove the CastleLong castling rights from the color of the piece we just moved (aka set them to NONE)
        if (moveToPerform->PieceToMove->pieceColor == White) chessGame->gameCastlingRights.whiteCastlingRights = None;
        else chessGame->gameCastlingRights.blackCastlingRights = None;

        break;

        // In case it's a doublePawnMove, that makes an en passant possible next move, so set that possible en passant, which as the pawn already moved, would be 1 square down (for white) or 1 square up (for black)
        case DoublePawnMove:

        // The newEnPassantDestinationPos for the next move
        Pos *newEnPassantDestinationPos = malloc(sizeof(Pos));
        // Set the X which obviously is equal to the xPos of the pieceToMove
        newEnPassantDestinationPos->X = moveToPerform->PieceToMove->pos.X;
        // The Y pos though depends on the pieceColor (either gonna be 5 (white) or 2 (black))
        if (moveToPerform->PieceToMove->pieceColor == White) newEnPassantDestinationPos->Y = 5;
        else newEnPassantDestinationPos->Y = 2;

        // Now Set the NewEnPassantDestinationPos to the one defined above
        SetNewEnPassantDestinationPos(newEnPassantDestinationPos, chessGame);

        break;
    }
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
        Move curMove = {pieceToGetMovesFor->pos, curPos, pieceToGetMovesFor, NULL, Default};
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
        if (!IsPosOnBoard(curPos)) continue;


        // Create the move from the pieceToCheckMovesFor to the curPos
        Move curMove = {pieceToCheckMovesFor->pos, curPos, pieceToCheckMovesFor, NULL, Default};
        
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

// Gives back all the possible moves for given piece on given board
MoveDataLinkedList *GetAllMovesForPiece(Piece *pieceToGetMovesFor, ChessGame *chessGame)
{
    // List to keep track of all the moves we get throughout this function
    MoveDataLinkedList *moveList = initMoveDataLinkedList();

    // Obviously depending on type, move generation will work completly different for different pieces
    switch (pieceToGetMovesFor->pieceType)
    {
        case Pawn:

            // Get the Direction on y this pawn can move to, this is either up or down depending on color (notice that yes -1 does actually represent up)
            int yDir = pieceToGetMovesFor->pieceColor == White ? -1 : 1;


            /* Capture on upper left */

            // Destination position for the capture move to the upper left from the pawn
            Pos captureOnLeftPos = {pieceToGetMovesFor->pos.X - 1, pieceToGetMovesFor->pos.Y + yDir};

            // Check wether the captureOnLeftPos actually is on board, else this capture ain't valid so we just go on
            if (IsPosOnBoard(captureOnLeftPos))
            {
                // Get the piece at the captureOnLeftPos (this will be NULL if there ain't a piece there)
                Piece *pieceAtUpperLeft = GetPieceAtPos(captureOnLeftPos, chessGame->board);
                // Check wether there actually is a piece at the captureOnLeftPos and if so check wether it has a different color then the pawn, if so we can capture it, so add this move
                if (pieceAtUpperLeft != NULL && pieceAtUpperLeft->pieceColor != pieceToGetMovesFor->pieceColor)
                {

                    addMoveToMoveDataLinkedList(
                        (Move){pieceToGetMovesFor->pos, captureOnLeftPos, pieceToGetMovesFor, pieceAtUpperLeft, Default},
                        moveList);
                }
                // Else if the captureOnLeftPos has the same x and y values as the possibleEnPassantDestinationPos, meaning you can perform an enPassant here, so add this move
                else if (captureOnLeftPos.X == chessGame->possibleEnPassantDestinationPos->X && captureOnLeftPos.Y == chessGame->possibleEnPassantDestinationPos->Y)
                {
                    addMoveToMoveDataLinkedList(
                        (Move){pieceToGetMovesFor->pos, captureOnLeftPos, pieceToGetMovesFor, GetPieceAtPos((Pos){pieceToGetMovesFor->pos.X - 1, pieceToGetMovesFor->pos.Y}, chessGame->board), Default}, // In this case we'll have to add the pawn at the left side of this pawn as the PieceToCapture (as this is what en passant does)
                        moveList);
                }
            }


            /* Capture on upper right */

            // Destination position for the capture move to the upper right from the pawn
            Pos captureOnRightPos = {pieceToGetMovesFor->pos.X + 1, pieceToGetMovesFor->pos.Y + yDir};

            // Check wether the captureOnRightPos actually is on board, else this capture ain't valid so we just go on
            if (IsPosOnBoard(captureOnRightPos))
            {
                // Get the piece at the captureOnRightPos (this will be NULL if there ain't a piece there)
                Piece *pieceAtUpperRight = GetPieceAtPos(captureOnRightPos, chessGame->board);
                // Check wether there actually is a piece at the captureOnRightPos and if so check wether it has a different color then the pawn, if so we can capture it, so add this move
                if (pieceAtUpperRight != NULL && pieceAtUpperRight->pieceColor != pieceToGetMovesFor->pieceColor)
                {

                    addMoveToMoveDataLinkedList(
                        (Move){pieceToGetMovesFor->pos, captureOnRightPos, pieceToGetMovesFor, pieceAtUpperRight, Default},
                        moveList);
                }
                // Else if the captureOnRightPos has the same x and y values as the possibleEnPassantDestinationPos, you can perform an enPassant here, so add this move
                else if (captureOnRightPos.X == chessGame->possibleEnPassantDestinationPos->X && captureOnRightPos.Y == chessGame->possibleEnPassantDestinationPos->Y)
                {
                    addMoveToMoveDataLinkedList(
                        (Move){pieceToGetMovesFor->pos, captureOnLeftPos, pieceToGetMovesFor, GetPieceAtPos((Pos){pieceToGetMovesFor->pos.X + 1, pieceToGetMovesFor->pos.Y}, chessGame->board), Default}, // In this case we'll have to add the pawn at the right side of this pawn as the PieceToCapture (as this is what en passant does)
                        moveList);
                }
            }


            /* Normal Pawn Moves (1/2 forward) */

            // Get the pos in the yDir of the pawn, this would be the destination pos of a normal single PawnMove
            Pos singlePawnMoveDestinationPos = {pieceToGetMovesFor->pos.X, pieceToGetMovesFor->pos.Y + yDir};
                        
            // Check wether there is not a piece at the singlePawnMoveDestinationPos, if so this move is valid, so add it and check wether a doublePawnMove may be performable
            if (GetPieceAtPos(singlePawnMoveDestinationPos, chessGame->board) == NULL)
            {
                addMoveToMoveDataLinkedList(
                        (Move){pieceToGetMovesFor->pos, singlePawnMoveDestinationPos, pieceToGetMovesFor, NULL, Default},
                        moveList);

                // Check wether the pawn still is on its starting square, if so, given a singlePawnMove move would be possible, maybe a doublePawnMove also could be
                if (pieceToGetMovesFor->pos.Y == pieceToGetMovesFor->pieceColor == White ? 6 : 1)
                {
                    // So get the pos 2 two ahead of the pawn (on the yDir), which would be the destanation of a doublePawnMove
                    Pos doublePawnMoveDestinationPos = {pieceToGetMovesFor->pos.X, pieceToGetMovesFor->pos.Y + yDir * 2};

                    // Now check wether there is a piece at this pos, and if there ain't, add the double forwardPawnMove as it's valid
                    if (GetPieceAtPos(doublePawnMoveDestinationPos, chessGame->board) == NULL)
                    {
                        addMoveToMoveDataLinkedList(
                            (Move){pieceToGetMovesFor->pos, doublePawnMoveDestinationPos, pieceToGetMovesFor, NULL, DoublePawnMove}, // Notice that the moveType is a DoublePawnMove for this move
                            moveList);
                    }
                }
            }


            break;

        case Knight:

            // The Knight can offset into some fun L-directions, here we get these possible offsets into a simple list
            MoveDir KnightMoveOffsets[] = {{1,2}, {-1,2}, {2,1}, {-2,1}, {2,-1}, {-2,-1}, {-1,-2}, {1,-2}};

            // Now we check this offsets for representing valid moves, as checkMoveOffsetsForValidity() returns these valid moves, we simpoly merge them to the moveList
            mergeMoveDataLinkedList(moveList, checkMoveOffsetsForValidity(KnightMoveOffsets, sizeof(KnightMoveOffsets)/(sizeof(KnightMoveOffsets[0])), pieceToGetMovesFor, chessGame->board));

            break;

        case Bishop:

            // The bishop can move in diagonal directions, so this is just a list of that, every diagonal direction, we'll loop through to get the moves for each dir
            MoveDir diagonalMoveDirs[] = {{1,1}, {-1,1}, {-1,-1}, {1,-1}};

            // Loop through all the moveDirs and get according lineMoves
            for (int i = 0; i < 4; i++)
            {
                // Add the line moves of the current moveDir to the moveList
                mergeMoveDataLinkedList(moveList, getLineMoves(diagonalMoveDirs[i], pieceToGetMovesFor, chessGame->board));
            }


            break;

        case Rook:

            // The Rook can move in straight directions, so this is just a list of that, every straight direction, we'll loop through to get the moves for each dir
            MoveDir straightMoveDir[] = {{0,1}, {1,0}, {-1,0}, {0,-1}};

            // Loop through all the moveDirs and get according lineMoves
            for (int i = 0; i < 4; i++)
            {
                // Add the line moves of the current moveDir to the moveList
                mergeMoveDataLinkedList(moveList, getLineMoves(straightMoveDir[i], pieceToGetMovesFor, chessGame->board));
            }

            break;

        case Queen:

            // The Queen can move into every direction, so this is just a list of that, every direction, we'll loop through to get the moves for each dir
            MoveDir QueenMoveDirs[] = {{1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1}, {0,-1}, {1,-1}};

            // Loop through all the moveDirs and get according lineMoves
            for (int i = 0; i < 8; i++)
            {
                // Add the line moves of the current moveDir to the moveList
                mergeMoveDataLinkedList(moveList, getLineMoves(QueenMoveDirs[i], pieceToGetMovesFor, chessGame->board));
            }

            break;
        
        case King:

            // The King can offset into every direction, here we get these possible offsets into a simple list
            MoveDir KingMoveOffsets[] = {{1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1}, {0,-1}, {1,-1}};

            // Now we check this offsets for representing valid moves, as checkMoveOffsetsForValidity() returns these valid moves, we simpoly merge them to the moveList
            mergeMoveDataLinkedList(moveList, checkMoveOffsetsForValidity(KingMoveOffsets, sizeof(KingMoveOffsets)/(sizeof(KingMoveOffsets[0])), pieceToGetMovesFor, chessGame->board));


            /* Castling Moves */

            // Get the castlingRights for the color of the PieceToGetMovesFor
            CastlingRights castlingRightsForPieceToGetMovesFor = pieceToGetMovesFor->pieceColor == White ? chessGame->gameCastlingRights.whiteCastlingRights : chessGame->gameCastlingRights.blackCastlingRights;

            /* QueenSide */
            // Check wether the piece still has the rights to perform a QueenSide castle
            if (castlingRightsForPieceToGetMovesFor == QueenSide || castlingRightsForPieceToGetMovesFor == BothSides)
            {
                // Now check wether the three squares left to the king are emtpy, if so, we can perform a QueenSideCastle, so add given move (which is 2 to the left; PerformMove() will have to worry about getting corresponding rook)
                if (GetPieceAtPos((Pos){pieceToGetMovesFor->pos.X - 1, pieceToGetMovesFor->pos.Y}, chessGame->board) == NULL &&
                    GetPieceAtPos((Pos){pieceToGetMovesFor->pos.X - 2, pieceToGetMovesFor->pos.Y}, chessGame->board) == NULL &&
                    GetPieceAtPos((Pos){pieceToGetMovesFor->pos.X - 3, pieceToGetMovesFor->pos.Y}, chessGame->board) == NULL)
                {
                    addMoveToMoveDataLinkedList(
                        (Move){pieceToGetMovesFor->pos, (Pos){pieceToGetMovesFor->pos.X - 2, pieceToGetMovesFor->pos.Y}, pieceToGetMovesFor, NULL, CastleLong}, //For the PerformMove() function to know what to with this move (aka move the rook too not just the king) we need the additional information of this move beeing a CastleLong
                        moveList);
                }
            }

            /* KingSide */
            // Check wether the piece still has the rights to perform a kingside castle
            if (castlingRightsForPieceToGetMovesFor == KingSide || castlingRightsForPieceToGetMovesFor == BothSides)
            {
                // Now check wether the two squares right to the king are emtpy, if so, we can perform a KingSideCastle, so add given move (which is 2 to the right; PerformMove() will have to worry about getting corresponding rook)
                if (GetPieceAtPos((Pos){pieceToGetMovesFor->pos.X + 1, pieceToGetMovesFor->pos.Y}, chessGame->board) == NULL &&
                    GetPieceAtPos((Pos){pieceToGetMovesFor->pos.X + 2, pieceToGetMovesFor->pos.Y}, chessGame->board) == NULL)
                {
                    addMoveToMoveDataLinkedList(
                        (Move){pieceToGetMovesFor->pos, (Pos){pieceToGetMovesFor->pos.X + 2, pieceToGetMovesFor->pos.Y}, pieceToGetMovesFor, NULL, CastleShort}, //For the PerformMove() function to know what to with this move (aka move the rook too not just the king) we need the additional information of this move beeing a CastleShort
                        moveList);
                }
            }

            break;
    }

    // If we didn't find any moves we'll return this information (NULL)
    if (moveList->listSize == 0) return NULL;

    // Else just return the moves we found
    return moveList;
}