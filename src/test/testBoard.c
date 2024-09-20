#include "../../includes/testBoard.h"

void testInitBoard() {
    printf("===== testInitBoard =====\n");
    int board[ROWS][COLS];
    int successCount = 0, failCount = 0;

    initBoard(board);

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == 1) {
                successCount++;
            } else {
                failCount++;
            }
        }
    }

    printf("testInitBoard: %d tests succeeded, %d tests failed.\n", successCount, failCount);
}

void testDisplayBoard() {
    printf("===== testDisplayBoard =====\n");
    int board[ROWS][COLS];

    initBoard(board);

    printf("Displaying the board for testDisplayBoard:\n");
    displayBoard(board);

}
