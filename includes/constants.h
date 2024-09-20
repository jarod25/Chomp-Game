#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#ifdef USE_GUI
#include <gtk/gtk.h>
#include <glib.h>
#endif

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>


#define ROWS 7
#define COLS 9
#define MAX_DEPTH 5
#define INF 1000 

#endif //CONSTANTS_H
