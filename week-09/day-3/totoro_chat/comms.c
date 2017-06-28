#include "comms.h"



/*
 * This file contains the communications related functions
 * e.g. server, client, etc.
 */

void message_server(void)
{
    char service[20] = "msg_server";

    // Initialize winsock
    WSADATA ws_data;    // winsock version
    WORD ws_ver;        // pointer to winsock version related data

    ws_ver = MAKEWORD(2, 2);   // Set version to 2.2

    // Start winsock with error checking
    if (WSAStartup(ws_ver, &ws_data) != 0) {
        server_log(service, "WSAStartup failed with error: ", WSAGetLastError());
        return;
    }

    // Create a new socket to listen for client connections.
    SOCKET listening_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check for errors to ensure that the socket is a valid socket.
    if (listening_socket == INVALID_SOCKET) {
        server_log(service, "Error at socket(), error code: ", WSAGetLastError());

        // Clean up, exit
        WSACleanup();
        return;

    // Socket started fine
    } else {
        server_log(service, "socket() is OK!", 0);
    }

    // Set up a SOCKADDR_IN structure that will tell bind that we
    // want to listen for connections on all interfaces using the given port

    int port = m_port;                                  // Set port to message port
    SOCKADDR_IN server_addr;
    server_addr.sin_family = AF_INET;                   // IPv4
    server_addr.sin_port = htons(port);                 // host-to-network byte order
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    // Listen on all interface, host-to-network byte order

    // Bind the server address info to the socket created
    // Check for general errors.
    if (bind(listening_socket, (SOCKADDR*) &server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        server_log(service, "bind() failed! Error code: ", WSAGetLastError());

        // Close socket, clean up, exit
        closesocket(listening_socket);
        WSACleanup();
        return;

    } else {

        server_log(service, "bind() is OK!", 0);
    }

    // Define backlog, which is max allowed connection queue length
    static const int backlog = 5;   // 5 is normal for many applications.

    // Listen for client connections.
    if (listen(listening_socket, backlog) == SOCKET_ERROR) {

        server_log(service, "listen(): Error listening on socket: ", WSAGetLastError());

        // Close, cleanup, exit
        closesocket(listening_socket);
        WSACleanup();
        return;

    } else {
        log_msg[0] = '\0';
        server_log(service, "listen() is OK, I'm waiting for connections.", 0);
    }

    server_log(service, "accept() is ready.", 0);

    // Accept a new connection when one arrives.
    SOCKET new_connection;
    SOCKADDR_IN client_addr;                // client's IP address
    int client_addr_size = sizeof(client_addr);
    char recvbuff[1024];
    int bytes_received;

    while(run_msg_server) {

        // Build up connection with client
        new_connection = accept(listening_socket, (SOCKADDR*) &client_addr, &client_addr_size);
        if (new_connection < 0) {
            server_log(service, "error with accept(), cannot connect to client. ", WSAGetLastError());
            break;
        } else {
            server_log(service, "accept() is OK.", 0);
        }

        // Client is connected, ready to receive data
        bytes_received = recv(new_connection, recvbuff, sizeof(recvbuff), 0);

        // When there is data
        if (bytes_received > 0) {

            // FIXME write sender IP or name too
            for(int i =0; i < bytes_received; i++) {
                printf("%c", recvbuff[i]);
                log_msg[i] = recvbuff[i];
            }

            printf("\"\n");

            char msg_buffer[255] = "received:";
            strcat(msg_buffer, recvbuff);
            server_log(service, msg_buffer, 0);
        }

        // Clean up all the send/receive communication, get ready for new one
        if(shutdown(new_connection, SD_SEND) != 0) {
            server_log(service, "Error with shutdown(). Error code: ", WSAGetLastError());
        } else {
            server_log(service, "Connection shutdown() OK.", 0);
        }

        // When you are finished sending and receiving data on the
        // NewConnection socket and are finished accepting new connections
        // on ListeningSocket, you should close the sockets using the closesocket API.
        if(closesocket(new_connection) != 0)
            server_log(service, "Cannot close client's socket. Error code: ", WSAGetLastError());
         else
            server_log(service, "Closing client's socket.", 0);
     } // END while

    // Close listening socket
    if(closesocket(listening_socket) != 0)
        server_log(service, "Cannot close server socket. Error code: ", WSAGetLastError());
    else
        server_log(service, "Closing server socket.", 0);

    // Close winsock
    if (WSACleanup() != 0) {
        server_log(service, "WSACleanup failed with error: ", WSAGetLastError());
    } else {
        printf("msg_server > WSACleanup() is OK.", 0);
    }

    return;
}

void discovery_server(void)
{
    char service[20] = "dsc_server";

    // Initialize winsock
    WSADATA ws_data;    // winsock version
    WORD ws_ver;        // pointer to winsock version related data

    ws_ver = MAKEWORD(2, 2);   // Set version to 2.2

    // Start winsock with error checking
    if (WSAStartup(ws_ver, &ws_data) != 0) {
        server_log(service, "WSAStartup failed with error: ", WSAGetLastError());
        return;
    }

    // Create a new socket to listen for client connections.
    SOCKET listening_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check for errors to ensure that the socket is a valid socket.
    if (listening_socket == INVALID_SOCKET) {
        server_log(service, "Error at socket(), error code: ", WSAGetLastError());

        // Clean up, exit
        WSACleanup();
        return;

    // Socket started fine
    } else {
        server_log(service, "socket() is OK!", 0);
    }

    // Set up a SOCKADDR_IN structure that will tell bind that we
    // want to listen for connections on all interfaces using the given port

    int port = d_port;                                  // Set port to discovery port
    SOCKADDR_IN server_addr;
    server_addr.sin_family = AF_INET;                   // IPv4
    server_addr.sin_port = htons(port);                 // host-to-network byte order
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    // Listen on all interface, host-to-network byte order

    // Bind the server address info to the socket created
    // Check for general errors.
    if (bind(listening_socket, (SOCKADDR*) &server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        server_log(service, "bind() failed! Error code: ", WSAGetLastError());

        // Close socket, clean up, exit
        closesocket(listening_socket);
        WSACleanup();
        return;

    } else {

        server_log(service, "bind() is OK!", 0);
    }

    // Define backlog, which is max allowed connection queue length
    static const int backlog = 5;   // 5 is normal for many applications.

    // Listen for client connections.
    if (listen(listening_socket, backlog) == SOCKET_ERROR) {

        server_log(service, "listen(): Error listening on socket: ", WSAGetLastError());

        // Close, cleanup, exit
        closesocket(listening_socket);
        WSACleanup();
        return;

    } else {
        log_msg[0] = '\0';
        server_log(service, "listen() is OK, I'm waiting for connections.", 0);
    }

    server_log(service, "accept() is ready.", 0);

    // Accept a new connection when one arrives.
    SOCKET new_connection;
    SOCKADDR_IN client_addr;                // client's IP address
    int client_addr_size = sizeof(client_addr);
    char recvbuff[1024];
    int bytes_received;

    while(run_msg_server) {

        // Build up connection with client
        new_connection = accept(listening_socket, (SOCKADDR*) &client_addr, &client_addr_size);
        if (new_connection < 0) {
            server_log(service, "error with accept(), cannot connect to client. ", WSAGetLastError());
            break;
        } else {
            server_log(service, "accept() is OK.", 0);
        }

        // Client is connected, ready to receive data
        bytes_received = recv(new_connection, recvbuff, sizeof(recvbuff), 0);

        // When there is data
        if (bytes_received > 0) {

            // FIXME write sender IP or name too
            for(int i =0; i < bytes_received; i++) {
                printf("%c", recvbuff[i]);
                log_msg[i] = recvbuff[i];
            }

            printf("\"\n");

            char msg_buffer[255] = "received:";
            strcat(msg_buffer, recvbuff);
            server_log(service, msg_buffer, 0);
        }

        // Clean up all the send/receive communication, get ready for new one
        if(shutdown(new_connection, SD_SEND) != 0) {
            server_log(service, "Error with shutdown(). Error code: ", WSAGetLastError());
        } else {
            server_log(service, "Connection shutdown() OK.", 0);
        }

        // When you are finished sending and receiving data on the
        // NewConnection socket and are finished accepting new connections
        // on ListeningSocket, you should close the sockets using the closesocket API.
        if(closesocket(new_connection) != 0)
            server_log(service, "Cannot close client's socket. Error code: ", WSAGetLastError());
         else
            server_log(service, "Closing client's socket.", 0);
     } // END while

    // Close listening socket
    if(closesocket(listening_socket) != 0)
        server_log(service, "Cannot close server socket. Error code: ", WSAGetLastError());
    else
        server_log(service, "Closing server socket.", 0);

    // Close winsock
    if (WSACleanup() != 0) {
        server_log(service, "WSACleanup failed with error: ", WSAGetLastError());
    } else {
        printf("msg_server > WSACleanup() is OK.", 0);
    }

    return;
}

void broadcast_server(void)
{
    char service[20] = "brc_server";

    // Initialize winsock
    WSADATA ws_data;    // winsock version
    WORD ws_ver;        // pointer to winsock version related data

    ws_ver = MAKEWORD(2, 2);   // Set version to 2.2

    // Start winsock with error checking
    if (WSAStartup(ws_ver, &ws_data) != 0) {
        server_log(service, "WSAStartup failed with error: ", WSAGetLastError());
        return;
    }

    // Create a new socket to listen for client connections.
    SOCKET listening_socket = socket(AF_INET, SOCK_DGRAM, 0);

    // Check for errors to ensure that the socket is a valid socket.
    if (listening_socket == INVALID_SOCKET) {
        server_log(service, "Error at socket(), error code: ", WSAGetLastError());

        // Clean up, exit
        WSACleanup();
        return;

    // Socket started fine
    } else {
        server_log(service, "socket() is OK!", 0);
    }

    // Set up a SOCKADDR_IN structure that will tell bind that we
    // want to listen for connections on all interfaces using the given port

    int port = m_port;                                  // Set port to message port
    SOCKADDR_IN server_addr;
    server_addr.sin_family = AF_INET;                   // IPv4
    server_addr.sin_port = htons(port);                 // host-to-network byte order
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    // Listen on all interface, host-to-network byte order

    // Bind the server address info to the socket created
    // Check for general errors.
    if (bind(listening_socket, (SOCKADDR*) &server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        server_log(service, "bind() failed! Error code: ", WSAGetLastError());

        // Close socket, clean up, exit
        closesocket(listening_socket);
        WSACleanup();
        return;

    } else {

        server_log(service, "bind() is OK!", 0);
    }

    server_log(service, "Waiting for transmissions.", 0);

    while(1) {

        SOCKADDR_IN client_addr;                // client's IP address
        int client_addr_size = sizeof(client_addr);
        char recvbuff[1024];

        int message = recvfrom(listening_socket, recvbuff, strlen(recvbuff) + 1, 0, (SOCKADDR*) &client_addr, &client_addr_size);
        if(message == -1) {
            server_log(service, "bind() failed! Error code: ", WSAGetLastError());
        } else {
            // FIXME
            // implement function to react to arrived broadcast message

            char msg_buffer[1024] = "received:";
            strcat(msg_buffer, recvbuff);
            server_log(service, msg_buffer, 0);
            printf("discovery-server> received from %s %d %s\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port, recvbuff);
        }
    } // END while

    // Close listening socket
    if(closesocket(listening_socket) != 0)
        server_log(service, "Cannot close server socket. Error code: ", WSAGetLastError());
    else
        server_log(service, "Closing server socket.", 0);

    // Close winsock
    if (WSACleanup() != 0) {
        server_log(service, "WSACleanup failed with error: ", WSAGetLastError());
    } else {
        printf("msg_server > WSACleanup() is OK.", 0);
    }

    return;
}




int client(void)
{

    const char ip_address[] = "127.0.0.1";  // remote address
    int port = 54000;                       // remote port

    // Initialize winsock
    WSADATA ws_data;    // winsock version
    WORD ws_ver;        // pointer to winsock version related data

    ws_ver = MAKEWORD(2, 2);   // Set version to 2.2

    // Start winsock with error checking
    if (WSAStartup(ws_ver, &ws_data) != 0) {
        printf("WSAStartup failed with error %d\n", WSAGetLastError());
        return 1;
    }

    // Create a new socket for sending
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Check for errors to ensure that the socket is a valid socket.
    if (client_socket == INVALID_SOCKET) {
        printf("Client: Error at socket(), error code: %d\n", WSAGetLastError());

        // Clean up, exit
        WSACleanup();
        return 1;

    } else {
        printf("Client: socket() is OK!\n");
    }

    // Create a client interface structure
    SOCKADDR_IN client_if;
    client_if.sin_family = AF_INET;     // IPv4
    client_if.sin_addr.s_addr = inet_addr(ip_address);
    client_if.sin_port = htons(port);

    // Connect to server
    int connection_result = connect(client_socket, (SOCKADDR*) &client_if, sizeof(client_if));
    if (connection_result == SOCKET_ERROR) {

        // On error close socket, cleanup and exit
        printf("Cannot connect to server. Error %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    char buff[4096];
    char user_input[255];

    // Send and receive data
    do {
        printf("client tx>");
        gets(user_input);

        // Make sure user entered some text
        if (strlen(user_input) > 0) {

            // Send message to server
            int send_result = send(client_socket, user_input, strlen(user_input), 0);

            // If no error, wait for response
            if (send_result != SOCKET_ERROR) {
                memset(&buff, 0, sizeof(buff));
                int bytes_received = recv(client_socket, buff, 4096, 0);

                // Echo to console
                if (bytes_received > 0) {
                    printf("client rx>");
                    for (int i = 0; i < bytes_received; i++) {
                        printf("%c", buff[i]);
                    }
                    printf("<\n");
                }

            // Handle error
            } else {
                ;; //TODO
            }
        }

    // After empty line stop sending
    } while (strlen(user_input) > 0);

    // Close socket
    closesocket(client_socket);

    return 0;
}

/*
        // Some info on the receiver side...
        getsockname(listening_socket, (SOCKADDR*) &server_addr, (int*) sizeof(server_addr));
        printf("\n");
        printf("Server: accept() is OK...\n");
        printf("Server: Receiving IP(s) used: %s\n", inet_ntoa(server_addr.sin_addr));
        printf("Server: Receiving port used: %d\n", htons(server_addr.sin_port));

        // Some info on the sender side
        // Allocate the required resources
        memset(&client_addr, 0, sizeof(client_addr));
        getpeername(new_connection, (SOCKADDR*) &client_addr, &client_addr_size);
        printf("Server: Sending IP used: %s\n", inet_ntoa(client_addr.sin_addr));
        printf("Server: Sending port used: %d\n", htons(client_addr.sin_port));

        printf("Server: Client connected, ready for receiving and sending data\n");

*/
