#ifndef CChessGame
#define CChessGame

#include "CCBoard.h"
#include "CCPieces.h"

/* Contains structs to keep track of some data required to play a chessGame, this data is actually just everything found in a FEN string plus the game history */

/* Castling */
// The current state of the castling rights
// There actually is a beautiful way of adding castlingRights, (mathematically) add the current one (None  + Queenside = QueenSide / KingSide + QueenSide = Both), and removing (subtract it (BothSides - KingSide = QueenSide))
typedef enum CastlingRights
{
    None,
    QueenSide,
    KingSide,
    BothSides
} CastlingRights;

// Keeps track of the castling rights for both colors in a game
typedef struct GameCastlingRights 
{
    CastlingRights whiteCastlingRights;
    CastlingRights blackCastlingRights;
} GameCastlingRights;


/* Game Info */
// Geral Info about a chess game
typedef struct ChessGame //TODO: add gameHistory
{
    Board *board; // The board storing current game position
    PieceColor currentTurn; // Color that currently is on turn
    GameCastlingRights gameCastlingRights; // The current castling rights for the ongoing game
    Pos *possibleEnPassantDestinationPos; // The field that a pawn had to move to, to perform an en passant move in the current state of game (most of the time NULL, that's why it's a pointer)
    int playedHalfMoves; // Amount of moves since last capture or pawn moves (Used for draw rules)
    int nextMoveNumber; // The number of the next move to be performed
} ChessGame;

// Function that should always be called to initialize a chessGame as it sets up some things needed for the board to function expectedly (mainly zeroing and malloc())
ChessGame *InitChessGame();

// Simple function to add given castlingRightToAdd to given castlingRightToAddTo enum, returns 1 on given castling right beeing invalid to add, 0 on sucess
int AddCastlingRight(CastlingRights castlingRightToAdd, CastlingRights *castlingRightToAddTo);

// Simple function to set a new EnPassantDestinationPos, it frees the previous pos if there was one
void SetNewEnPassantDestinationPos(Pos *newPossibleEnPassantDestinationPos, ChessGame *chessGame);

// This function will free every part of given chessGame freeing is required for aka everything malloc() was used for
void DeleteChessGame(ChessGame **chessGame);

#endif