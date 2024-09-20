#include "../../includes/testAI.h"

void testDestroySquares() {
    printf("===== testDestroySquares =====\n");
    int board[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    bool result = destroySquares(board, 0, 2);
    ASSERT_EQ(true, result);

    ASSERT_NOT_EQ(0, board[0][0]);
    ASSERT_NOT_EQ(0, board[0][1]);
    ASSERT_EQ(0, board[0][2]);
    ASSERT_EQ(0, board[1][2]);
}

void testEvaluateBoard() {
    printf("===== testEvaluateBoard =====\n");
    int board[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    int score = evaluateBoard(board);
    ASSERT_EQ(8, score);
}

void testMinimax() {
    printf("===== testMinimax =====\n");
    int board[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    int score = minimax(board, 1, true, -INF, INF);
    printf("Minimax's result : %d\n", score);
}

void testAiChooseMove() {
    printf("===== testAiChooseMove =====\n");
    int board[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    int best_row, best_col;
    aiChooseMove(board, &best_row, &best_col);

    ASSERT_TRUE(best_row >= 0 && best_col >= 0);
}
