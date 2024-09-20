#include "../../includes/clientMain.h"

/**
 * @brief Main function to manage the game from the client side.
 *
 * This function initializes the connection to the server, sets up the game board,
 * and manages the game loop for the client player. Depending on the game mode (AI or human,
 * GUI or console), it handles the player's moves, communicates with the server,
 * and processes the server's moves.
 *
 * @param ip A string representing the server's IPv4 address. Must be a valid, dot-decimal notation.
 * @param port An integer representing the server's port number. Must be a valid port number (typically between 1024 and 65535).
 * @param ai A boolean value indicating whether the client uses an AI to make moves (true = AI, false = human).
 * @param guiMode A boolean value indicating whether the game runs in graphical mode (true) or console mode (false).
 * @param serverMode A boolean indicating whether the client is in server mode (true) or not (false).
 * @param clientMode A boolean indicating whether the client is in client mode (true) or not (false).
 */
void clientMain(char* ip, int port, bool ai, bool guiMode, bool serverMode, bool clientMode) {
    // Initialization of the client's connection to the server
    int new_sock = 0;

    // Establishing the client connection to the server
    int sock = initClient(ip, port);
    printf("sock = %d \n", sock);

    // Game-related variables
    int board[ROWS][COLS];         // The game board
    int row, col;                  // Row and column of the chosen square
    char col_char;                 // Column as a character (A-I)
    int player = 1;                // Player turn (1 = Client, 2 = Server)
    char buffer[1024] = {0};       // Buffer for transferring data between client and server
    int valread = 0;               // Value to check if the server has played a move

    // Initialize the game board
    initBoard(board);

    // If the GUI mode is enabled, launch the graphical interface
    if (guiMode) {
        printf("Launching GUI...\n");
        mainGui(ai, serverMode, clientMode, sock, new_sock);
    } else {
        // Client loop - runs until the game is over
        while (1) {
            // Client's turn
            if (player == 1) {
                printf("It's Client's turn.\n");

                // Display the current game board
                displayBoard(board);

                // Prompt the player to choose a square to destroy (e.g., B3)
                printf("Client, choose a square to destroy (e.g., B3): \n");

                // Human player move
                if (!ai) {
                    // Get the move from the player
                    scanf(" %c%d", &col_char, &row);

                    // Convert the user's input into matrix indices
                    col = toupper(col_char) - 'A';  // Convert 'A' -> 0, 'B' -> 1, etc.
                    row -= 1;  // Adjust the row index to start from 0

                    // Transfer the [A-I][1-7] data to the client via buffer
                    buffer[0] = toupper(col_char);  // Store column letter as is
                    buffer[1] = (row + 1) + '0';    // Convert row number to character (1 -> '1', 2 -> '2', etc.)
                }

                // AI move
                if (ai) {
                    printf("AI is choosing a move...\n");

                    // AI selects a move
                    aiChooseMove(board, &row, &col);
                    printf("AI chose col = %d and row = %d\n", col, row);

                    // Convert column and row into valid move format for the client
                    buffer[0] = col + 'A';  // Convert 0 -> 'A', 1 -> 'B', ..., 8 -> 'I'
                    buffer[1] = row + '1';  // Convert 0 -> '1', 1 -> '2', ..., 6 -> '7'
                }

                // Validate the move format before sending it to the server
                if (strlen(buffer) == 2 &&
                    buffer[0] >= 'A' && buffer[0] <= 'I' &&
                    buffer[1] >= '1' && buffer[1] <= '7') {

                    // Verify if the square can be destroyed
                    if (canDestroy(board, row, col)) {

                        // Destroy the square if valid
                        if (destroySquaresConsole(board, row, col, false)) {
                            printf("Valid move. Sending to server...\n");

                            // Display the updated board
                            displayBoard(board);

                            // Send the move to the server
                            send(sock, buffer, strlen(buffer), 0);

                            // End of client's turn
                            player = 2;
                        }

                        // Check if the game is over
                        if (board[0][0] == 0) {
                            printf("\nSERVER WINS!\n");
                            break;
                        }

                    } else {
                        // Invalid move (square already destroyed)
                        printf("Invalid move. Square already destroyed. Try again.\n");
                        continue;
                    }

                } else {
                    // Invalid move format
                    printf("Invalid move. Try again.\n");
                    continue;
                }
            }

            // Server's turn
            if (player == 2) {
                printf("Server's turn\n");

                // Wait until server plays a move and sends it
                read(sock, buffer, 1024);
                valread = strlen(buffer);

                if (valread != 0) {
                    // Process the server's move

                    if (valread > 0) {
                        printf("\nServer played: %c%c\n", buffer[0], buffer[1]);

                        // Convert lowercase to uppercase if necessary
                        if (buffer[0] >= 'a' && buffer[0] <= 'i') {
                            buffer[0] = toupper(buffer[0]);
                        }

                        // Validate the move format
                        if (strlen(buffer) == 2 &&
                            buffer[0] >= 'A' && buffer[0] <= 'I' &&
                            buffer[1] >= '1' && buffer[1] <= '7') {

                            // Convert buffer values to matrix indices
                            col = buffer[0] - 'A';
                            row = buffer[1] - '1';

                            // Verify if the square can be destroyed
                            if (canDestroy(board, row, col)) {
                                // Destroy the square
                                destroySquaresConsole(board, row, col, false);

                                // Display the updated board
                                displayBoard(board);

                                // End of server's turn
                                player = 1;

                                // Check if the game is over
                                if (board[0][0] == 0) {
                                    printf("\nCLIENT WINS!\n");
                                    break;
                                }

                                // Clear the buffer
                                memset(buffer, 0, sizeof(buffer));
                                printf("The buffer has been erased.\n");
                            }
                        }
                    }
                }
            }
        }
    }

    // Close the socket at the end of the game
    close(sock);
}
