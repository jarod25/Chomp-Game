#include "../../includes/mainTest.h"

int main() {
    printf("Running tests...\n");

//  Board Test
    testInitBoard();
    testDisplayBoard();

//  AI Test
    testDestroySquares();
    testEvaluateBoard();
    testMinimax();
    testAiChooseMove();

//  GameLogic Test
    testCanDestroy();
    testCountSquares();
    testDestroySquaresConsole();

    printf("All tests passed!\n");
    return 0;
}
