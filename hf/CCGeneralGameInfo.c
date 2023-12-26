/* Contains structs to keep track of some data required to play a chessGame */


/* Castling */
// The current state of the castling rights
enum CastlingRights
{
    BothSides,
    QueenSide,
    KingSide,
    None
};

// Keeps track of the castling rights for both colors in a game
typedef struct GameCastlingRights 
{
    enum CastlingRights whiteCastlingRights;
    enum CastlingRights blackCastlingRights;
} GameCastlingRights;


/* Game Info */
// Geral Info about a chess game
typedef struct GameInfo
{
    char currentPosAsFENString[128]; // The current boardPosition as FEN
    enum PieceColor currentTurn; // Color that currently is on turn
    GameCastlingRights gameCastlingRights; // The current castling rights for the ongoing game
    Pos possibleEnPassantDestination; // The field that a pawn had to move to, to perform an en passant move in the current state of game
    short playedHalfMoves; // Amount of moves since last capture or pawn moves (Used for draw rules)
    short nextMoveNumber; // The number of the next move to be performed
} GameInfo;