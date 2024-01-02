#include <stdio.h>
#include "hf/CCBoard.h"
#include "hf/FEN.h"
#include "hf/CCGeneralGameInfo.h"


int main()
{
    // printf("hi");
    // GameInfo gameInfo = readFENData("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    // printf("%s \n", gameInfo.currentPosAsFENString);
    // printf("%d", gameInfo.currentTurn);

    Board chessBoard;
    chessBoard.headPieceData = NULL; // To make sure there aint garbage

    Pos pos = {1,2};

    AddPiece(Pawn, White, pos, &chessBoard);
    Piece *piece2 = AddPiece(Pawn, Black, pos, &chessBoard);
    AddPiece(Bishop, White, pos, &chessBoard);
    Piece *piece4 = AddPiece(Bishop, Black, pos, &chessBoard);

    RemovePiece(piece2, &chessBoard);
    RemovePiece(piece4, &chessBoard);

    D_PrintPieceDataList(&chessBoard);
    
    return 0;
}








