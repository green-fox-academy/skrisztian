/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SOCKET_SERVER_H
#define __SOCKET_SERVER_H

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdint.h>
#include <math.h>

/* Defines ------------------------------------------------------------------*/
#define SERVER_PORT			54545
#define SERVER_QUEUE_SIZE	2
#define SERVER_BUFF_LEN		11

#define SERVER_IP			"10.27.6.120"
#define QUEUE_SIZE 			1
#define SOCKET_ERROR		-1

#define LCD_FRAME_BUFFER			SDRAM_DEVICE_ADDR
#define RGB_565_BYTE_PER_PIXEL		2
#define ARGB_BYTE_PER_PIXEL			4

/* Private variables -----------------------------------------------------------*/
typedef struct {
	uint16_t x;
	uint16_t y;
} coordinate_t;

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void socket_server_thread(void const *argument);

#endif /* __SOCKET_SERVER_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
