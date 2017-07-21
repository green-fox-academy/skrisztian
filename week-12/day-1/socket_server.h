/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SOCKET_SERVER_H
#define __SOCKET_SERVER_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#include "cmsis_os.h"
#include "app_ethernet.h"
#include "lwip/sockets.h"
#include <string.h>
#include <stdint.h>

#define SERVER_PORT			54545
#define SERVER_QUEUE_SIZE	2
#define SERVER_BUFF_LEN		98

/* Exported functions ------------------------------------------------------- */
void socket_server_thread(void const *argument);

#endif /* __SOCKET_SERVER_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
