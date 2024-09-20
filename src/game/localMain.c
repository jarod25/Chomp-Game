#include "../../includes/localMain.h"

/**
 * Manages the main game loop for a local game session.
 * Depending on the flags, it either launches a GUI or runs the game in the console.
 *
 * @param ai A boolean indicating whether the AI is playing (true) or not (false).
 * @param gui A boolean indicating whether to launch the GUI (true) or use the console (false).
 */
void localMain(bool ai, bool gui) {
    int board[ROWS][COLS];
    int row, col;
    char col_char;
    int player = 1;

    if (gui) {
        printf("Launching GUI...\n");
        mainGui(ai, false, false, 0, 0);
    } else {
        initBoard(board);

        while (1) {
            displayBoard(board);

            if (!ai || player == 1) {
                printf("Player %d's turn, choose a square to destroy (e.g., B3): \n", player);
                scanf(" %c%d", &col_char, &row);

                // Convert the input into a row and column
                col = toupper(col_char) - 'A'; // Convert 'A' -> 0, 'B' -> 1, etc.
                row -= 1; // Adjust the row index to start from 0

                if (!canDestroy(board, row, col)) {
                    printf("Invalid move, try again.\n");
                    continue;
                }

                if (!destroySquaresConsole(board, row, col, false)) {
                    continue;
                }
            } else {
                printf("AI is choosing a move...\n");

                aiChooseMove(board, &row, &col);

                if (row != -1 && col != -1) {
                    executeMove(board, row, col);
                } else {
                    printf("AI could not find a valid move.\n");
                }
            }

            if (evaluateBoard(board) == 0) {
                printf("Player %d has lost!\n", player);
                break;
            }

            player = (player == 1) ? 2 : 1;
        }
    }
}
