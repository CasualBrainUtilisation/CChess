#include <stdio.h>
#include <ctype.h>
#include "CCBoard.h"

/* Very simple visualization of the chessGame in the terminal */

// Function that prints out given board in the terminal
void PrintBoard(Board *board)
{
    // Each piece will be visiualized with the char at the index of its pieceType in pieceVisualization[6]
    char pieceVisualization[6] = "pnbrqk";

    // Scout through the whole board and print each piece
    for (int y = 1; y <= 8; y++)
    {
        for (int x = 1; x <= 8; x++)
        {
            // Get the current pos in the loop
            Pos pos = {x, y};
            // Get the piece at the current pos
            Piece *pieceAtPos = GetPieceAtPos(pos, board);
            // Now if this piece is NULL just print emtpy spaces, else print the actual piece (WOW)
            if (pieceAtPos == NULL)
            {
                printf("%2.c", '_');
            }
            else
            {
                // Get the char of the pieceType the piece here has
                char charToPrint = pieceVisualization[pieceAtPos->pieceType];
                // Check wether the piece Color is white, if so make the charToPrint uppercase
                if (pieceAtPos->pieceColor == 0)
                {
                    charToPrint = toupper(charToPrint);
                }
                // Finally print the char
                printf("%2.c", charToPrint);
            }  
        }
        // Between each line there should be a new line char
        printf("\n");
    }
}