#include "udp_server.h"

void udp_server_thread(void)
{
    char service[20] = "udp_server";

    // Create a new socket to listen for client connections.
    SOCKET udp_server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Check for errors to ensure that the socket is a valid socket.
    if (udp_server_socket == INVALID_SOCKET) {
        server_log(service, "Error at socket(), error code: ", WSAGetLastError());
        return;

    // Socket started fine
    } else {
        server_log(service, "socket() is OK!", 0);
    }

    // Set up a SOCKADDR_IN structure that will tell bind that we
    // want to listen for connections on all interfaces using the given port
    SOCKADDR_IN udp_server_addr;
    udp_server_addr.sin_family = AF_INET;
    udp_server_addr.sin_port = htons(UDP_SERVER_PORT);
    udp_server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the server address info to the socket created
    // Check for general errors.
    if (bind(udp_server_socket, (SOCKADDR*) &udp_server_addr, sizeof(udp_server_addr)) == SOCKET_ERROR) {
        server_log(service, "bind() failed! Error code: ", WSAGetLastError());
        closesocket(udp_server_socket);
        return;
    } else {
        server_log(service, "bind() is OK!", 0);
    }

    server_log(service, "Waiting for UDP transmissions.", 0);
    while(1) {

        SOCKADDR_IN udp_client_addr;
        int udp_client_addr_size = sizeof(udp_client_addr);
        char recvbuff[1024];

        int message = recvfrom(udp_server_socket, recvbuff, sizeof(recvbuff), 0, (SOCKADDR*) &udp_client_addr, &udp_client_addr_size);
        recvbuff[message] = 0;
        if(message > 0) {
            server_log(service, recvbuff, 0);
            printf("%s>%s %d %s\n", service, inet_ntoa(udp_client_addr.sin_addr), udp_client_addr.sin_port, recvbuff);
        }
    } // END while

    closesocket(udp_server_socket);
    while (1) {
        _endthread();
    }
}
