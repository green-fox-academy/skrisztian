/* Includes ------------------------------------------------------------------*/
#include "socket_server.h"
#include "socket_client.h"
#include "stm32746g_discovery_ts.h"
#include "lcd_log.h"
#include "cmsis_os.h"
#include "app_ethernet.h"
#include "lwip/sockets.h"
#include "stm32746g_discovery_ts.h"
#include <string.h>


// Server thread waiting for paint data
void socket_server_thread(void const *argument)
{
	// Server address structure initialization
	struct sockaddr_in addr_in;                                 // Inet address structure definition
	addr_in.sin_family = AF_INET;                               // This address is an internet address
	addr_in.sin_port = htons(54545);                      		// Server port
	addr_in.sin_addr.s_addr = 0;             					// Server IP
	struct sockaddr *addr = (struct sockaddr *)&addr_in;        // Make a struct sockaddr pointer, which points to the address stucture

	// Create socket for server
	int master_socket = socket(PF_INET, SOCK_STREAM, 0);
	int flag = bind(master_socket, addr, sizeof(*addr)); //flag for errors
	if (flag < 0)
		LCD_ErrLog((char *)"Binding failed.\n");

	flag = listen(master_socket, QUEUE_SIZE);
	// Check if listening is ok
	if (flag < 0)
		LCD_ErrLog((char *)"Listening failed\n");

	// Client socket and address
	struct sockaddr client_addr;    		// Client address structure
	int slave_socket;              			// Slave socket definition, this will be used to store the incoming socket

	// Buffer and structure for touch screen data
	uint8_t buff[SERVER_BUFF_LEN];    		// Buffer for incoming and outgoing data
	screen_t paint;							//structure containing info about the LCD screen
	display_menu();

	while (1) {
		osDelay(10);

		// Accept the connection and save the incoming socket
		slave_socket = accept(master_socket, &client_addr, NULL);
		// Check if the socket is valid
		if (slave_socket < 0) {
			//LCD_ErrLog((char *)"Connection not accepted.\n");
			osDelay(5);
		}

		//LCD_UsrLog((char *)"Connection accepted.\n");

		// Receive the data sent by the client
		int received_bytes;
		do {
			received_bytes = recv(slave_socket, buff, SERVER_BUFF_LEN, 0);
			if (received_bytes == 0) {
				osDelay(5);
			} else if (received_bytes == SOCKET_ERROR) {
				osDelay(10);
				break;
			} else {
				//de-serialize
				deserialize_screen_data(buff, &paint);
				// Print out received data for DEBUGGING
				/*char coord[50];
				sprintf(coord, "X coord: %d Y coord: %d\n", paint.x, paint.y);
				LCD_UsrLog(coord);*/
				//Draw circle at coordinates
				if (paint.action == 3) {
					//BSP_LCD_SelectLayer(PAINT_LAYER);
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					display_menu();
				} else if (paint.action == 1){
					BSP_LCD_SetTextColor(paint.color);
					BSP_LCD_FillCircle(paint.x, paint.y, paint.l_weight);
				}
			}
		} while (received_bytes > 0);

		closesocket(slave_socket);

		}

	close(master_socket);
	while (1) {
		osThreadTerminate(NULL);
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
