#include "../../includes/gameLogic.h"

/**
 * Checks if a specific square on the board can be destroyed.
 * A square can be destroyed if it is within the bounds of the board and is not empty.
 *
 * @param board A 2D array representing the game board with dimensions defined by ROWS and COLS constants.
 * @param row The row index of the square to check.
 * @param col The column index of the square to check.
 * @return True if the square can be destroyed (i.e., it is within bounds and not empty), false otherwise.
 */
bool canDestroy(int board[ROWS][COLS], int row, int col) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS || board[row][col] == 0) {
        return false;
    }
    return true;
}

/**
 * Counts the number of contiguous squares starting from a given square that would be destroyed.
 * The count is done from the specified starting point until an empty square is encountered or the board boundaries are reached.
 *
 * @param board A 2D array representing the game board with dimensions defined by ROWS and COLS constants.
 * @param row The starting row index for counting squares.
 * @param col The starting column index for counting squares.
 * @return The number of contiguous squares from the starting point that would be destroyed.
 */
int countSquares(int board[ROWS][COLS], int row, int col) {
    int count = 0;
    for (int i = row; i < ROWS; i++) {
        for (int j = col; j < COLS; j++) {
            if (board[i][j] == 1) {
                count++;
            } else {
                break;
            }
        }
    }
    return count;
}

/**
 * Displays the details of the previous move on the console.
 * The move is displayed using the column letter and row number format.
 *
 * @param row The row index of the previous move.
 * @param col The column index of the previous move.
 */
void showPreviousMoveConsole(int row, int col) {
    printf("Previous move: %c%d\n", col + 'A', row + 1);
}

/**
 * Destroys squares on the board starting from the given square, according to a specific pattern.
 * The destruction pattern extends from the starting square until a maximum of 5 contiguous squares are destroyed.
 *
 * @param board A 2D array representing the game board with dimensions defined by ROWS and COLS constants.
 * @param row The starting row index for destruction.
 * @param col The starting column index for destruction.
 * @param ai A boolean indicating if the move is made by the AI (true) or a player (false).
 * @return True if the squares were successfully destroyed, otherwise false. If the move is made by a player and exceeds 5 squares, false is returned.
 */
bool destroySquaresConsole(int board[ROWS][COLS], int row, int col, bool ai) {
    if (!ai && countSquares(board, row, col) > 5) {
        printf("You are trying to destroy too many squares! You can only destroy up to 5 squares.\n");
        return false;
    }
    for (int i = row; i < ROWS; i++) {
        for (int j = col; j < COLS; j++) {
            if (board[i][j] == 1 && countSquares(board, i, j) <= 5) {
                board[i][j] = 0;
            }
        }
    }
    if (!ai) showPreviousMoveConsole(row, col);
    return true;
}
