#include "../../includes/serverMain.h"

/**
 * @brief Main function to manage the server-side gameplay.
 *
 * This function handles the server-side operations of the game. It initializes the server,
 * sets up the game board, and alternates between the client's and server's (or AI's) turns
 * to play the game. The function also supports a graphical mode (`guiMode`) and interacts
 * with the client via socket communication.
 *
 * @param port The port number on which the server listens for a client connection.
 * @param ai Boolean flag indicating whether the AI should play on the server's side.
 * @param guiMode Boolean flag indicating whether the game should be played in graphical mode.
 * @param serverMode Boolean flag indicating if the server mode is active (unused here but passed to `mainGui`).
 * @param clientMode Boolean flag indicating if the client mode is active (unused here but passed to `mainGui`).
 */
void serverMain(int port, bool ai, bool guiMode, bool serverMode, bool clientMode) {

    // Server variables initialization
    int server_fd = 0;  // File descriptor for the server socket
    int sock = 0;       // Unused in this function but passed to `mainGui`

    // Establish the server connection and accept a client
    int new_socket = initServer(port);  // Capture the new socket returned by initServer()
    printf("Server socket initialized, connection established with client.\n");

    // Game-related variables
    int board[ROWS][COLS];         // The game board
    int row, col;                  // Row and column of the chosen square
    char col_char;                 // Column as a character (A-I)
    int player = 1;                // Player turn (1 = Client, 2 = Server)

    char buffer[1024] = {0};       // Buffer for transferring data between client and server
    int valread = 0;               // Value to check if the client has played a move

    // Initialize and display the game board
    initBoard(board);

    if (guiMode) {
        printf("Launching GUI...\n");
        mainGui(ai, serverMode, clientMode, sock, new_socket);  // Launch GUI mode if selected
    }
    else {
        // Server loop - runs until the game is over
        while (1) {

            // Client's turn
            if (player == 1) {
                printf("Client's turn\n");

                // Wait for the client to send their move
                read(new_socket, buffer, 1024);

                // Check if the game is over
                if (board[0][0] == 0) {
                    printf("\nSERVER WINS!\n");
                    break;
                }

                // Process the client's move
                valread = strlen(buffer);

                if (valread > 0) {
                    // Client's move format is [Column][Row] (e.g., B3)
                    printf("Client played: %c%c\n", buffer[0], buffer[1]);

                    // Convert lowercase to uppercase if needed
                    if (buffer[0] >= 'a' && buffer[0] <= 'i') {
                        buffer[0] = toupper(buffer[0]);
                    }

                    // Validate move format (column between A-I, row between 1-7)
                    if (strlen(buffer) == 2 &&
                        buffer[0] >= 'A' && buffer[0] <= 'I' &&
                        buffer[1] >= '1' && buffer[1] <= '7') {

                        printf("Valid move from client.\n");

                        // Convert buffer value into matrix indices
                        col = buffer[0] - 'A';  // 'A' = 0, 'I' = 8
                        row = buffer[1] - '1';  // '1' = 0, '7' = 6

                        // Verify if the square can be destroyed
                        if (canDestroy(board, row, col)) {
                            // Destroy the square
                            destroySquaresConsole(board, row, col, false);

                            // Display the updated board
                            displayBoard(board);

                            // End of client's turn, switch to server's turn
                            player = 2;

                            // Check if the game is over
                            if (board[0][0] == 0) {
                                printf("\nSERVER WINS!\n");
                                break;
                            }

                            // Clear the buffer for the next turn
                            memset(buffer, 0, sizeof(buffer));
                            printf("Buffer cleared.\n");
                        }
                    }
                }
            }

            // Server's (or AI's) turn
            if (player == 2) {
                printf("It's Server's turn.\n");

                // Display the current board
                displayBoard(board);

                // Prompt the server (or AI) to choose a square
                if (!ai) {
                    printf("Server's turn, choose a square to destroy (e.g., B3): \n");
                    scanf(" %c%d", &col_char, &row);

                    // Convert user's input into matrix indices
                    col = toupper(col_char) - 'A';  // Convert 'A' -> 0, 'B' -> 1, etc.
                    row -= 1;  // Adjust the row index to start from 0

                    // Transfer the move data to the client via buffer
                    buffer[0] = toupper(col_char);  // Store column letter as is
                    buffer[1] = (row + 1) + '0';    // Convert row number to character
                } else {
                    printf("AI is choosing a move...\n");

                    // AI selects a move
                    aiChooseMove(board, &row, &col);
                    printf("AI chose col = %d and row = %d\n", col, row);

                    // Format the move for the client
                    buffer[0] = col + 'A';  // Convert 0 -> 'A', 1 -> 'B', etc.
                    buffer[1] = row + '1';  // Convert 0 -> '1', 1 -> '2', etc.
                }

                // Validate the move format before sending it to the client
                if (strlen(buffer) == 2 &&
                    buffer[0] >= 'A' && buffer[0] <= 'I' &&
                    buffer[1] >= '1' && buffer[1] <= '7') {

                    // Verify if the square can be destroyed
                    if (canDestroy(board, row, col)) {
                        // Destroy the square
                        destroySquaresConsole(board, row, col, false);
                        printf("Valid move. Sending to client...\n");

                        // Display the updated board
                        displayBoard(board);

                        // Send the move to the client
                        send(new_socket, buffer, strlen(buffer), 0);

                        // End of server's turn, switch to client's turn
                        player = 1;

                        // Check if the game is over
                        if (board[0][0] == 0) {
                            printf("\nCLIENT WINS!\n");
                            break;
                        }
                    } else {
                        // Invalid move (square already destroyed)
                        printf("Invalid move. Square already destroyed. Try again.\n");
                        continue;
                    }

                } else {
                    // Invalid move format
                    printf("Invalid move format. Try again.\n");
                    continue;
                }
            }
        }
    }

    // Close the sockets at the end of the game
    close(new_socket);
    close(server_fd);
}
