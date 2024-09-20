#ifndef AI_H
#define AI_H

#include "constants.h"
#include "gameLogic.h"
#include "board.h"

bool destroySquares(int board[ROWS][COLS], int row, int col);
int evaluateBoard(int board[ROWS][COLS]);
int minimax(int board[ROWS][COLS], int depth, bool isMaximizing, int alpha, int beta);
void shuffleMoves(int moves[][2], int num_moves);
void aiChooseMove(int board[ROWS][COLS], int *best_row, int *best_col);
void executeMove(int board[ROWS][COLS], int row, int col);
void receiveOpponentMove(int board[ROWS][COLS], int row, int col);

#endif //AI_H
