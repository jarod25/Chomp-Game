#include "../../includes/server.h"

/**
 * @brief Initializes the server-side connection and waits for a client to connect.
 *
 * This function sets up a server that listens for incoming TCP connections on a specified port.
 * It creates a socket, binds it to the given port, listens for connections, and accepts one client.
 *
 * If any step in the process fails (socket creation, binding, listening, or accepting), the function
 * will print an error message and terminate the program with a failure exit code.
 *
 * @param port The port number on which the server will listen for incoming connections.
 *             Must be a valid port number (between 1024 and 65535 for non-privileged users).
 *
 * @return The file descriptor of the new socket for the accepted client connection.
 *         This socket can be used for communication with the client.
 *         Returns -1 if any error occurs during connection establishment.
 */
int initServer(int port) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create the TCP socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP address
    address.sin_port = htons(port);        // Convert port number to network byte order

    // Bind the socket to the given port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections (up to 3 clients in the backlog queue)
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server waiting for connection on port %d...\n", port);

    // Accept a client connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connection established with the client!\n");
    return new_socket;
}
