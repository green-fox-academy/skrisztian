#include "comms.h"
#include <stdlib.h>
#include "p_utils.h"

/*
 * This file contains the communications related functions
 * e.g. server, client, etc.
 */

int init_winsock(void)
{

    char service[20] = "winsock";

    // Initialize winsock
    WSADATA ws_data;    // winsock version
    WORD ws_ver;        // pointer to winsock version related data

    ws_ver = MAKEWORD(2, 2);   // Set version to 2.2

    // Start winsock with error checking
    if (WSAStartup(ws_ver, &ws_data) != 0) {
        int error = WSAGetLastError();
        server_log(service, "WSAStartup failed with error: ", error);
        return error;
    } else {
        server_log(service, "WSAStartup OK.", 0);
    }

    return 0;
}

int close_winsock(void)
{
    char service[20] = "winsock";

    // Close winsock
    if (WSACleanup() != 0) {
       int error = WSAGetLastError();
        server_log(service, "WSACleanup failed with error: ", error);
        return error;
    } else {
        server_log(service, "WSAStartup OK.", 0);
    }

    return 0;
}

void message_listener(void)
{
    char service[20] = "msg_server";

    // Create a new socket to listen for client connections.
    SOCKET listening_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check for errors to ensure that the socket is a valid socket.
    if (listening_socket == INVALID_SOCKET) {
        server_log(service, "Error at socket(), error code: ", WSAGetLastError());

        // Clean up, exit
        return;

    // Socket started fine
    } else {
        server_log(service, "socket() is OK!", 0);
    }

    // Set up a SOCKADDR_IN structure that will tell bind that we
    // want to listen for connections on all interfaces using the given port

    SOCKADDR_IN server_addr;
    server_addr.sin_family = AF_INET;                   // IPv4
    server_addr.sin_port = htons(m_port);               // set port to message listener port
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    // Listen on all interface, host-to-network byte order

    // Bind the server address info to the socket created
    // Check for general errors.
    if (bind(listening_socket, (SOCKADDR*) &server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        server_log(service, "bind() failed! Error code: ", WSAGetLastError());

        // Close socket, clean up, exit
        closesocket(listening_socket);
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

    while(1) {

        // Build up connection with client
        new_connection = accept(listening_socket, (SOCKADDR*) &client_addr, &client_addr_size);
        if (new_connection < 0) {
            server_log(service, "error with accept(), cannot connect to client. ", WSAGetLastError());

        } else {
            server_log(service, "accept() is OK.", 0);
        }

        // Client is connected, ready to receive data
        bytes_received = recv(new_connection, recvbuff, sizeof(recvbuff), 0);

        // When there is data
        if (bytes_received > 0) {

            printf("message_listener>%s\n", recvbuff);
            server_log(service, recvbuff, 0);
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

    return;
}

void discovery_listener(void)
{
    char service[20] = "dsc_server";

    // Create a new socket to listen for client connections.
    SOCKET listening_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check for errors to ensure that the socket is a valid socket.
    if (listening_socket == INVALID_SOCKET) {
        server_log(service, "Error at socket(), error code: ", WSAGetLastError());

        // Clean up, exit
        return;

    // Socket started fine
    } else {
        server_log(service, "socket() is OK!", 0);
    }

    // Set up a SOCKADDR_IN structure that will tell bind that we
    // want to listen for connections on all interfaces using the given port

    SOCKADDR_IN server_addr;
    server_addr.sin_family = AF_INET;                   // IPv4
    server_addr.sin_port = htons(d_port);               // discovery listener port
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    // Listen on all interface, host-to-network byte order

    // Bind the server address info to the socket created
    // Check for general errors.
    if (bind(listening_socket, (SOCKADDR*) &server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        server_log(service, "bind() failed! Error code: ", WSAGetLastError());

        // Close socket, clean up, exit
        closesocket(listening_socket);
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

    while(1) {

        // Build up connection with client
        new_connection = accept(listening_socket, (SOCKADDR*) &client_addr, &client_addr_size);
        if (new_connection < 0) {
            server_log(service, "error with accept(), cannot connect to client. ", WSAGetLastError());
            break;
        } else {
            server_log(service, "accept() is OK.", 0);
        }

        // Client is connected, ready to receive data
        memset(recvbuff, 0, sizeof(recvbuff));
        bytes_received = recv(new_connection, recvbuff, sizeof(recvbuff), 0);

        // When there is data
        if (bytes_received > 0) {

           printf("discovery_listener>%s\n", recvbuff);
           server_log(service, recvbuff, 0);
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

    return;
}

void broadcast_listener(void)
{
    char service[20] = "brc_listener";

    // Create a new socket to listen for client connections.
    SOCKET listening_socket = socket(AF_INET, SOCK_DGRAM, 0);

    // Check for errors to ensure that the socket is a valid socket.
    if (listening_socket == INVALID_SOCKET) {
        server_log(service, "Error at socket(), error code: ", WSAGetLastError());
        return;

    // Socket started fine
    } else {
        server_log(service, "socket() is OK!", 0);
    }

    // Set up a SOCKADDR_IN structure that will tell bind that we
    // want to listen for connections on all interfaces using the given port

    SOCKADDR_IN server_addr;
    server_addr.sin_family = AF_INET;                   // IPv4
    server_addr.sin_port = htons(b_port);               // host-to-network byte order
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    // Listen on all interface, host-to-network byte order

    // Bind the server address info to the socket created
    // Check for general errors.
    if (bind(listening_socket, (SOCKADDR*) &server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        server_log(service, "bind() failed! Error code: ", WSAGetLastError());

        // Close socket, clean up, exit
        closesocket(listening_socket);
        return;

    } else {

        server_log(service, "bind() is OK!", 0);
    }

    server_log(service, "Waiting for transmissions.", 0);

    while(1) {

        SOCKADDR_IN client_addr;                // client's IP address
        int client_addr_size = sizeof(client_addr);
        char recvbuff[1024] = "";

        int message = recvfrom(listening_socket, recvbuff, strlen(recvbuff) + 1, 0, (SOCKADDR*) &client_addr, &client_addr_size);
        if(message == -1) {
            server_log(service, "Received failed! Error code: ", WSAGetLastError());
        } else {
            // FIXME
            // implement function to react to arrived broadcast message

            server_log(service, recvbuff, 0);
            printf("discovery-server> received from %s %d %s\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port, recvbuff);

            // Get sender's data
            char *token;

            token = strtok(recvbuff, " ");

            if (strcmp(token, "TOTORO")) {
                token = strtok(NULL, " ");
                printf("port: %s\n", token);

                char m_port_s[10];
                itoa(m_port, m_port_s, 10);

                // Send out message to discovery port
                char msg_to_send[] = "";
                strcmp(msg_to_send, hostname);
                strcat(msg_to_send, " ");
                strcat(msg_to_send, m_port_s);
                send_msg(inet_ntoa(client_addr.sin_addr), atoi(token), msg_to_send);
            }
        }
    } // END while

    // Close listening socket
    if(closesocket(listening_socket) != 0)
        server_log(service, "Cannot close server socket. Error code: ", WSAGetLastError());
    else
        server_log(service, "Closing server socket.", 0);

    return;
}




int send_msg(char* remote_ip, int remote_port, char* message)
{
    char service[20] = "send_msg";

    // Create a new socket for sending
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Check for errors to ensure that the socket is a valid socket.
    if (client_socket == INVALID_SOCKET) {
        server_log(service, "Client: Error at socket(), error code: %d\n", WSAGetLastError());
        return -1;
    } else {
        server_log(service, "Client: socket() is OK", 0);
    }

    // Create a client interface structure
    SOCKADDR_IN client_if;
    client_if.sin_family = AF_INET;
    client_if.sin_addr.s_addr = inet_addr(remote_ip);
    client_if.sin_port = htons(remote_port);

    // Connect to server
    int connection_result = connect(client_socket, (SOCKADDR*) &client_if, sizeof(client_if));
    if (connection_result == SOCKET_ERROR) {

        // On error close socket, cleanup and exit
        server_log(service, "Cannot connect to server. Error ", WSAGetLastError());
        printf("Cannot connect to server. Error %d\n", WSAGetLastError());
        closesocket(client_socket);
        return -2;
    }

    char buff[1200];

    // Send data
    int send_result = send(client_socket, message, strlen(message), 0);

    // Prepare for logging
    char temp1[1024] = "";
    strcat(temp1, remote_ip);
    char temp2[10];
    itoa(remote_port, temp2, 10);
    strcat(temp1, " ");
    strcat(temp1, temp2);
    strcat(temp1, " ");
    strcat(temp1, message);

    // Check error
    if (send_result != SOCKET_ERROR) {
        printf("sent msg: >%s< to %s %d\n", message, remote_ip, remote_port);
        server_log(service, temp1, 0);
    } else {
        int error = WSAGetLastError();
        printf("Error sending msg: >%s< to %s %d, error: %d\n", message, remote_ip, remote_port, error);
        server_log(service, temp1, error);
    }
    // Close socket
    closesocket(client_socket);

    return 0;
}

int send_broadcast(int remote_port)
{
    const char ip_address[] = "255.255.255.255";    // remote address
    char service[20] = "broadcast_client";

    // Create a new socket for sending
    SOCKET client_socket = socket(AF_INET, SOCK_DGRAM, 0);

    // Check for errors to ensure that the socket is a valid socket.
    if (client_socket == INVALID_SOCKET) {
        server_log(service, "Error at socket(), error code: ", WSAGetLastError());
        // Clean up, exit
        return 1;

    } else {
        server_log(service, "socket() is OK.", 0);
    }

    // Create a client interface structure
    SOCKADDR_IN client_if;
    client_if.sin_family = AF_INET;     // IPv4

    // Create remote interface structure
    SOCKADDR_IN remote_address;
    remote_address.sin_family = AF_INET;
    remote_address.sin_addr.s_addr = inet_addr(ip_address);
    remote_address.sin_port = htons(remote_port);

    // Set socket to broadcast
    int opt_val = 1;
    int opt_len = sizeof(int);
    setsockopt(client_socket, SOL_SOCKET, SO_BROADCAST, (char*) &opt_val, opt_len);

    // Construct message
    char temp[20];
    itoa(d_port, temp, 10);     // convert discovery port number to string
    char broadcast_msg[100] = "TOTORO ";
    strcat(broadcast_msg, temp);

    // Send and receive data
    int send_result = sendto(client_socket, broadcast_msg, strlen(broadcast_msg), 0, (SOCKADDR*) &remote_address, sizeof(remote_address));

    // Log error
    if (send_result == SOCKET_ERROR) {
        server_log(service, "Error sending UDP message: ", WSAGetLastError());
        printf("Error sending UDP message: %d\n", WSAGetLastError());

    } else {
        printf("UDP sent: %s\n", broadcast_msg);
    }

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
