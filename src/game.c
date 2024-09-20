#include "../includes/game.h"

/**
 * Checks if the '-ia' flag is present in the command-line arguments.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return True if the '-ia' flag is present, false otherwise.
 */
bool checkIa(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-ia") == 0) {
            return true;
        }
    }
    return false;
}

/**
 * Extracts the IP address and port number from the command-line arguments.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @param ip A buffer to store the extracted IP address.
 * @param port A pointer to store the extracted port number.
 * @return True if both IP address and port were successfully extracted, false otherwise.
 */
bool extractIpPort(int argc, char *argv[], char *ip, int *port) {
    for (int i = 1; i < argc; i++) {
        if (sscanf(argv[i], "%15[^:]:%d", ip, port) == 2) {
            return true;
        }
    }
    return false;
}

/**
 * Extracts the port number from the command-line arguments.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @param port A pointer to store the extracted port number.
 * @return True if the port was successfully extracted, false otherwise.
 */
bool extractPort(int argc, char *argv[], int *port) {
    for (int i = 1; i < argc; i++) {
        int extracted_port = atoi(argv[i]);
        if (extracted_port > 0) {
            *port = extracted_port;
            return true;
        }
    }
    return false;
}

/**
 * Prints the usage instructions for the program.
 *
 * @param prog_name The name of the program.
 */
void printUsage(char *prog_name) {
    printf("Usage:\n");
    printf("  - Server: %s -s [-ia] <port>\n", prog_name);
    printf("  - Client: %s -c [-ia] <ip>:<port>\n", prog_name);
    printf("  - Local : %s -l [-ia]\n", prog_name);
}

/**
 * Main function for the program, handling command-line arguments and launching the appropriate mode.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return 0 on success, -1 on failure (e.g., invalid arguments).
 */
int main(int argc, char *argv[]) {
    // Check if AI mode is enabled
    bool aiMode = checkIa(argc, argv);
    printf("AI mode: %s\n", aiMode ? "enabled" : "disabled");

    if (argc >= 2) {
        bool localMode = false, serverMode = false, clientMode = false, guiMode = true;
        int port = 0;
        char ip[16] = {0};

        // Determine the mode based on flags
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-l") == 0) {
                localMode = true;
            } else if (strcmp(argv[i], "-s") == 0) {
                serverMode = true;
            } else if (strcmp(argv[i], "-c") == 0) {
                clientMode = true;
            } else if (strcmp(argv[i], "-g") == 0) {
                guiMode = false;
            }
        }

        // Launch the appropriate mode
        if (localMode) {
            localMain(aiMode, guiMode);
        } else if (serverMode && extractPort(argc, argv, &port)) {
            printf("Starting server on port: %d\n", port);
            serverMain(port, aiMode, guiMode, serverMode, clientMode);
        } else if (clientMode && extractIpPort(argc, argv, ip, &port)) {
            printf("Connecting to server at IP: %s, Port: %d\n", ip, port);
            clientMain(ip, port, aiMode, guiMode, serverMode, clientMode);
        } else {
            printUsage(argv[0]);
            return -1;
        }
    } else {
        printUsage(argv[0]);
        return -1;
    }

    return 0;
}
