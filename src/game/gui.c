#include "../../includes/gui.h"
#include <gtk/gtk.h>

/**
 * @brief Wrapper function to cast user data and call the AI move function.
 *
 * This function is used as a callback for functions like `g_timeout_add()`. It casts
 * the `gpointer` user data into the correct `GameData` type and calls the `aiPlayMove()`
 * function to execute the AI's move.
 *
 * @param user_data A generic pointer (`gpointer`) which should point to a `GameData` structure.
 * @return gboolean Returns the result of `aiPlayMove()`, which is typically `TRUE` to
 * continue the timeout or `FALSE` to stop it.
 */
gboolean callAiPlayMove(gpointer user_data) {
    GameData *game = (GameData *) user_data;
    return aiPlayMove(game);
}

/**
 * @brief Resets the button's style to default.
 *
 * This function resets the CSS name of the given button back to "default".
 *
 * @param data Pointer to the button to be reset.
 * @return gboolean Returns FALSE to stop further calls.
 */
gboolean resetButtonName(gpointer data) {
    GtkWidget *button = GTK_WIDGET(data);
    gtk_widget_set_name(button, "default"); // Reset the button's style
    return FALSE;
}

/**
 * @brief Destroys squares on the board starting from the given square.
 *
 * This function iterates through the game board and destroys squares starting from
 * the selected row and column. Squares are marked as "Destroyed" and the corresponding
 * button label is updated to "X".
 *
 * @param game Pointer to the game data structure.
 * @param row The row of the starting square.
 * @param col The column of the starting square.
 */
void destroySquaresGUI(GameData *game, int row, int col) {
    if (game->board[row][col] == 1) {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                GtkWidget *button = game->buttons[i][j];
                if (g_strcmp0(gtk_widget_get_name(button), "Last") == 0) {
                    gtk_widget_set_name(button, "Destroyed");
                }
            }
        }
    }

    for (int i = row; i < ROWS; i++) {
        for (int j = col; j < COLS; j++) {
            if (game->board[i][j] == 1) {
                GtkWidget *button = game->buttons[i][j];
                game->board[i][j] = 0;
                gtk_button_set_label(GTK_BUTTON(button), "X");
                gtk_widget_set_name(button, "Last");
            }
        }
    }
}
/**
 * This function
 *
 * @param label what is written to say which player it is the turn to play
 * @param player player 1 or player 2
 * @param ai says if ai is eneabled or not
 */
void updatePlayerLabel(GtkWidget *label, int player, bool ai) {
    char player_text[50];
    if (ai) {
        sprintf(player_text, "Player %d's turn (AI)", player);
    } else {
        sprintf(player_text, "Player %d's turn", player);
    }
    gtk_label_set_text(GTK_LABEL(label), player_text);
}

/**
 * @brief Displays a popup when a player wins the game.
 *
 * This function creates and displays a dialog box showing the winner of the game.
 * It also closes the main game window and the popup when the "OK" button is clicked.
 *
 * @param parent Pointer to the parent window.
 * @param player The winning player's number.
 * @param main_window Pointer to the main game window.
 */
void showWinnerPopup(GtkWidget *parent, int player, GtkWidget *main_window) {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *label;

    dialog = gtk_dialog_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Game Over");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(parent));
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    label = gtk_label_new(g_strdup_printf("Player %d is the winner!", player));
    gtk_box_append(GTK_BOX(content_area), label);

    GtkWidget *close_button = gtk_button_new_with_label("OK");
    gtk_box_append(GTK_BOX(content_area), close_button);

    g_signal_connect_swapped(close_button, "clicked", G_CALLBACK(gtk_window_destroy), dialog);
    g_signal_connect_swapped(close_button, "clicked", G_CALLBACK(gtk_window_destroy), main_window);
    gtk_widget_show(dialog);
}

/**
 * @brief Handles button click events on the game board.
 *
 * This function processes the user's button clicks, updates the board,
 * handles moves for both players (including AI and networked modes),
 * and checks for win conditions.
 *
 * @param widget Pointer to the clicked button.
 * @param data Pointer to the game data structure.
 */
void onButtonClicked(GtkWidget *widget, gpointer data) {
    GameData *game = (GameData *) data;

    int row, col;
    char buffer[1024] = {0}; // Buffer for transferring data between client and server

    GtkWidget *main_window = gtk_widget_get_ancestor(widget, GTK_TYPE_WINDOW);

    // Find the row and column of the clicked button
    for (row = 0; row < ROWS; row++) {
        for (col = 0; col < COLS; col++) {
            if (widget == game->buttons[row][col]) {
                break;
            }
        }
        if (col < COLS) break;
    }

    // If it's a game against the AI, block clicks when it's not the human player's turn
    // If it's a local game

    if ((!game->ai || (game->ai && game->player == 1))
        && game->serverMode == false && game->clientMode == false) {
        // Check if the player is trying to destroy more than 5 squares
        if (countSquares(game->board, row, col) <= 5) {
            GtkWidget *button = game->buttons[row][col];

            // Prevent actions on squares that are already destroyed or "Last"
            if (g_strcmp0(gtk_widget_get_name(button), "Last") == 0 ||
                g_strcmp0(gtk_widget_get_name(button), "Destroyed") == 0) {
                return;
            } else {
                // Destroy the squares selected
                destroySquaresGUI(game, row, col);

                // Switch player
                game->player = (game->player == 1) ? 2 : 1;

                // if it's AI turn, add a delay before AI plays
                if (game->player == 2 && game->ai) {
                    g_timeout_add(2000, (GSourceFunc) callAiPlayMove, game);
                }

                // Update the window title to reflect the current player
                updatePlayerLabel(game->player_label, game->player, false);
            }
        } else {
            GtkWidget *button = game->buttons[row][col];
            g_print("You are trying to destroy too many squares! You can only destroy up to 5 squares.\n");
            gtk_widget_set_name(button, "False");
            g_timeout_add(500, resetButtonName, button);
        }

        // Check if the player has lost (e.g., if the square 0,0 is destroyed)
        if (game->board[0][0] == 0) {
            g_print("Player %d lost!\n", game->player);
            showWinnerPopup(widget, game->player, main_window);
            g_main_loop_quit(game->loop);
        }

        // Save the last clicked button
        game->last_clicked = widget;
    }


    // If it's a game from the CLIENT point of view
    if (game->player == 1 && game->serverMode == false && game->clientMode == true) {
        // Check if the player is trying to destroy more than 5 squares
        if (countSquares(game->board, row, col) <= 5) {
            GtkWidget *button = game->buttons[row][col];

            // Prevent actions on squares that are already destroyed or "Last"
            if (g_strcmp0(gtk_widget_get_name(button), "Last") == 0 ||
                g_strcmp0(gtk_widget_get_name(button), "Destroyed") == 0) {
                return;
            } else {
                // Destroy the squares selected
                destroySquaresGUI(game, row, col);

                // Convert column and row into valid move format for the client
                buffer[0] = col + 'A'; // Convert 0 -> 'A', 1 -> 'B', ..., 8 -> 'I'
                buffer[1] = row + '1'; // Convert 0 -> '1', 1 -> '2', ..., 6 -> '7'
                g_print("Move selected: %c%c\n\n", buffer[0], buffer[1]);

                // Send the move to the server
                send(game->sock, buffer, strlen(buffer), 0);
                memset(buffer, 0, sizeof(buffer));


                // Change the player to Player 2
                game->player = 2;

                // Update the window title to reflect the current player
                updatePlayerLabel(game->player_label, game->player, false);
                gtk_widget_set_name(button, "False");
                // Wait for Player 2's move (server response)
                printf("waiting for server' move");
                read(game->sock, buffer, 1024); // Read the move from the server

                // Translate the received move to row and col
                col = buffer[0] - 'A';
                row = buffer[1] - '1';



                printf("Received move from server: %c%c\n\n", buffer[0], buffer[1]);

                // Destroy the squares based on the server's move if A1 not destroyed
                if (game->board[0][0] != 0) {
                    destroySquaresGUI(game, row, col);
                }

                // Switch back to Player 1
                game->player = 1;
                updatePlayerLabel(game->player_label, game->player, false);
            }
        } else {
            GtkWidget *button = game->buttons[row][col];
            g_print("You are trying to destroy too many squares! You can only destroy up to 5 squares.\n");
            gtk_widget_set_name(button, "False");
            g_timeout_add(500, resetButtonName, button);
        }

        // Check if the player has lost (e.g., if the square 0,0 is destroyed)
        if (game->board[0][0] == 0) {
            g_print("Player %d lost!\n", game->player);
            showWinnerPopup(widget, game->player, main_window);
            // Close the socket at the end of the game
            close(game->sock);
            g_main_loop_quit(game->loop);
        }

        // Save the last clicked button
        game->last_clicked = widget;
    }

    // if it's a game from the SERVER point of view
    if (game->player == 2 && game->serverMode == true && game->clientMode == false) {

        if (game->first_turn_done) {
        
        
            // Check if the player is trying to destroy more than 5 squares
            if (countSquares(game->board, row, col) <= 5) {
                GtkWidget *button = game->buttons[row][col];

                // Prevent actions on squares that are already destroyed or "Last"
                if (g_strcmp0(gtk_widget_get_name(button), "Last") == 0 ||
                    g_strcmp0(gtk_widget_get_name(button), "Destroyed") == 0) {
                    return;
                } else {
                    // Destroy the squares selected
                    destroySquaresGUI(game, row, col);

                    // Convert column and row into valid move format for the client
                    buffer[0] = col + 'A'; // Convert 0 -> 'A', 1 -> 'B', ..., 8 -> 'I'
                    buffer[1] = row + '1'; // Convert 0 -> '1', 1 -> '2', ..., 6 -> '7'
                    g_print("Move selected: %c%c\n\n", buffer[0], buffer[1]);

                    // Send the move to the client
                    send(game->new_socket, buffer, strlen(buffer), 0);
                    memset(buffer, 0, sizeof(buffer));


                    // Change the player to Player 1
                    game->player = 1;

                    // Update the window title to reflect the current player
                    updatePlayerLabel(game->player_label, game->player, false);
                    gtk_widget_set_name(button, "False");
                    // Wait for Player 1's move (client response)
                    read(game->new_socket, buffer, 1024); // Read the move from the client

                    // Translate the received move to row and col
                    col = buffer[0] - 'A';
                    row = buffer[1] - '1';

                    printf("Received move from client: %c%c\n\n", buffer[0], buffer[1]);

                    // Destroy the squares based on the client's move if A1 not destroyed
                    if (game->board[0][0] != 0) {
                        destroySquaresGUI(game, row, col);
                    }

                    // Switch back to Player 2
                    game->player = 2;
                    updatePlayerLabel(game->player_label, game->player, false);
                }
            } else {
                GtkWidget *button = game->buttons[row][col];
                g_print("You are trying to destroy too many squares! You can only destroy up to 5 squares.\n");
                gtk_widget_set_name(button, "False");
                g_timeout_add(500, resetButtonName, button);
            }
        }
        else if (!game->first_turn_done){
            game->first_turn_done = true;
            g_print("we read the client's move");
            // Wait for Player 1's move (client response)
            read(game->new_socket, buffer, 1024); // Read the move from the client

            // Translate the received move to row and col
            col = buffer[0] - 'A';
            row = buffer[1] - '1';

            printf("Received move from client: %c%c\n\n", buffer[0], buffer[1]);

            // Destroy the squares based on the client's move
            destroySquaresGUI(game, row, col);
            memset(buffer, 0, sizeof(buffer));

        }
        

        // Check if the player has lost (e.g., if the square 0,0 is destroyed)
        if (game->board[0][0] == 0) {
            g_print("Player %d lost!\n", game->player);
            showWinnerPopup(widget, game->player, main_window);
            // Close the socket at the end of the game
            close(game->new_socket);
            g_main_loop_quit(game->loop);
        }

        // Save the last clicked button
        game->last_clicked = widget;
    }
}

/**
 * @brief AI performs its move.
 *
 * This function allows the AI to choose a move, destroy squares on the board, and
 * update the game state accordingly. It also checks for win conditions after the move.
 *
 * @param data Pointer to the game data structure.
 * @return gboolean Returns FALSE to stop further calls.
 */
gboolean aiPlayMove(gpointer data) {
    GameData *game = (GameData *) data;
    int row, col;

    printf("AI is choosing a move...\n");
    aiChooseMove(game->board, &row, &col);

    if (row != -1 && col != -1) {
        destroySquaresGUI(game, row, col);
        game->player = 1;
    } else {
        printf("AI could not find a valid move.\n");
    }

    if (game->board[0][0] == 0) {
        g_print("Player %d lost!\n", game->player);
        showWinnerPopup(game->last_clicked, game->player, gtk_widget_get_ancestor(game->last_clicked, GTK_TYPE_WINDOW));
        g_main_loop_quit(game->loop);
    }

    return FALSE;
}

/**
 * @brief Handles terminal input in console mode.
 *
 * This function allows the user to input their move through the terminal, which is then
 * processed to destroy squares on the board.
 *
 * @param channel The GIO channel for reading input.
 * @param condition Condition flags for the input.
 * @param data Pointer to the game data structure.
 * @return gboolean Returns TRUE to continue monitoring input.
 */
gboolean handleTerminalInput(GIOChannel *channel, GIOCondition condition, gpointer data) {
    (void) channel;
    (void) condition;
    GameData *game = (GameData *) data;
    char col_char;
    int row;

    g_print("Enter the square to destroy (e.g., A1): ");
    if (scanf(" %c%d", &col_char, &row) != 2) {
        g_print("Input error.\n");
        return TRUE; // Continue monitoring input
    }

    col_char = toupper(col_char);
    int col = col_char - 'A';
    row -= 1;

    // Validate coordinates
    if (col >= 0 && col < COLS && row >= 0 && row < ROWS) {
        g_print("Chosen square: %c%d\n", col_char, row + 1);
        if (countSquares(game->board, row, col) <= 5) {
            destroySquaresGUI(game, row, col);
            game->player = (game->player == 1) ? 2 : 1;
            if (game->player == 2 && game->ai) {
                g_timeout_add(2000, (GSourceFunc) callAiPlayMove, game);
            }
        } else {
            g_print("You are trying to destroy too many squares!\n");
        }
    } else {
        g_print("Invalid coordinates.\n");
    }

    return TRUE; // Continue monitoring input
}

/**
 * @brief Activates the GTK application, initializes the game board and grid.
 *
 * This function is called when the GTK application is activated. It creates the main game window,
 * sets up the game board using a grid of buttons, and initializes the board state in the game data.
 * Each button on the grid corresponds to a cell in the game, and click events are connected to the
 * onButtonClicked callback. It also loads CSS styles for the game.
 *
 * @param app Pointer to the GtkApplication instance.
 * @param user_data Pointer to user data, in this case, the GameData structure.
 */
void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *box;
    GameData *game = (GameData *) user_data;

    int button_size = 50;
    int board_width = COLS * button_size;
    int board_height = ROWS * button_size;

    window = gtk_application_window_new(app);
    if (game->serverMode) {
        gtk_window_set_title(GTK_WINDOW(window), "Game Board - Server");
    } else if (game->clientMode) {
        gtk_window_set_title(GTK_WINDOW(window), "Game Board - Client");
    } else {
        gtk_window_set_title(GTK_WINDOW(window), "Game Board");
    }
    gtk_window_set_default_size(GTK_WINDOW(window), board_width + 100, board_height + 100);

    grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

    for (int j = 0; j < COLS; j++) {
        GtkWidget *label = gtk_label_new(g_strdup_printf("%c", 'A' + j));
        gtk_grid_attach(GTK_GRID(grid), label, j + 1, 0, 1, 1);
    }

    for (int i = 0; i < ROWS; i++) {
        GtkWidget *label = gtk_label_new(g_strdup_printf("%d", i + 1));
        gtk_grid_attach(GTK_GRID(grid), label, 0, i + 1, 1, 1);
        for (int j = 0; j < COLS; j++) {
            GtkWidget *button = gtk_button_new_with_label("");
            game->buttons[i][j] = button;
            game->board[i][j] = 1;
            gtk_grid_attach(GTK_GRID(grid), button, j + 1, i + 1, 1, 1);
            g_signal_connect(button, "clicked", G_CALLBACK(onButtonClicked), game);
        }
    }

    GtkCssProvider *css_provider = gtk_css_provider_new();
    GFile *css_file = g_file_new_for_path("src/game/styles.css");
    gtk_css_provider_load_from_file(css_provider, css_file);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                               GTK_STYLE_PROVIDER(css_provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Create a vertical box to hold the player label and grid
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_set_spacing(GTK_BOX(box), GTK_ALIGN_CENTER);

    // Create and set the player label
    GtkWidget *player_label = gtk_label_new("Player 1");
    gtk_box_append(GTK_BOX(box), player_label);

    // Append the grid to the box
    gtk_box_append(GTK_BOX(box), grid);

    // Set the box as the child of the window
    gtk_window_set_child(GTK_WINDOW(window), box);

    gtk_widget_show(window);
    updatePlayerLabel(player_label, game->player, false);

    GIOChannel *channel = g_io_channel_unix_new(fileno(stdin));
    g_io_add_watch(channel, G_IO_IN, handleTerminalInput, game);
}



/**
 * @brief Main function for launching the GUI version of the game.
 *
 * This function initializes the game data, sets the player based on the mode (server or client), and
 * launches the GTK application. It connects the game data to the activate callback to handle the game's
 * graphical interface. It also initializes networking sockets depending on the mode of play (server or client).
 *
 * @param ai Boolean flag indicating if the AI mode is enabled.
 * @param serverMode Boolean flag indicating if the game is running in server mode.
 * @param clientMode Boolean flag indicating if the game is running in client mode.
 * @param sock The socket used for client-server communication.
 * @param new_socket The new socket used by the server for communication.
 * @return int The exit status of the application.
 */
int mainGui(bool ai, bool serverMode, bool clientMode, int sock, int new_socket) {
    GtkApplication *app;
    GameData game = {0};
    printf("value of sock when entering gui = %d \n", sock);
    printf("clientMode is: %s after mainGui\n", clientMode ? "true" : "false");
    printf("serverMode is: %s after mainGui\n", serverMode ? "true" : "false");

    initBoard(game.board);

    // Initialization of sock (client side) and new_socket (server side)

    // Set the player based on the mode
    if (serverMode) {
        game.player = 2;
        g_print("player Server, please press any square to recieve the first client's move");
    } else {
        game.player = 1;
    }

    bool first_turn_done = false; // permet au server le lire le premier move du client
    game.first_turn_done = first_turn_done;
    game.ai = ai;
    game.clientMode = clientMode;
    game.serverMode = serverMode;
    game.sock = sock;
    game.new_socket = new_socket;

    app = gtk_application_new("org.example.gtk4", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), &game);

    game.loop = g_main_loop_new(NULL, FALSE);

    int status = g_application_run(G_APPLICATION(app), 0, 0);
    g_object_unref(app);
    g_main_loop_unref(game.loop);

    return status;
}
