#ifndef CCFEN
#define CCFEN

#include "CChessGame.h"

// Read the data from any FEN and return game from it, or return NULL to indicate an invalid FEN
ChessGame *GetGameFromFEN(char FEN[]);

#endif