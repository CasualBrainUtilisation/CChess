#ifndef CCFEN
#define CCFEN

#include "CChessGame.h"

// Read the data from any FEN and return game from it, or return NULL to indicate an invalid FEN
ChessGame *GetGameFromFEN(char FEN[]);

// Converts the given ChessGame to a FEN string literal and sets the FENToChange to it (wow such gorgeus function!)
void ConvertGameToFEN(char FENToReturn[128], ChessGame *chessGameToConvert);

#endif