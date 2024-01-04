#include "CCBoard.h"
#include "CCPieces.h"

/* Contains structs to keep track of some data required to play a chessGame, this data is actually just everything found in a FEN string plus the game history */

/* Castling */
// The current state of the castling rights
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


/* ChessGame struct */
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
ChessGame *InitChessGame()
{
    // Allocate memory for the ChessGame struct
    ChessGame *chessGame = malloc(sizeof(ChessGame)); // I am aware I could use calloc instead and spare a bunch of zeroing that follows this, still decided against doing so //TODO: probably ask the simple question WHY? and use calloc lmao

    // Init the board of the chessGame
    chessGame->board = InitBoard();

    // Set the currenTurn to white on default
    chessGame->currentTurn = White;

    // Set the gameCastlingRights for white and black to be None on default
    chessGame->gameCastlingRights.blackCastlingRights = None;
    chessGame->gameCastlingRights.whiteCastlingRights = None;

    // Set the possibleEnPassantDestinationPos to be NULL on default, indicating that there is no possibleEnPassantDestinationPos avaible rn
    chessGame->possibleEnPassantDestinationPos = NULL;

    // Set the playedHalfMoves to be 0 on default
    chessGame->playedHalfMoves = 0;
    // Set the nextMoveNumber to be 1 on default
    chessGame->nextMoveNumber = 1;

    // Finally return created chessGame
    return chessGame; 
}

// This function will free every part of given chessGame freeing is required for aka everything malloc() was used for
void DeleteChessGame(ChessGame **chessGame)
{
    // Delete the stupid chessBoard
    DeleteChessBoard(&(*chessGame)->board);

    // Set the possibleEnPassantDestinationPos pointer to NULL
    (*chessGame)->possibleEnPassantDestinationPos = NULL;

    // Finally free the chessGame itself
    free(*chessGame);
    *chessGame = NULL;
}