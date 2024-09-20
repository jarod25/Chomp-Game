#include "../../includes/ai.h"

/**
 * Destroys squares on the board starting from the given square, according to a specific pattern.
 * The destruction pattern extends from the starting square until a maximum of 5 contiguous squares are destroyed.
 *
 * @param board A 2D array representing the game board with dimensions defined by ROWS and COLS constants.
 * @param row The starting row index for destruction.
 * @param col The starting column index for destruction.
 * @return True if the squares were successfully destroyed, otherwise false. If more than 5 squares are to be destroyed, false is returned.
 */
bool destroySquares(int board[ROWS][COLS], int row, int col) {
    if (countSquares(board, row, col) > 5) {
        return false;
    }
    for (int i = row; i < ROWS; i++) {
        for (int j = col; j < COLS; j++) {
            if (board[i][j] == 1 && countSquares(board, i, j) <= 5) {
                board[i][j] = 0;
            }
        }
    }
    return true;
}

/**
 * Evaluates the current state of the board by counting the number of squares that are still present.
 *
 * @param board A 2D array representing the game board with dimensions defined by ROWS and COLS constants.
 * @return The total number of squares on the board that are still present (i.e., not destroyed).
 */
int evaluateBoard(int board[ROWS][COLS]) {
    int score = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == 1) {
                score++;
            }
        }
    }
    return score;
}

/**
 * Performs the Minimax algorithm with Alpha-Beta pruning to determine the best move.
 * The algorithm recursively explores all possible moves up to a specified depth and selects the move with the best evaluation score.
 *
 * @param board A 2D array representing the game board with dimensions defined by ROWS and COLS constants.
 * @param depth The maximum depth of the search tree.
 * @param isMaximizing A boolean indicating whether the current player is maximizing (true) or minimizing (false).
 * @param alpha The current best score for the maximizing player.
 * @param beta The current best score for the minimizing player.
 * @return The best score for the current player.
 */
int minimax(int board[ROWS][COLS], int depth, bool isMaximizing, int alpha, int beta) {
    if (depth == 0 || evaluateBoard(board) <= 0) {
        return evaluateBoard(board);
    }

    if (isMaximizing) {
        int bestValue = -INF;
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                if (canDestroy(board, r, c)) {
                    int new_board[ROWS][COLS];
                    memcpy(new_board, board, sizeof(int) * ROWS * COLS);
                    if (destroySquares(new_board, r, c)) {
                        int value = minimax(new_board, depth - 1, false, alpha, beta);
                        bestValue = (value > bestValue) ? value : bestValue;
                        alpha = (alpha > bestValue) ? alpha : bestValue;

                        if (beta <= alpha) {
                            return bestValue;
                        }
                    }
                }
            }
        }
        return bestValue;
    } else {
        int bestValue = INF;
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                if (canDestroy(board, r, c)) {
                    int new_board[ROWS][COLS];
                    memcpy(new_board, board, sizeof(int) * ROWS * COLS);
                    if (destroySquares(new_board, r, c)) {
                        int value = minimax(new_board, depth - 1, true, alpha, beta);
                        bestValue = (value < bestValue) ? value : bestValue;
                        beta = (beta < bestValue) ? beta : bestValue;

                        if (beta <= alpha) {
                            return bestValue;
                        }
                    }
                }
            }
        }
        return bestValue;
    }
}

/**
 * Shuffles an array of moves randomly.
 * This function is used to introduce randomness into the move selection process.
 *
 * @param moves A 2D array containing the moves to be shuffled. Each move is represented by a pair of row and column indices.
 * @param num_moves The number of moves in the array.
 */
void shuffleMoves(int moves[][2], int num_moves) {
    for (int i = num_moves - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp_row = moves[i][0];
        int temp_col = moves[i][1];
        moves[i][0] = moves[j][0];
        moves[i][1] = moves[j][1];
        moves[j][0] = temp_row;
        moves[j][1] = temp_col;
    }
}

/**
 * Chooses the best move for the AI using the Minimax algorithm.
 * The AI evaluates all possible moves and selects the one with the highest score.
 * If only the A1 square is left, the AI will choose it by default.
 *
 * @param board A 2D array representing the game board with dimensions defined by ROWS and COLS constants.
 * @param best_row A pointer to an integer where the selected row index will be stored.
 * @param best_col A pointer to an integer where the selected column index will be stored.
 */
void aiChooseMove(int board[ROWS][COLS], int *best_row, int *best_col) {
    int moves[ROWS * COLS][2];
    int num_moves = 0;
    int only_A1_left = 1;

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (canDestroy(board, r, c)) {
                moves[num_moves][0] = r;
                moves[num_moves][1] = c;
                num_moves++;
                if (!(r == 0 && c == 0)) {
                    only_A1_left = 0;
                }
            }
        }
    }

    if (!only_A1_left) {
        int valid_moves[ROWS * COLS][2];
        int valid_moves_count = 0;

        for (int i = 0; i < num_moves; i++) {
            if (!(moves[i][0] == 0 && moves[i][1] == 0)) {
                valid_moves[valid_moves_count][0] = moves[i][0];
                valid_moves[valid_moves_count][1] = moves[i][1];
                valid_moves_count++;
            }
        }
        memcpy(moves, valid_moves, sizeof(valid_moves));
        num_moves = valid_moves_count;
    }

    shuffleMoves(moves, num_moves);

    int bestValue = -INF;
    *best_row = -1;
    *best_col = -1;

    for (int i = 0; i < num_moves; i++) {
        int r = moves[i][0];
        int c = moves[i][1];

        int new_board[ROWS][COLS];
        memcpy(new_board, board, sizeof(int) * ROWS * COLS);
        if (destroySquares(new_board, r, c)) {
            int moveValue = minimax(new_board, MAX_DEPTH - 1, false, -INF, INF);
            if (moveValue > bestValue) {
                bestValue = moveValue;
                *best_row = r;
                *best_col = c;
            }
        }
    }

    if (*best_row == -1 && *best_col == -1 && only_A1_left) {
        *best_row = 0;
        *best_col = 0;
    }

    printf("AI chooses move at %c%d\n", *best_col + 'A', *best_row + 1);
}

/**
 * Executes a move on the board by destroying squares starting from the specified position.
 *
 * @param board A 2D array representing the game board with dimensions defined by ROWS and COLS constants.
 * @param row The row index of the square to start destruction.
 * @param col The column index of the square to start destruction.
 */
void executeMove(int board[ROWS][COLS], int row, int col) {
    if (canDestroy(board, row, col)) {
        if (destroySquares(board, row, col)) {
            printf("Move executed at %c%d.\n", col + 'A', row + 1);
        } else {
            printf("The move exceeds the limit of 5 squares.\n");
        }
    } else {
        printf("Invalid move.\n");
    }
}
