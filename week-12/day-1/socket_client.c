#include "socket_client.h"

void socket_client_thread(void const *argument)
{
	// char remote_ip[20] = REMOTE_IP;
	// int remote_port = REMOTE_PORT;
	
	// Temp string buffer for LCD log messsages
	char temp[100];
	
	// Buffer to send data by TCP client
	char send_buffer[CLIENT_BUFF_LEN];

	// Create a client socket
	int client_socket;
	client_socket = lwip_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client_socket < 0) {
		log_msg(ERROR, "Cannot create TCP client socket\n");
		log_msg(ERROR, "Closing application\n");
		osThreadTerminate(NULL);
	} else {
		log_msg(USER, "TCP client socket is up.\n");
	}

	// Create client address structure
	struct sockaddr_in client_addr;
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = inet_addr(REMOTE_IP);
	client_addr.sin_port = htons(REMOTE_PORT);

	// Connect to server
	for (int i = 0; i < MAX_TRY; i++) {
		sprintf(temp, "Trying to connect to %s:%d\n", REMOTE_IP, REMOTE_PORT);
		log_msg(USER, temp);

		// Test connection
		int connection_result = lwip_connect(client_socket, (struct sockaddr*) &client_addr, sizeof(client_addr));
		if (connection_result < 0) {
			sprintf(temp, "Attempt %d failed\n", i + 1);
			log_msg(DEBUG, temp);
		} else {
			log_msg(USER, "Connection successful.\n");
			break;
		}
		osDelay(1000);
	}

	if (connection_result < 0) {
		log_msg(ERROR, "Giving up. Closing TCP client.\n");
		lwip_close(client_socket);
		while (1) {
			osThreadTerminate(NULL);
		}
	}

	// Send data
	// Remove while loop, if send should occure once
	while (1) {

		// Give processor time to other threads too
		osDelay(10);

		// Send buffer init is at the beginning of this function
		// If data sent is binary and not string use sizeof() below
		size_t send_buffer_size = strlen(send_buffer);
		int send_result = lwip_send(client_socket, &send_buffer, send_buffer_size, 0);

		if (send_result < 0) {
			log_msg(ERROR, "TCP client could not send data.\n");
			log_msg(ERROR, "Closing connection.\n");
			break;
		} else {
			sprintf(temp, "TCP client sent %d bytes\n", send_result);
			log_msg(USER, temp);
		}
	}

	// Close socket
	lwip_close(client_socket);

	// End thread
	while (1) {
		log_msg(ERROR, "TCP client thread terminating\n");
		osThreadTerminate(NULL);
	}
}