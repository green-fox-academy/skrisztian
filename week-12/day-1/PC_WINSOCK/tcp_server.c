#include tcp_server.h

void tcp_server(void)
{
    char service[20] = "tcp_server";

    // Create a new socket to listen for client connections.
    SOCKET listening_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check for errors to ensure that the socket is a valid socket.
    if (listening_socket == INVALID_SOCKET) {
        server_log(service, "Error at socket(), error code: ", WSAGetLastError());
        return;
    } else {
        server_log(service, "socket() is OK!", 0);
    }

    // Set up a SOCKADDR_IN structure
    SOCKADDR_IN server_addr;
    server_addr.sin_family = AF_INET;      
    server_addr.sin_port = htons(TCP_SERVER_PORT);    
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the server address info to the socket created
    if (bind(listening_socket, (SOCKADDR*) &server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        server_log(service, "bind() failed! Error code: ", WSAGetLastError());
        closesocket(listening_socket);
        return;
    } else {
        server_log(service, "bind() is OK!", 0);
    }

    // Define backlog, which is max allowed connection queue length
    static const int backlog = TCP_SERVER_QUEUE;

    // Listen for client connections.
    if (listen(listening_socket, backlog) == SOCKET_ERROR) {
        server_log(service, "listen(): Error listening on socket: ", WSAGetLastError());
        closesocket(listening_socket);
        return;
    } else {
        server_log(service, "listen() is OK, waiting for connections.", 0);
    }

    server_log(service, "accept() is ready.", 0);

    // Accept a new connection when one arrives.
    SOCKET new_connection;
    SOCKADDR_IN client_addr;
    int client_addr_size = sizeof(client_addr);
    char recvbuff[TCP_SERVER_BUFFER_LEN];
    int bytes_received;

    while(1) {

        // Build up connection with client
        new_connection = accept(listening_socket, (SOCKADDR*) &client_addr, &client_addr_size);
        if (new_connection == INVALID_SOCKET) {
            server_log(service, "error with accept(), cannot connect to client. ", WSAGetLastError());
        } else {
            server_log(service, "accept() is OK.", 0);
        }

        // Client is connected, ready to receive data
        bytes_received = recv(new_connection, recvbuff, sizeof(recvbuff), 0);
        recvbuff[bytes_received] = 0;

        // When there is data
		// Loop if needed
        if (bytes_received > 0) {

            // Print message
			printf("%s:%d TCP>%s\n", inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port), recvbuff);

			// Log message
            server_log(service, recvbuff, 0);

			// Close client socket
            if(closesocket(new_connection) != 0)
                server_log(service, "Cannot close client's socket. Error code: ", WSAGetLastError());
            else
                server_log(service, "Closing client's socket.", 0);
        }
     } // END while

    // Close listening socket
    if(closesocket(listening_socket) != 0)
        server_log(service, "Cannot close server socket. Error code: ", WSAGetLastError());
    else
        server_log(service, "Closing server socket.", 0);

    return;
}