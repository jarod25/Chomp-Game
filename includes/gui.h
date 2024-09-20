#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>

#include "client.h"
#include "server.h"
#include "ai.h"

typedef struct {
    int board[ROWS][COLS];
    GtkWidget *buttons[ROWS][COLS];
    int player;
    bool ai;
    //for server/client network
    bool first_turn_done;
    bool serverMode;
    bool clientMode;
    int sock;
    int new_socket;
    GtkWidget *player_label;
    GtkWidget *timer_label;
    guint timer_id;
    int time_left;
    GtkWidget *last_clicked;
    GMainLoop *loop;
} GameData;

// Function prototypes
gboolean callAiPlayMove(gpointer user_data);

void destroySquaresGUI(GameData *game, int row, int col);

gboolean resetButtonName(gpointer data);

gboolean handleTerminalInput(GIOChannel *channel, GIOCondition condition, gpointer data);

void showWinnerPopup(GtkWidget *parent, int player, GtkWidget *main_window);

void onButtonClicked(GtkWidget *widget, gpointer data);

gboolean aiPlayMove(gpointer data);

void activate(GtkApplication *app, gpointer user_data);

int mainGui(bool ai, bool serverMode, bool clientMode, int sock, int new_socket);

void updatePlayerLabel(GtkWidget *label, int player, bool ai);

gboolean updateTimer(gpointer data);

#endif //GUI_H
