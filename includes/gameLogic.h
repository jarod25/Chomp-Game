#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "constants.h"

bool canDestroy(int board[ROWS][COLS], int row, int col);
int countSquares(int board[ROWS][COLS], int row, int col);
void showPreviousMoveConsole(int row, int col);
bool destroySquaresConsole(int board[ROWS][COLS], int row, int col, bool ai);

#endif //GAME_LOGIC_H
