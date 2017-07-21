#include "socket_server.h"

void socket_server_thread(void const *argument)
{
	// Define params
	int server_socket;
	struct sockaddr_in server_addr;

	// Create server socket
	server_socket = lwip_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_socket < 0)
		log_msg(ERROR, "Cannot create TCP socket\n");
		osThreadTerminate(NULL);
	} else {
		log_msg(USER, "TCP socket created\n");
	}

	// Create sockaddr_in structure
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_len = sizeof(server_addr);

	// Bind socket to port
	int bind_result = lwip_bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr));
	if (bind_result < 0) {
		log_msg(ERROR, "Cannot bind TCP socket\n");
		lwip_close(server_socket);
		osThreadTerminate(NULL);
	} else {
		log_msg(USER, "TCP socket bind successful\n");
	}

	// Listen for incoming connections
	if (lwip_listen(server_socket, SERVER_QUEUE_SIZE) != 0 ) {
		log_msg(ERROR, "TCP server listen failed\n");
		lwip_close(server_socket);
		osThreadTerminate(NULL);
	} else {
		log_msg(USER, "TCP server listen active\n");
	}

	// Accept incoming connection -> client socket
	while (1) {

		// Define client params
		int client_socket;
		struct sockaddr_in client_addr;
        int client_addr_len = sizeof(client_addr);
	    char buffer[SERVER_BUFF_LEN];
	    int received_bytes;

	    // Let other threads to get some processor time too
	    osDelay(10);

	    // Accept connection
	    client_socket = lwip_accept(server_socket, (struct sockaddr*) &client_addr, (socklen_t*) &client_addr_len);

	    // Receive message
	    if (client_socket > 0) {
	    log_msg(USER, "Client connected\n");
		do {
	    		received_bytes = lwip_recv(client_socket, buffer, sizeof(buffer),0);
				buffer[received_bytes] = '\0;
				log_msg(USER, "received:");
				log_msg(USER, buffer);
				log_msg(USER, "\n");
		} while (received_bytes > 0);

		// Close client connection
		int close_result = lwip_close(client_socket);
		if (close_result < 0)
			log_msg(ERROR, "Cannot close TCP client socket\n");
		else 
			log_msg(USER, "TCP client socket closed\n");
	}

	// Close server socket
	while(1) {
		int close_result = lwip_close(server_socket);
		if (close_result < 0)
			log_msg(ERROR, "Cannot close TCP servre socket\n");
		else 
			log_msg(USER, "TCP server socket closed\n");
		
		log_msg(ERROR, "TCP thread terminating\n");
		osThreadTerminate(NULL);
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
