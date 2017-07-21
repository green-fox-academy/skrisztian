#include tcp_client.h

int tcp_client(void)
{

	char service[20] = "tcp_client";
	char send_buffer[TCP_CLIENT_BUFFER_LEN];
    
    // Create a new socket for sending
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check for errors 
    if (client_socket == INVALID_SOCKET) {
        server_log(service, "Client: Error at socket(), error code: %d\n", WSAGetLastError());
        return -1;
    } else {
        server_log(service, "Client: socket() is OK", 0);
    }

    // Create a client interface structure
    SOCKADDR_IN client_if;
    client_if.sin_family = AF_INET;
    client_if.sin_addr.s_addr = htonl(TCP_REMOTE_IP);
    client_if.sin_port = htons(TCP_REMOTE_PORT);

    // Connect to server
    int connection_result = connect(client_socket, (SOCKADDR*) &client_if, sizeof(client_if));
    if (connection_result == SOCKET_ERROR) {
        server_log(service, "Cannot connect to server. Error ", WSAGetLastError());
        closesocket(client_socket);
        return -1;
    }

    // Send data
    int send_result = send(client_socket, send_buffer, strlen(send_buffer), 0);

    char temp[1024];
	sprintf(temp, "%s:%d %s", TCP_REMOTE_IP, TCP_REMOTE_PORT, send_buffer);

    // Check error
    if (send_result != SOCKET_ERROR) {
        printf("%s>%s\n", service, temp);
        server_log(service, temp, send_result);
    } else {
        int error = WSAGetLastError();
        printf("%s>Error sending msg: %s", temp);
        server_log(service, temp, send_result);
    }
	
    // Close socket
    closesocket(client_socket);
	server_log(service, "Closing client socket", 0);
	
    return 0;
}