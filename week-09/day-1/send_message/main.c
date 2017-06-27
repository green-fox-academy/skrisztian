#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>

// Trailing \0 string terminators are not sent!!!


// A sample of the select() return value
int recv_time_out_tcp(SOCKET socket, long sec, long usec)
{
    // Setup timeval variable
    struct timeval timeout;
    struct fd_set fds;

     // assign the second and microsecond variables
    timeout.tv_sec = sec;
    timeout.tv_usec = usec;

    // Setup fd_set structure
    FD_ZERO(&fds);
    FD_SET(socket, &fds);

    // Possible return values:
    // -1: error occurred
    // 0: timed out
    // > 0: data ready to be read
    return select(0, &fds, 0, 0, &timeout);
}

int server(void)
{
    // Initialize winsock
    WSADATA ws_data;    // winsock version
    WORD ws_ver;        // pointer to winsock version related data

    ws_ver = MAKEWORD(2, 2);   // Set version to 2.2

    // Start winsock with error checking
    if (WSAStartup(ws_ver, &ws_data) != 0) {
        printf("WSAStartup failed with error %d\n", WSAGetLastError());
        return 1;
    }

    // Create a new socket to listen for client connections.
    SOCKET listening_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check for errors to ensure that the socket is a valid socket.
    if (listening_socket == INVALID_SOCKET) {
        printf("Server: Error at socket(), error code: %d\n", WSAGetLastError());

        // Clean up, exit
        WSACleanup();
        return 1;

    } else {
        printf("Server: socket() is OK!\n");
    }

    // Set up a SOCKADDR_IN structure that will tell bind that we
    // want to listen for connections on all interfaces using the given port

    int port = 54000;                                    // Set port to 5150;
    SOCKADDR_IN server_addr;
    server_addr.sin_family = AF_INET;                   // IPv4
    server_addr.sin_port = htons(port);                 // host-to-network byte order
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    // Listen on all interface, host-to-network byte order

    // Bind the server address info to the socket created
    // Check for general errors.
    if (bind(listening_socket, (SOCKADDR*) &server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Server: bind() failed! Error code: %ld.\n", WSAGetLastError());

        // Close socket, clean up, exit
        closesocket(listening_socket);
        WSACleanup();
        return 1;

    } else {
        printf("Server: bind() is OK!\n");
    }

    // Define backlog, which is max allowed connection queue length
    static const int backlog = 5;   // 5 is normal for many applications.

    // Listen for client connections.
    if (listen(listening_socket, backlog) == SOCKET_ERROR) {
        printf("Server: listen(): Error listening on socket %ld.\n", WSAGetLastError());

        // Close, cleanup, exit
        closesocket(listening_socket);
        WSACleanup();
        return 1;

    } else {
        printf("Server: listen() is OK, I'm waiting for connections...\n");
    }

    printf("Server: accept() is ready...\n");

    // Accept a new connection when one arrives.
    SOCKET new_connection;
    SOCKADDR_IN client_addr;                // client's IP address
    int client_addr_size = sizeof(client_addr);
    char recvbuff[1024];
    int bytes_received;

    // Build up connection with client
    new_connection = SOCKET_ERROR;
    while(new_connection == SOCKET_ERROR) {
        new_connection = accept(listening_socket, (SOCKADDR*) &client_addr, &client_addr_size);
    }

    printf("Server: accept() is OK...\n");
    printf("Server: Client connected, ready for receiving and sending data\n");

    // Transfer the connected socket to the listening socket
    listening_socket = new_connection;



    while(1) {
/*
        // Build up connection with client
        new_connection = SOCKET_ERROR;
        while(new_connection == SOCKET_ERROR) {
            new_connection = accept(listening_socket, (SOCKADDR*) &client_addr, &client_addr_size);
        }

        printf("Server: accept() is OK...\n");
        printf("Server: Client connected, ready for receiving and sending data\n");
*/
        // Transfer the connected socket to the listening socket
        listening_socket = new_connection;

        // At this point you can do two things with these sockets
        // 1. Wait for more connections by calling accept again
        //    on ListeningSocket (loop)
        // 2. Start sending or receiving data on NewConnection.

        bytes_received = recv(new_connection, recvbuff, sizeof(recvbuff), 0);

        // When there is data
        if (bytes_received > 0) {
            printf("Server: recv() looks fine....\n");

            // Some info on the receiver side...
            getsockname(listening_socket, (SOCKADDR*) &server_addr, (int*) sizeof(server_addr));
            printf("Server: Receiving IP(s) used: %s\n", inet_ntoa(server_addr.sin_addr));
            printf("Server: Receiving port used: %d\n", htons(server_addr.sin_port));

            // Some info on the sender side
            // Allocate the required resources
            memset(&client_addr, 0, sizeof(client_addr));
            getpeername(new_connection, (SOCKADDR*) &client_addr, &client_addr_size);
            printf("Server: Sending IP used: %s\n", inet_ntoa(client_addr.sin_addr));
            printf("Server: Sending port used: %d\n", htons(client_addr.sin_port));

            // Print the received bytes. Take note that this is the total
            // byte received, it is not the size of the declared buffer
            printf("Server: Bytes received: %d\n", bytes_received);
            // Print what those bytes represent
            printf("Server: Those bytes are: \"");
            // Print the string only, discard other
            // remaining 'rubbish' in the 1024 buffer size
            for(int i =0; i < bytes_received; i++) {
                printf("%c", recvbuff[i]);
            }
            printf("\"");

            // Send back the received data
            int send_result = send(listening_socket, recvbuff, bytes_received, 0);
            if (send_result == SOCKET_ERROR) {
                    printf("Server: send() failed with error code: %d\n", WSAGetLastError());
                    break;
            }

        // No data
        } else if (bytes_received == 0) {
            printf("Server: Connection closed!\n");
            break;

        // Error
        } else {
          printf("Server: recv() failed with error code: %d\n", WSAGetLastError());
          break;
        }
    }

    // Clean up all the send/receive communication, get ready for new one
    if(shutdown(new_connection, SD_SEND) != 0)
        printf("\nServer: Error with shutdown(). Error code: %ld\n", WSAGetLastError());
    else
        printf("\nServer: shutdown() looks OK...\n");



    // When you are finished sending and receiving data on the
    // NewConnection socket and are finished accepting new connections
    // on ListeningSocket, you should close the sockets using the closesocket API.
    if(closesocket(new_connection) != 0)
      printf("Server: Cannot close \"NewConnection\" socket. Error code: %ld\n", WSAGetLastError());
    else
      printf("Server: Closing \"NewConnection\" socket...\n");



    // Close winsock
    if (WSACleanup() != 0) {
        printf("WSACleanup failed with error %d\n", WSAGetLastError());
        return 1;
    } else {
        printf("Server: WSACleanup() is OK...\n");
    }

    return 0;
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
        printf("client>");
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
                    printf("server>");
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

// Create Server and Client Applications that send and receive messages.
int main()
{
    char cmd[10];

    printf("Enter 'c' for client, 's' for server\n");
    printf(">");

    gets(cmd);

    switch (cmd[0]) {
    case 'c':
        client();
        break;
    case 's':
        server();
        break;
    default:
        printf("Command not recognized.\n");
        break;
    }

    return 0;
}
