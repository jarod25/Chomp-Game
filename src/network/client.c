#include "../../includes/client.h"

/**
 * @brief Initializes a client connection to the server.
 *
 * This function creates a TCP socket, configures the server's address and port,
 * and attempts to establish a connection to the specified server.
 * If the socket creation or connection fails, the function returns -1.
 *
 * @param ip A string representing the server's IPv4 address.
 *           Must be a valid, dot-decimal notation (e.g., "192.168.1.1").
 * @param port An integer representing the server's port number.
 *             Must be a valid port number (typically between 1024 and 65535).
 * @return On success, returns the socket file descriptor (a non-negative integer).
 *         On failure, returns -1 and prints an error message to the console.
 */
int initClient(char* ip, int port) {
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Create the TCP Socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Configure the IP address
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address \n");
        return -1;
    }

    // Connect client to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection error \n");
        return -1;
    }
    return sock;
}
