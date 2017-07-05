/* Includes ------------------------------------------------------------------*/
#include "socket_client.h"
#include "socket_server.h"
#include "lcd_log.h"
#include "cmsis_os.h"
#include "app_ethernet.h"
#include "lwip/sockets.h"
#include "stm32746g_discovery_ts.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void socket_client_thread(void const *argument)
{
	char remote_ip[20] = "10.27.6.120";
	int remote_port = 54545;

	menu_start_x = 430;
	screen_t_size = 11;

	screen_t touch_current;
	screen_t touch_previous;

	// Initialize touch data
	touch_current.touch = 0;
	touch_current.color = LCD_COLOR_RED;
	touch_current.l_weight = 6;
	touch_current.action = 0;

	screen_t test_data;
	test_data.touch = 1;
	test_data.x = 240;
	test_data.y = 136;
	test_data.color = 2;
	test_data.l_weight = 2;
	test_data.action = 0;


	TS_StateTypeDef TS_State;

	// Wait for IP address
	while(!is_ip_ok())
		osDelay(100);

	// Create a client socket
	int client_socket;
	client_socket = lwip_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client_socket < 0) {
		LCD_ErrLog((char*) "Cannot create client socket\n");
		LCD_ErrLog((char*) "Closing application\n");
		osThreadTerminate(NULL);
	} else {
		LCD_UsrLog((char*) "Client socket is up.\n");
	}

	// Create client address structure
	struct sockaddr_in client_addr;
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = inet_addr(remote_ip);
	client_addr.sin_port = htons(remote_port);

	// Connect to server
	int max_try = 60;
	int connection_result;
	for (int i = 0; i < max_try; i++) {
		LCD_UsrLog((char*) "Trying to connect to ");
		LCD_UsrLog((char*) remote_ip);
		LCD_UsrLog((char*) "\n");
		connection_result = lwip_connect(client_socket, (struct sockaddr*) &client_addr, sizeof(client_addr));
		if (connection_result < 0) {
			LCD_ErrLog((char*) "Connection failed\n");
		} else {
			LCD_UsrLog((char*) "Connection successful.\n");
			break;
		}
		osDelay(1000);
	}

	if (connection_result < 0) {
		LCD_ErrLog((char*) "Giving up. Closing application.\n");
		lwip_close(client_socket);
		osThreadTerminate(NULL);
	}

	osDelay(1000);

	// Init touch screen
	max_x = (uint16_t) BSP_LCD_GetXSize();
	max_y = (uint16_t) BSP_LCD_GetYSize();
	BSP_TS_Init(max_x, max_y);

	// Clear screen, draw menu
	BSP_LCD_SelectLayer(1);
	BSP_LCD_Clear(LCD_COLOR_WHITE);
	BSP_LCD_SelectLayer(0);
	display_menu();
	BSP_LCD_SelectLayer(1);
	BSP_LCD_SetTextColor(LCD_COLOR_RED);

	// Send touch data
	while (1) {

		// Give processor time to other threads too
		osDelay(10);

		// Read touch values
		BSP_TS_GetState(&TS_State);

		// One finger: drawing
		if (TS_State.touchDetected) {

			// Continuous touch -> save previous state
			if (touch_current.touch)
				save_touch(&touch_current, &touch_previous);

			// Get current state

			get_coordinates(&touch_current, &TS_State);

			// Two+ fingers -> clear screen
			if (TS_State.touchDetected > 1 && touch_current.touch
			    && touch_current.action == 1) {

				// Only if there is some movement
				if (touch_current.x - touch_previous.x != 0 ||
				    touch_current.y - touch_previous.y != 0) {
					touch_current.action = 3;
					BSP_LCD_Clear(LCD_COLOR_WHITE);
					BSP_LCD_SelectLayer(0);
					display_menu();
					BSP_LCD_SelectLayer(1);
				}
			// One finger
			} else {

				// Draw a circle at touch
				if (touch_current.action == 1) {
					BSP_LCD_SetTextColor(touch_current.color);
					BSP_LCD_FillCircle(touch_current.x, touch_current.y, touch_current.l_weight);
				} else if (touch_current.action == 2) {
					set_pen(&touch_current);
				}
			}

			touch_current.touch = 1;

			// Debug
			// print_point(&touch_current);

			int sent = send_screen(client_socket, &touch_current);
			if (!sent) {
				LCD_ErrLog((char*) "Could not send data.\n");
				LCD_ErrLog((char*) "Closing connection.\n");
				break;
			}
		} else {
			touch_current.touch = 0;
			touch_current.action = 0;
		}
	}

	// Close socket
	lwip_close(client_socket);

	// End thread
	while (1) {
		osThreadTerminate(NULL);
	}
}

uint8_t* serialize_uint32(uint8_t *buffer, uint32_t value)
{
  // Write big-endian int value into buffer
    buffer[0] = value >> 24;
    buffer[1] = value >> 16;
    buffer[2] = value >> 8;
    buffer[3] = value;
    return buffer + 4;
}

uint8_t* serialize_uint16(uint8_t *buffer, uint16_t value)
{
  // Write big-endian int value into buffer
  // Assumes 32-bit int and 8-bit char.
    buffer[0] = value >> 8;
    buffer[1] = value;
    return buffer + 2;
}

uint8_t* serialize_uint8(uint8_t *buffer, uint8_t value)
{
    buffer[0] = value;
    return buffer + 1;
}

uint8_t* serialize_screen_data(uint8_t *buffer, screen_t* screen_data)
{
    buffer = serialize_uint8(buffer, screen_data->touch);
    buffer = serialize_uint16(buffer, screen_data->x);
    buffer = serialize_uint16(buffer, screen_data->y);
    buffer = serialize_uint32(buffer, screen_data->color);
    buffer = serialize_uint8(buffer, screen_data->l_weight);
    buffer = serialize_uint8(buffer, screen_data->action);

    return buffer;
}

uint8_t send_screen(int socket, screen_t* screen_data)
{
    uint8_t buffer[screen_t_size];
    uint8_t* ptr;

    ptr = serialize_screen_data(buffer, screen_data);
    return (lwip_send(socket, buffer, ptr - buffer, 0) == ptr - buffer);
}

void deserialize_screen_data(uint8_t* buffer, screen_t* screen_data)
{
    screen_data->touch = buffer[0];
    screen_data->x = buffer[1] << 8 | buffer[2];
    screen_data->y = buffer[3] << 8 | buffer[4];
    screen_data->color = buffer[5] << 24 | buffer[6] << 16 | buffer[7] << 8 | buffer[8];
    screen_data->l_weight = buffer[9];
    screen_data->action = buffer[10];

    return;
}

void save_touch(screen_t* touch_current, screen_t* touch_previous)
{
	touch_previous->touch = touch_current->touch;
	touch_previous->x = touch_current->x;
	touch_previous->y = touch_current->y;
	touch_previous->color = touch_current->color;
	touch_previous->l_weight = touch_current->l_weight;
	touch_previous->action = touch_current->action;

	return;
}

void get_coordinates(screen_t* touch_data, TS_StateTypeDef* TS_State)
{
	uint16_t x = TS_State->touchX[0];
	uint16_t y = TS_State->touchY[0];
	uint16_t border = touch_data->l_weight;

	// Inside the drawing area: draw
	if ((x > border && x < menu_start_x - border) &&
		(y > border && y < max_y - border))
		touch_data->action = 1; // Draw

	// On the menu area: use menu
	else if (x > menu_start_x)
		touch_data->action = 2;	// Use menu

	// On the border area: do nothing
	else
		touch_data->action = 0;	// Do nothing

	touch_data->x = x;
	touch_data->y = y;

	return;
}

void print_point(screen_t* touch_data)
{
	char tmp[10];
	sprintf(tmp, "t: %d\n", touch_data->touch);
	LCD_UsrLog((char*) tmp);
	sprintf(tmp, "x: %d\n", touch_data->x);
	LCD_UsrLog((char*) tmp);
	sprintf(tmp, "y: %d\n", touch_data->y);
	LCD_UsrLog((char*) tmp);
	sprintf(tmp, "c: %x\n", touch_data->color);
	LCD_UsrLog((char*) tmp);
	sprintf(tmp, "l: %d\n", touch_data->l_weight);
	LCD_UsrLog((char*) tmp);
	sprintf(tmp, "s: %d\n", touch_data->action);
	LCD_UsrLog((char*) tmp);

	return;
}

void display_menu(void)
{
	BSP_LCD_LayerDefaultInit(MENU_LAYER, LCD_FRAME_BUFFER);
	BSP_LCD_SetLayerVisible(MENU_LAYER, ENABLE);
	BSP_LCD_SelectLayer(MENU_LAYER);
	//Menu Boarder
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DrawVLine(MENU_X, MENU_Y1, MENU_Y2);
	//Red Button
	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_FillCircle(BUTTON_X, BUTTON1_Y, BUTTON_SIZE);
	//Green Button
	BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	BSP_LCD_FillCircle(BUTTON_X, BUTTON2_Y, BUTTON_SIZE);
	//Blue Button
	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
	BSP_LCD_FillCircle(BUTTON_X, BUTTON3_Y, BUTTON_SIZE);
	//Brush size 1
	BSP_LCD_SetTextColor(LCD_COLOR_GRAY);
	BSP_LCD_FillRect(BUTTON_X2, BUTTON4_Y, BUTTON_Width, BUTTON4_Heigth);
	//Brush size 2
	BSP_LCD_FillRect(BUTTON_X2, BUTTON5_Y, BUTTON_Width, BUTTON5_Heigth);
	//Brush size 3
	BSP_LCD_FillRect(BUTTON_X2, BUTTON6_Y, BUTTON_Width, BUTTON6_Heigth);
	//ERASER
	BSP_LCD_DrawRect(BUTTON_X2, BUTTON7_Y, BUTTON_Width, BUTTON7_Heigth);
	BSP_LCD_DisplayChar(BUTTON_X - 5, BUTTON7_Y + 1, 'E');
}

void set_pen(screen_t *data)
{
	//set color
	if (data->y < 46) {
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		data->color = LCD_COLOR_RED;
	} else if (data->y >= 46 && data->y < 92) {
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		data->color = LCD_COLOR_GREEN;
	} else if (data->y >= 92 && data->y < 142) {
		BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
		data->color = LCD_COLOR_BLUE;
	// set line weight
	} else if (data->y >= 142 && data->y < 168) {
		data->l_weight = 3;
	} else if (data->y >= 168 && data->y < 198) {
		data->l_weight = 6;
	} else if (data->y >= 198 && data->y < 232) {
		data->l_weight = 9;
	//eraser
	} else if (data->y >= 232) {
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		data->color = LCD_COLOR_WHITE;
	}
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
