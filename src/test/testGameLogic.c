#include "../../includes/testGameLogic.h"

void testCanDestroy() {
    printf("===== testCanDestroy =====\n");
    int board[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    ASSERT_TRUE(canDestroy(board, 0, 0));
    ASSERT_FALSE(canDestroy(board, 7, 0));
    ASSERT_FALSE(canDestroy(board, -1, 0));
    ASSERT_FALSE(canDestroy(board, 0, 9));
}

void testCountSquares() {
    printf("===== testCountSquares =====\n");
    int board[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    ASSERT_EQ(63, countSquares(board, 0, 0));
    ASSERT_EQ(54, countSquares(board, 1, 0));
    ASSERT_EQ(36, countSquares(board, 1, 3));
    ASSERT_TRUE(countSquares(board, 8, 8) <= 5);
}

void testDestroySquaresConsole() {
    printf("===== testDestroySquaresConsole =====\n");
    int board[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    ASSERT_FALSE(destroySquaresConsole(board, 0, 0, false));
}
