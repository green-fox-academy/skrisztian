#include "udp_client.h"

int udp_client(void)
{
    char service[20] = "udp_client";
	char send_buffer[UDP_SEND_BUFFER_LEN];

    // Create a new socket for sending
    SOCKET client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Check for errors to ensure that the socket is a valid socket.
    if (client_socket == INVALID_SOCKET) {
        server_log(service, "Error at socket(), error code: ", WSAGetLastError());
        return -1;
    } else {
        server_log(service, "socket() is OK.", 0);
    }

    // Create a client interface structure
    SOCKADDR_IN client_if;
    client_if.sin_family = AF_INET;

    // Create remote interface structure
    SOCKADDR_IN remote_address;
    remote_address.sin_family = AF_INET;
    remote_address.sin_addr.s_addr = inet_addr(UDP_REMOTE_IP);
    remote_address.sin_port = htons(UDP_REMOTE_PORT);

    /* Set socket to broadcast */
    int opt_val = 1;
    int opt_len = sizeof(int);
    setsockopt(client_socket, SOL_SOCKET, SO_BROADCAST, (char*) &opt_val, opt_len);
	/* End set socket to broadcast */
	
     // Send data
    int send_result = sendto(client_socket, send_buffer, strlen(send_buffer), 0, (SOCKADDR*) &remote_address, sizeof(remote_address));

    // Log error
    if (send_result == SOCKET_ERROR) {
        server_log(service, "Error sending message", WSAGetLastError());
        printf("%s>Error sending message %d\n", %service, WSAGetLastError());
    } else {
        printf("%s>Sent %s\n", service, send_buffer);
    }

    // Close socket
    closesocket(client_socket);
	server_log(service, "Closing client socket", 0);

    return 0;
}
