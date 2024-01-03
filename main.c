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

    Board *chessBoard = InitBoard();

    Pos pos = {1,2};

    AddPiece(Pawn, White, pos, chessBoard);
    Piece *piece2 = AddPiece(Pawn, Black, pos, chessBoard);
    AddPiece(Bishop, White, pos, chessBoard);
    Piece *piece4 = AddPiece(Bishop, Black, pos, chessBoard);

    Pos pos2 = {2,2};
    AddPiece(Rook, White, pos2, chessBoard);

    RemovePiece(piece2, chessBoard);
    RemovePiece(piece4, chessBoard);

    D_PrintPieceDataList(chessBoard);
    printf("%d \n", GetPieceAtPos(pos2, chessBoard)->pieceType);

    Pos newPos = {0, 0};
    Piece *piece = GetPieceAtPos(newPos, chessBoard);
    if (piece == NULL)
    {
        printf("f \n");
    }

    Pos startPos = {5, 5};
    Pos targetPos = {4, 4};
    Piece *pieceToMove = AddPiece(Queen, White, startPos, chessBoard);
    Piece *pieceToCapture = AddPiece(Queen, Black, targetPos, chessBoard);
    Move move = {startPos, targetPos, pieceToMove, pieceToCapture};
    PerformMove(move, chessBoard);

    Piece *pieceAtTargetPos = GetPieceAtPos(targetPos, chessBoard);
    if (pieceAtTargetPos == NULL) printf("WHAT \n");
    D_PrintPiece(pieceAtTargetPos, "TargetPos (Hopefully contains white queen)");

    DeleteChessBoard(&chessBoard);
    printf("%p", chessBoard);
    
    return 0;
}








