#include "../../includes/board.h"

/**
 * Initializes the board with default values for the console.
 * This function sets all cells on the board to a default value of 1.
 *
 * @param board A 2D array representing the board to be initialized.
 *              It has dimensions defined by ROWS and COLS constants.
 */
void initBoard(int board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = 1;
        }
    }
}

/**
 * Displays the current state of the board on the console.
 * The board is printed with column labels (A to I) and row numbers (1 to ROWS).
 *
 * @param board A 2D array representing the board to be displayed.
 *              It has dimensions defined by ROWS and COLS constants.
 *              The values of the cells are printed in the console.
 */
void displayBoard(int board[ROWS][COLS]) {
    printf("   A B C D E F G H I\n");
    for (int i = 0; i < ROWS; i++) {
        printf("%d ", i + 1);
        for (int j = 0; j < COLS; j++) {
            printf(" %d", board[i][j]);
        }
        printf("\n");
    }
}
