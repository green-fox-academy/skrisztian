/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SOCKET_CLIENT_H
#define __SOCKET_CLIENT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32746g_discovery_ts.h"
#include "socket_server.h"
#include <string.h>
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Menu */
//Circular buttons
#define MENU_X						430
#define MENU_Y1						0
#define MENU_Y2						272
#define MENU_LAYER					0
#define BUTTON_X					456
#define BUTTON1_Y					23
#define BUTTON2_Y					69
#define BUTTON3_Y					115
#define BUTTON_SIZE					20

//Rectengular buttons
#define BUTTON_X2					436
#define BUTTON4_Y					153
#define BUTTON_Width				40
#define BUTTON4_Heigth				6
#define BUTTON5_Y					177
#define BUTTON5_Heigth				12
#define BUTTON6_Y					206
#define BUTTON6_Heigth				18
#define BUTTON7_Y					240
#define BUTTON7_Heigth				25

// double definition
#define LCD_FRAME_BUFFER			SDRAM_DEVICE_ADDR

/* Exported functions ------------------------------------------------------- */
typedef struct {
    uint8_t touch;
    uint16_t x;
    uint16_t y;
    uint32_t color;
    uint8_t l_weight;
    uint8_t action;
} screen_t;

uint8_t screen_t_size;
uint16_t max_x;
uint16_t max_y;
uint16_t menu_start_x;

void socket_client_thread(void const *argument);
uint8_t* serialize_uint8(uint8_t *buffer, uint8_t value);
uint8_t* serialize_uint16(uint8_t *buffer, uint16_t value);
uint8_t* serialize_uint32(uint8_t *buffer, uint32_t value);
uint8_t* serialize_screen_data(uint8_t* buffer, screen_t* screen_data);
void deserialize_screen_data(uint8_t* buffer, screen_t* screen_data);
uint8_t send_screen(int socket, screen_t* screen_data);
void save_touch(screen_t* touch_current, screen_t* touch_previous);
void get_coordinates(screen_t* touch_current, TS_StateTypeDef* TS_State);
void print_point(screen_t* touch_data);
void display_menu(void);
void set_pen(screen_t *data);


#endif /* __SOCKET_CLIENT_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
